/*
 * \brief  Emulation of Linux kernel interfaces
 * \author Stefan Kalkowski
 * \author Hinnerk van Bruinehsen <hvbruiehsen@stackptr.de>
 * \date   2018-07-18
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/**
 * Unconditionally include common Genode headers _before_ lx_emul.h to
 * prevent shenanigans with macro definitions.
 */
#include <base/attached_io_mem_dataspace.h>
#include <base/attached_rom_dataspace.h>
#include <base/env.h>
#include <base/snprintf.h>
#include <gpio_session/connection.h>
#include <irq_session/client.h>
#include <os/backtrace.h>
#include <util/mmio.h>

#include <component.h>
#include <lx_emul.h>

#include <lx_emul/impl/kernel.h>
#include <lx_emul/impl/delay.h>
#include <lx_emul/impl/slab.h>
#include <lx_emul/impl/work.h>
#include <lx_emul/impl/spinlock.h>
#include <lx_emul/impl/mutex.h>
#include <lx_emul/impl/sched.h>
#include <lx_emul/impl/timer.h>
#include <lx_emul/impl/completion.h>
#include <lx_kit/irq.h>
//#include <lx_kit/mapped_io_mem_range.h>

#include <drivers/defs/am335x.h>


extern "C" { struct page; }

class Control_module : private Genode::Attached_io_mem_dataspace, Genode::Mmio
{
  private:
	enum {
				MAC_ID0_LO = 0x630,
				MAC_ID0_HI = 0x634,
				MAC_ID1_LO = 0x638,
				MAC_ID1_HI = 0x63c,
	};
	struct mac_id0_lo : Register<0x630, 32> {};
	struct mac_id0_hi : Register<0x634, 32> {};
	struct mac_id1_lo : Register<0x638, 32> {};
	struct mac_id1_hi : Register<0x63c, 32> {};
  public:

	Control_module(Genode::Env &env) : Genode::Attached_io_mem_dataspace(env, Am335x::CTR_MOD_BASE, Am335x::CTR_MOD_SIZE, false),
																		 Genode::Mmio((Genode::addr_t)local_addr<void>()) { }

  int regmap_read(unsigned int reg, unsigned int *val) {
		switch (reg) {
		case MAC_ID0_LO:
			*val = read<mac_id0_lo>();
			return 0;

		case MAC_ID0_HI:
			*val = read<mac_id0_hi>();
			return 0;

		case MAC_ID1_LO:
			*val = read<mac_id1_lo>();
			return 0;

		case MAC_ID1_HI:
			*val = read<mac_id1_hi>();
			return 0;

		default:
			Genode::log("Failed to read register ", reg, " from CONTROL_MODULE");
			return -1;
		}
	}

};

static resource _cpsw_resource[] =
{
 { 0x4a100000, 0x800, "cpsw-regs", IORESOURCE_MEM },
 { 0x4a101200, 0x100, "cpsw-regs", IORESOURCE_MEM },
 { 0x44e10650, 0x04,  "gmii-sel",  IORESOURCE_MEM },
 { 0x28, 0x28, "3pgswrxthr0-irq" /* name unused */, IORESOURCE_IRQ },
 { 0x29, 0x29, "3pgwsrxint0-irq" /* name unused */, IORESOURCE_IRQ },
 { 0x2a, 0x2a, "3pgwstxint0-irq" /* name unused */, IORESOURCE_IRQ },
 { 0x2b, 0x2b, "3pgwsmisc0-irq" /* name unused */, IORESOURCE_IRQ },
};

static resource _mdio_resource[] =
{
 { 0x4a101000, 0x00, "mdio-regs", IORESOURCE_MEM },
};

static resource _cpsw_phy_sel_resource[] = {
 { 0x44e10650, 0x04,  "gmii-sel",  IORESOURCE_MEM },
};

class Addr_to_page_mapping : public Genode::List<Addr_to_page_mapping>::Element
{
	private:

		struct page  *_page { nullptr };

		static Genode::List<Addr_to_page_mapping> & _list()
		{
			static Genode::List<Addr_to_page_mapping> _l;
			return _l;
		}

	public:

		Addr_to_page_mapping(struct page *page)
		: _page(page) { }

		static void insert(struct page * page)
		{
			Addr_to_page_mapping *m = (Addr_to_page_mapping*)
				Lx::Malloc::mem().alloc(sizeof (Addr_to_page_mapping));
			m->_page = page;
			_list().insert(m);
		}

		static struct page * remove(unsigned long addr)
		{
			for (Addr_to_page_mapping *m = _list().first(); m; m = m->next())
				if ((unsigned long)m->_page->addr == addr) {
					struct page * ret = m->_page;
					_list().remove(m);
					Lx::Malloc::mem().free(m);
					return ret;
				}

			return nullptr;
		}

		static struct page * find_page(void* addr)
		{
			for (Addr_to_page_mapping *m = _list().first(); m; m = m->next())
				if ((unsigned long)m->_page->addr <= (unsigned long)addr &&
					((unsigned long)m->_page->addr + m->_page->size) > (unsigned long)addr)
					return m->_page;
			return nullptr;
		}
};


struct Device : Genode::List<Device>::Element
{
	struct device * dev; /* Linux device */

	Device(struct device *dev) : dev(dev) {
		list()->insert(this); }

	static Genode::List<Device> *list()
	{
		static Genode::List<Device> _list;
		return &_list;
	}
};


class Driver : public Genode::List<Driver>::Element
{
	private:

		struct device_driver * _drv; /* Linux driver */

	public:

		Driver(struct device_driver *drv) : _drv(drv)
		{
			list()->insert(this);
		}

		/**
		 * List of all currently registered drivers
		 */
		static Genode::List<Driver> *list()
		{
			static Genode::List<Driver> _list;
			return &_list;
		}

		/**
		 * Match device and drivers
		 */
		bool match(struct device *dev)
		{
			/*
			 *  Don't try if buses don't match, since drivers often use 'container_of'
			 *  which might cast the device to non-matching type
			 */
			if (_drv->bus != dev->bus)
				return false;

			return _drv->bus->match ? _drv->bus->match(dev, _drv) : true;
		}

		/**
		 * Probe device with driver
		 */
		int probe(struct device *dev)
		{
			dev->driver = _drv;

			if (dev->bus->probe) return dev->bus->probe(dev);
			else if (_drv->probe)
				return _drv->probe(dev);

			return 0;
		}
};


//struct Gpio_irq : public Genode::List<Gpio_irq>::Element
//{
//	unsigned                         irq_nr;
//	bool                             enabled = true;
//	bool                             pending = false;
//	Gpio::Connection                 gpio;
//	Genode::Irq_session_client       irq;
//	Genode::Signal_handler<Gpio_irq> sigh;
//	Lx::Task                         task;
//	irq_handler_t                    ihandler;
//	void *                           dev_id;
//
//	/**
//	 * List of all currently registered irqs
//	 */
//	static Genode::List<Gpio_irq> *list()
//	{
//		static Genode::List<Gpio_irq> _list;
//		return &_list;
//	}
//
//	static void run_irq(void *args)
//	{
//		Gpio_irq * girq = static_cast<Gpio_irq*>(args);
//		while (1) {
//			Lx::scheduler().current()->block_and_schedule();
//			girq->ihandler(girq->irq_nr, girq->dev_id);
//			girq->irq.ack_irq();
//		}
//	}
//
//	void unblock()
//	{
//		if (enabled) task.unblock();
//
//		pending = !enabled;
//	}
//
//	void enable()
//	{
//		enabled = true;
//		if (pending) unblock();
//	}
//
//	void disable()
//	{
//		enabled = false;
//	}
//
//	Gpio_irq(Genode::Env &env, unsigned nr, irq_handler_t handler, void * dev_id)
//	: irq_nr(nr),
//	  gpio(env, nr),
//	  irq(gpio.irq_session(Gpio::Session::LOW_LEVEL)),
//	  sigh(env.ep(), *this, &Gpio_irq::unblock),
//	  task(run_irq, this, "gpio_irq", Lx::Task::PRIORITY_3, Lx::scheduler()),
//	  ihandler(handler),
//	  dev_id(dev_id)
//	{
//		list()->insert(this);
//
//		irq.sigh(sigh);
//		irq.ack_irq();
//	}
//};


struct Cpsw
{
	using String = Genode::String<128>;

	struct Mdio
	{
		String                name;
		String                mdio_driver;
		struct device_node *  mdio_dn { nullptr };
		struct platform_device * pdev { nullptr };
		struct mii_bus *       mii_bus  { nullptr };
		Genode::Attached_io_mem_dataspace io_ds {Lx_kit::env().env(), 0x4a101000, 0x100 };

		struct Phy
		{
			String              name;
			String              phy_driver;
			String              phy_mode;
			unsigned int        phy_id;
			int                 dual_emac_res_vlan;
			const unsigned      phy_reg;
			struct phy_device * phy_dev { nullptr };

			Phy(String name, String phy_mode, unsigned int id, const unsigned reg)
				: name(name), phy_mode(phy_mode), phy_id(id), phy_reg(reg) {}
		};

		struct Phy_sel
		{
			String              name;
			String              type;
			const unsigned      phy_sel_reg;
			Genode::Attached_io_mem_dataspace io_ds {Lx_kit::env().env(), 0x44e10650, 0x04 };

			Phy_sel(String name, String type, const unsigned phy_sel_reg)
				: name(name), type(type), phy_sel_reg(phy_sel_reg) {}
		};

		enum { MAX = 2 };
		Genode::Constructible<Phy> phys[MAX];
		Genode::Constructible<Phy_sel> phy_sel;

		template <typename FUNC>
		void for_each(FUNC && f)
		{
			for (unsigned i = 0; i < MAX; i++)
				if (phys[i].constructed()) f(*phys[i]);
		}

		Mdio(String name, String mdio_driver) : name(name), mdio_driver(mdio_driver) {};
	};

	String                            name;
	String                            device;
	const unsigned                    irq;
	const size_t                      mmio;
	struct net_device *               net_dev0 { nullptr };
	struct net_device *               net_dev1 { nullptr };
	struct platform_device *          pdev     { nullptr };
	Session_component *               session  { nullptr };
	Genode::Attached_io_mem_dataspace io_ds    { Lx_kit::env().env(), mmio, 0x1000 }; // Size is 0x1000 becausea mdio and  cpdma want their mmio
	Genode::Constructible<Mdio>       mdio;
	Mdio::Phy *                       phy0 { nullptr };
	Mdio::Phy *                       phy1 { nullptr };
	Mdio::Phy_sel *                   phy_sel {nullptr };

	Cpsw(String name, String device, const unsigned irq,
			 const size_t mmio)
		: name(name), device(device), irq(irq), mmio(mmio) {};
};


static Genode::Constructible<Cpsw> cpsw_device;

net_device * Session_component::_register_session_component(Session_component & s,
                                                            Genode::Session_label policy)
{
	Genode::Session_label name = policy.last_element();

	/* No more cards available */
	if (!cpsw_device.constructed()) return nullptr;

	/* Session does not match cards policy */
	if (cpsw_device->name.length() > 1 &&
	    cpsw_device->name != name) return nullptr;

	/* Session already in use */
	if (cpsw_device->session) return nullptr;

	cpsw_device->session = &s;
	return cpsw_device->net_dev0;
}


#include <lx_emul/extern_c_begin.h>
#include <linux/phy.h>
#include <linux/timecounter.h>
#include <lx_emul/extern_c_end.h>

extern "C" {

void lx_backtrace() { Genode::backtrace(); }

int platform_driver_register(struct platform_driver * drv)
{
	using String = Cpsw::String;

	if(!cpsw_device.constructed()){
		Genode::log("CPSW device not constructed: constructing now!");
	  try {
	  	Genode::Attached_rom_dataspace config { Lx_kit::env().env(), "config" };
	  	config.xml().for_each_sub_node("card", [&] (Genode::Xml_node const node) {

	  		Genode::log(__PRETTY_FUNCTION__, " Register card: " );

	  		String name         = node.attribute_value("name", String());
	  		String type         = node.attribute_value("type", String());
	  		unsigned irq        = node.attribute_value("irq",  0UL);
	  		Genode::addr_t mmio = node.attribute_value("mmio", 0UL);
	  		cpsw_device.construct(name, type, irq, mmio);

	  		cpsw_device->mdio.construct("mdio", "ti,davinci_mdio");
	  		cpsw_device->mdio->phys[0].construct("slave", "rmii", 0, 0); // TODO/Cleanup: 0x4a100100); // the register is the P0 control register
	  		cpsw_device->mdio->phys[1].construct("slave", "mii", 1, 1); //0 TODO/Cleanup: x4a100200); // the register is the P1 control register
	  		cpsw_device->mdio->phy_sel.construct("cpsw-phy-sel", "ti,am3352-cpsw-phy-sel", 0x4a100300); //the register is the P3 control register

	  		Genode::log("phy0: ", cpsw_device->mdio->phys[0]->phy_mode);

	  		if (cpsw_device->mdio.constructed()) {
	  			cpsw_device->mdio->for_each([&] (Cpsw::Mdio::Phy & p) {
						if (cpsw_device->phy0 == nullptr)
						  cpsw_device->phy0 = &p;
						else if (cpsw_device->phy1 == nullptr)
						  cpsw_device->phy1 = &p;
						else
							Genode::warning("Trying to assign unexpected phy device!");
	  			});
	  		} else {
					Genode::warning("mdio device was not successfully constructed");
				}

	  	});
	  } catch(...) { }
	}

	Genode::log("Initializing driver for: ",drv->driver.name);

	if (Genode::strcmp(drv->driver.name, "davinci_mdio") == 0) {

	  platform_device * pd = new (Lx::Malloc::dma()) platform_device();

    pd->name = cpsw_device->mdio->name.string();
	  pd->dev.of_node = (device_node *) &cpsw_device->mdio;
	  pd->dev.plat_dev = pd;
	  pd->resource = _mdio_resource;
	  pd->num_resources = 1;
	  cpsw_device->mdio->pdev = pd;

	  drv->probe(pd);
	} else if (Genode::strcmp(drv->driver.name, "cpsw-phy-sel") == 0) {

		platform_device *pd = new (Lx::Malloc::dma()) platform_device();

		pd->name = cpsw_device->mdio->phy_sel->name.string();
		pd->dev.of_node = (device_node *) &cpsw_device->mdio->phy_sel;
		pd->dev.plat_dev = pd;
		pd->resource = _cpsw_phy_sel_resource;
		pd->num_resources = 1;


		drv->probe(pd);
	} else Genode::warning("Driver for ", drv->driver.name, " not implemented!");
	//if (!cpsw_device.constructed()) {
	//	Genode::warning("No valid configuration provided, use default values");
	//	cpsw_device.construct(String(), "ti,cpsw", 41, 0x4A100000, true, true);
	//}

	//for (unsigned i = 0; i < CPSW_MAX; i++) {
	//	if (!cpsw_device.constructed()) break;

	//	if(!cpsw_device->mdio.constructed())
	//		Genode::log("mdio not constructed!!!");

	//	platform_device * pd = new (Lx::Malloc::dma()) platform_device();
	//	platform_device * mdio_pd = new (Lx::Malloc::dma()) platform_device();
	//	device_node * phy0 = new (Lx::Malloc::dma()) device_node();
	//	device_node * phy1 = new (Lx::Malloc::dma()) device_node();
	//	device_node * mdio = new (Lx::Malloc::dma()) device_node();
	//	property * prop = new (Lx::Malloc::dma()) property();
	//	net_device * netdev0 = new (Lx::Malloc::dma()) net_device();
	//	net_device * netdev1 = new (Lx::Malloc::dma()) net_device();

	//	cpsw_device->mdio->mdio_dn = mdio;
	//	cpsw_device->net_dev0 = netdev0;
	//	cpsw_device->net_dev1 = netdev1;
	//	mdio_pd->name    = cpsw_device->mdio->name.string();
	//	mdio_pd->dev.of_node = (device_node*) &cpsw_device->mdio;
	//	mdio_pd->dev.plat_dev = mdio_pd;
	//	cpsw_device->mdio->pdev = mdio_pd;

	//	phy0->name     = "phy0";
	//	phy1->name     = "phy1";
	//	phy0->properties = prop;
	//	phy1->properties = prop;

	//	pd->name         = cpsw_device->name.string();
	//	pd->dev.of_node  = (device_node*) &cpsw_device;

	//	phy0->sibling = phy1;
	//	phy0->parent  = (device_node*) &cpsw_device;
	//	phy1->sibling = nullptr;
	//	phy0->parent  = (device_node*) &cpsw_device;
	//	pd->dev.of_node->child    = phy0;
	//	cpsw_device->pdev = pd;
	//	pd->dev.plat_dev = pd;
	//	pd->resource = _cpsw_resource;
	//	pd->num_resources = 6;
	//	Genode::log("pdev: ", pd, " pdev->dev:  pdev->dev.of_node: ", pd->dev.of_node);
	//	drv->probe(pd);
	//	{
	//		//			net_device * dev = cpsw_device->net_dev0;
	//		//			int err = dev ? dev->netdev_ops->ndo_open(dev) : -1;
	//		//			if (err) {
	//		//				Genode::error("ndo_open()  failed: ", err);
	//		//				return err;
	//		//			}
	//	}
	//}
	return 0;
}


struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name,
                                    unsigned char name_assign_type,
                                    void (*setup)(struct net_device *),
                                    unsigned int txqs, unsigned int rxqs)
{
	Genode::log(__PRETTY_FUNCTION__, " start of function");
	size_t alloc_size = ALIGN(sizeof(struct net_device), NETDEV_ALIGN);
	alloc_size += sizeof_priv;
	alloc_size += NETDEV_ALIGN - 1;

	struct net_device *p   = (struct net_device*) kzalloc(alloc_size,
	                                                      GFP_KERNEL);
	struct net_device *dev = PTR_ALIGN(p, NETDEV_ALIGN);

	INIT_LIST_HEAD(&dev->mc.list);
	dev->mc.count = 0;

	dev->gso_max_segs = GSO_MAX_SEGS;

	setup(dev);

	Genode::log(__PRETTY_FUNCTION__, "before etherops foo");
	static const struct ethtool_ops default_ethtool_ops { };
	if (!dev->ethtool_ops) dev->ethtool_ops = &default_ethtool_ops;

	dev->dev_addr = (unsigned char*) kzalloc(ETH_ALEN, GFP_KERNEL);

	Genode::log(__PRETTY_FUNCTION__, " end of function");
	return dev;
}


bool of_device_is_available(const struct device_node *device)
{
	return device;
}


const struct of_device_id *of_match_device(const struct of_device_id *matches,
                                           const struct device *dev)
{
	Cpsw * cpsw = (Cpsw*) dev->plat_dev->dev.of_node;
	for (; matches && matches->compatible[0]; matches++)
		if (Genode::strcmp(matches->compatible, cpsw->device.string()) == 0)
			return matches;

	TRACE_AND_STOP;
	return nullptr;
}

void * devm_ioremap_resource(struct device *dev, struct resource *res)
{
	Cpsw * cpsw = (Cpsw*) dev->plat_dev->dev.of_node;
	Genode::log(__PRETTY_FUNCTION__ , "name of dev: ", cpsw->name, " res addr: ", res);

	if (Genode::strcmp(cpsw->name.string(), "mdio") == 0) {
		Genode::log(__PRETTY_FUNCTION__, " - requested ioremap for mdio...");
		Cpsw::Mdio * mdio = (Cpsw::Mdio*) dev->plat_dev->dev.of_node;
		Genode::log(__PRETTY_FUNCTION__, " - requested ioremap for mdio..., mdio name: ", mdio->name);
		return mdio->io_ds.local_addr<void>();

	} else if (Genode::strcmp(cpsw->name.string(), "cpsw-phy-sel") == 0) {
		Genode::log(__PRETTY_FUNCTION__, " - requested ioremap for phy_sel...");
		Cpsw::Mdio::Phy_sel * phy_sel = (Cpsw::Mdio::Phy_sel *) dev->plat_dev->dev.of_node;
		return phy_sel->io_ds.local_addr<void>();
	}

	Genode::warning("devm_ioremap_resource is not implemented for ", cpsw->name);
	TRACE_AND_STOP;
}


void *_ioremap(phys_addr_t phys_addr, size_t size, int wc)
{
	Genode::log(__PRETTY_FUNCTION__, " - Beginning");
	try {
		Genode::Attached_io_mem_dataspace *ds = new(Lx::Malloc::mem())
			Genode::Attached_io_mem_dataspace(phys_addr, size, !!wc);
		return ds->local_addr<void>();
	}catch (...) {
		panic("Failed to request I/O memory: (%lx,%lx)", (long unsigned int)phys_addr, phys_addr + size);
			return 0;
	}
}

/* More elegant solution like in devm_ioremap would be better */
void *ioremap(phys_addr_t offset, size_t size) {
	// return Lx::ioremap(offset, size, Genode::UNCACHED); /* TODO - evaluate if lxkit function can be ported/made usable */
	return _ioremap(offset, size, 0);
}


void iounmap(volatile void __iomem *cookie)
{
	Genode::log("Tried to unmap memory: ", cookie);
	TRACE;
}
void platform_set_drvdata(struct platform_device *pdev, void *data)
{
	pdev->dev.driver_data = data;
	struct net_device * ndev = (net_device*)data;
	ndev->dev.of_node = pdev->dev.of_node;
}

int of_get_phy_mode(struct device_node *np)
{
	Cpsw * cpsw = (Cpsw*) &cpsw_device;
	Genode::log(__PRETTY_FUNCTION__, " Need to get PHY mode from phy - need logic for different phys, node: ", np->name);

	for (int i = 0; i < PHY_INTERFACE_MODE_MAX; i++) {
		if (!Genode::strcmp(cpsw->mdio->phys[0]->phy_mode.string(),
		                  phy_modes((phy_interface_t)i)))
			return i;
	}

	return -ENODEV;
}


ktime_t ktime_get_real(void)
{
	Lx::timer_update_jiffies();
	return ktime_get();
}


void timecounter_init(struct timecounter *tc, const struct cyclecounter *cc, u64 start_tstamp)
{
	tc->cc = cc;
	tc->cycle_last = cc->read(cc);
	tc->nsec = start_tstamp;
	tc->mask = (1ULL << cc->shift) - 1;
	tc->frac = 0;
}


void *dma_alloc_coherent(struct device *dev, size_t size,
                         dma_addr_t *dma_handle, gfp_t flag)
{
	void *addr = Lx::Malloc::dma().alloc_large(size);
	dma_addr_t dma_addr = (dma_addr_t) Lx::Malloc::dma().phys_addr(addr);

	*dma_handle = (dma_addr_t) dma_addr;
	return addr;
}


void *dmam_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp)
{
	dma_addr_t dma_addr;
	void *addr;
	if (size > 2048) {
		addr = Lx::Malloc::dma().alloc_large(size);
		dma_addr = (dma_addr_t) Lx::Malloc::dma().phys_addr(addr);
	} else
		addr = Lx::Malloc::dma().alloc(size, 12, &dma_addr);

	*dma_handle = dma_addr;
	return addr;
}


dma_addr_t dma_map_single(struct device *dev, void *cpu_addr, size_t size,
                          enum dma_data_direction)
{
	dma_addr_t dma_addr = (dma_addr_t) Lx::Malloc::dma().phys_addr(cpu_addr);

	if (dma_addr == ~0UL) {

		struct page * p = Addr_to_page_mapping::find_page(cpu_addr);
		if (p) {
			dma_addr = (dma_addr_t) Lx::Malloc::dma().phys_addr(p->addr);
			dma_addr += (dma_addr_t)cpu_addr - (dma_addr_t)p->addr;
		}

		if (dma_addr == ~0UL)
			Genode::error(__func__, ": virtual address ", cpu_addr,
			              " not registered for DMA");
	}

	return dma_addr;
}


int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return (dma_addr == ~0UL) ? 1 : 0;
}


void *dev_get_platdata(const struct device *dev)
{
	return dev->platform_data;
}


int netif_running(const struct net_device *dev)
{
	return dev->state & (1 << __LINK_STATE_START);
}


void netif_carrier_on(struct net_device *dev)
{
	dev->state &= ~(1UL << __LINK_STATE_NOCARRIER);

	Cpsw * cpsw = (Cpsw*) dev->dev.of_node;
	if (cpsw->session) cpsw->session->link_state(true);
}


void netif_carrier_off(struct net_device *dev)
{
	dev->state |= 1UL << __LINK_STATE_NOCARRIER;

	Cpsw * cpsw = (Cpsw*) dev->dev.of_node;
	if (cpsw->session) cpsw->session->link_state(false);
}


int netif_device_present(struct net_device * d)
{
	TRACE;
	return 1;
}



int platform_get_irq(struct platform_device * d, unsigned int i)
{
	if (i > 4) return -1;

	Cpsw * cpsw = (Cpsw*) d->dev.of_node;

	return cpsw->irq + i;
}


int devm_request_irq(struct device *dev, unsigned int irq, irq_handler_t handler, unsigned long irqflags, const char *devname, void *dev_id)
{
	Lx::Irq::irq().request_irq(Platform::Device::create(Lx_kit::env().env(), irq), handler, dev_id);
	return 0;
}


struct clk *devm_clk_get(struct device *dev, const char *id)
{
	static struct clk clocks[] {
	  { "fck", 125*1000*1000 }
	};

	for (unsigned i = 0; i < (sizeof(clocks) / sizeof(struct clk)); i++)
		if (Genode::strcmp(clocks[i].name, id) == 0)
			return &clocks[i];

	Genode::error("MISSING CLOCK: ", id);
	return nullptr;
}


unsigned long clk_get_rate(struct clk * clk)
{
	if (!clk) return 0;
	return clk->rate;
}


int is_valid_ether_addr(const u8 * a)
{
	for (unsigned i = 0; i < ETH_ALEN; i++)
		if (a[i] != 0 && a[i] != 255) return 1;
	return 0;
}


int register_netdev(struct net_device * ndev)
{
	// BROKEN:
	//Genode::log(__PRETTY_FUNCTION__, " Try to register netdev...", d->dev.of_node);
	//d->state |= (1 << __LINK_STATE_START) | (1UL << __LINK_STATE_NOCARRIER);

	//Genode::log(__PRETTY_FUNCTION__, " Succeeded to set state");
	//Cpsw * cpsw = (Cpsw*) d->dev.of_node;
	//Genode::log(__PRETTY_FUNCTION__, " Succeeded to get cpsw");
	//if (cpsw->net_dev0 == nullptr) {
	//  cpsw->net_dev0 = d;
	//} else if (cpsw->net_dev1 == nullptr) {
	//  cpsw->net_dev1 = d;
	//} else {
	//	Genode::log(__PRETTY_FUNCTION__, " - Error: all net devices are initialized!");
	//}

	////cpsw->net_dev = d;
	//Genode::log(__PRETTY_FUNCTION__, " Succeeded to write d to cpsw");

	//Genode::log(__PRETTY_FUNCTION__, " Succeeded to register netdev...");
	//return 0;
	Genode::log("register_netdev: registering device: ", ndev);

	using namespace Genode;
	static bool announce = false;
	int err = -ENODEV;

	//Nic_device *nic = Nic_device::add(ndev);
	Cpsw * cpsw = (Cpsw*) ndev->dev.of_node;
	Genode::log("register_netdev: device: ", ndev);
	Genode::log("register_netdev: device: ", ndev->dev.of_node);
	cpsw->net_dev0 = ndev;


	/* TODO: move to main */
	if  (!announce) {
		//static ::Root root(_signal->env(), Lx:Malloc::mem(), nic);

		announce = true;

		ndev->state |= 1 << __LINK_STATE_START;

		if (err = ndev ->netdev_ops->ndo_open(ndev))
			return err;

		//if (ndev ->netdev_ops->ndo_set_rx_mode)
		//	ndev->netdev_ops->ndo_set_rx_mode(ndev);

		//_nic = nic;
		//_signal->parent().announce(_signal->ep().rpc_ep().manage(&root));
	}

	return err;
}


void *kmem_cache_alloc_node(struct kmem_cache *cache, gfp_t, int)
{
	return (void*)cache->alloc();
}


void get_page(struct page *page)
{
	atomic_inc(&page->_count);
}


static struct page *allocate_pages(gfp_t gfp_mask, size_t const size)
{
	struct page *page = (struct page *)kzalloc(sizeof(struct page), 0);

	page->addr = Lx::Malloc::dma().alloc_large(size);
	page->size = size;

	if (!page->addr) {
		Genode::error("alloc_pages: ", size, " failed");
		kfree(page);
		return 0;
	}

	Addr_to_page_mapping::insert(page);

	atomic_set(&page->_count, 1);

	return page;
}


struct page *alloc_pages(gfp_t gfp_mask, unsigned int order)
{
	size_t const size = PAGE_SIZE << order;
	return allocate_pages(gfp_mask, size);
}


void *__alloc_page_frag(struct page_frag_cache *, unsigned int const fragsz,
                        gfp_t const gfp_mask)
{
	struct page *page = allocate_pages(gfp_mask, fragsz);
	return page ? page->addr : page;
}


void __free_page_frag(void *addr)
{
	struct page *page = Addr_to_page_mapping::remove((unsigned long)addr);

	if (!atomic_dec_and_test(&page->_count))
		Genode::error("page reference count != 0");

	Lx::Malloc::dma().free_large(page->addr);
	kfree(page);
}

int driver_register(struct device_driver *drv)
{
	new (Lx::Malloc::mem()) Driver(drv);
	return 0;
}


int device_add(struct device *dev)
{
	if (dev->driver)
		return 0;

	/* foreach driver match and probe device */
	for (Driver *driver = Driver::list()->first(); driver; driver = driver->next())
		if (driver->match(dev)) {
			int ret = driver->probe(dev);

			if (!ret) return 0;
		}

	return 0;
}


void device_del(struct device *dev)
{
	if (dev->driver && dev->driver->remove)
		dev->driver->remove(dev);
}


int device_register(struct device *dev)
{
	return device_add(dev);
}


void reinit_completion(struct completion *work)
{
	init_completion(work);
}


static void _completion_timeout(unsigned long t)
{
	Lx::Task *task = (Lx::Task *)t;
	task->unblock();
}


long __wait_completion(struct completion *work, unsigned long timeout)
{
	timer_list t;
	Lx::timer_update_jiffies();
	unsigned long j = timeout ? jiffies + timeout : 0;

	if (timeout) {
		setup_timer(&t, _completion_timeout, (unsigned long)Lx::scheduler().current());
		mod_timer(&t, j);
	}

	while (!work->done) {

		if (j && j <= jiffies) {
			lx_log(1, "timeout jiffies %lu", jiffies);
			return 0;
		}

		Lx::Task *task = Lx::scheduler().current();
		work->task = (void *)task;
		task->block_and_schedule();
	}

	if (timeout)
		del_timer(&t);

	work->done = 0;

	return (j  || j == jiffies) ? 1 : j - jiffies;
}


size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = Genode::strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		Genode::memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}


void local_irq_restore(unsigned long f) { }


unsigned long local_irq_save(unsigned long flags) { return flags; }


int pm_runtime_get_sync(struct device *dev)
{
	return 0;
}


void pm_runtime_mark_last_busy(struct device *dev) { }


int in_interrupt(void)
{
	return 0;
}


int pm_runtime_put_autosuspend(struct device *dev)
{
	return 0;
}


int dev_set_name(struct device *dev, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	Genode::String_console sc(dev->name, 32);
	sc.vprintf(fmt, args);

	va_end(args);

	new (Lx::Malloc::mem()) Device(dev);
	return 0;
}


struct device *bus_find_device_by_name(struct bus_type *bus, struct device *start, const char *name)
{
	for (Device *dev = Device::list()->first(); dev; dev = dev->next()) {
		if (Genode::strcmp(dev->dev->name, name) == 0)
			return dev->dev;
	}
	return nullptr;
}


void netif_napi_add(struct net_device *dev, struct napi_struct *napi,
                    int (*poll)(struct napi_struct *, int), int weight)
{
	napi->dev    = dev;
	napi->poll   = poll;
	napi->state  = NAPI_STATE_SCHED;
	napi->weight = weight;
}


const char *dev_name(const struct device *dev)
{
	return dev->name;
}


extern "C" void consume_skb(struct sk_buff *skb);

void dev_kfree_skb_any(struct sk_buff * sk)
{
	consume_skb(sk);
}


void napi_enable(struct napi_struct *n)
{
	clear_bit(NAPI_STATE_SCHED, &n->state);
	clear_bit(NAPI_STATE_NPSVC, &n->state);
}


void napi_disable(struct napi_struct *n)
{
	set_bit(NAPI_STATE_SCHED, &n->state);
	set_bit(NAPI_STATE_NPSVC, &n->state);
}


void __napi_schedule(struct napi_struct *n)
{
	Cpsw * cpsw = (Cpsw*) n->dev->dev.of_node;
	if (cpsw->session) cpsw->session->unblock_rx_task(n);
}


bool napi_schedule_prep(struct napi_struct *n)
{
	return !test_and_set_bit(NAPI_STATE_SCHED, &n->state);
}


void napi_complete(struct napi_struct *n)
{
	clear_bit(NAPI_STATE_SCHED, &n->state);
}


unsigned long find_next_bit(const unsigned long *addr, unsigned long size, unsigned long offset)
{
	unsigned long i  = offset / BITS_PER_LONG;
	offset -= (i * BITS_PER_LONG);

	for (; offset < size; offset++)
		if (addr[i] & (1UL << offset))
			return offset + (i * BITS_PER_LONG);

	return size;
}

long find_next_zero_bit_le(const void *addr,
                           unsigned long size, unsigned long offset)
{
        static unsigned cnt = 0;
        unsigned long max_size = sizeof(long) * 8;
        if (offset >= max_size) {
					Genode::warning("Offset greater max size:", offset, " max: ", max_size);
                return offset + size;
        }

        for (; offset < max_size; offset++)
                if (!(*(unsigned long*)addr & (1L << offset)))
                        return offset;

        return offset + size;
}



gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	Cpsw * cpsw = (Cpsw*) napi->dev->dev.of_node;
	if (cpsw->session) cpsw->session->receive(skb);

	dev_kfree_skb(skb);
	return GRO_NORMAL;
}


void dma_unmap_single(struct device *dev, dma_addr_t addr, size_t size, enum dma_data_direction dir)
{
	// FIXME
	TRACE;
}


bool netif_queue_stopped(const struct net_device *dev)
{
	// FIXME
	TRACE;
	return 0;
}


struct device_node *of_parse_phandle(const struct device_node *np, const char *phandle_name, int index)
{
	Cpsw * cpsw = (Cpsw*) np;
	Genode::log(__PRETTY_FUNCTION__, "Check what should be returned here!");
	return (device_node*) cpsw->mdio->pdev->dev.of_node;
}


struct phy_device *of_phy_connect(struct net_device *dev,
                                  struct device_node *phy_np,
                                  void (*hndlr)(struct net_device *),
                                  u32 flags, int iface)
{
	Cpsw::Mdio::Phy * phy = (Cpsw::Mdio::Phy*) phy_np;
	struct phy_device * phydev = phy ? phy->phy_dev : nullptr;
	if (!phydev) return nullptr;

	phydev->dev_flags = flags;
	int ret = phy_connect_direct(dev, phydev, hndlr, (phy_interface_t)iface);
	return ret ? nullptr : phydev;
}


struct device_node *of_get_child_by_name(const struct device_node *node,
                                         const char *name)
{
	if (Genode::strcmp("mdio", name) != 0) return nullptr;

	Cpsw * cpsw = (Cpsw*) node;
	return cpsw->mdio.constructed() ? (device_node*) &*cpsw->mdio : nullptr;
}


static int of_mdiobus_register_phy(Cpsw::Mdio::Phy & ph, struct mii_bus *mdio)
{
	//struct phy_device * phy =  (struct phy_device*) get_phy_device(mdio, ph.phy_reg, false);
	struct phy_device * phy =  (struct phy_device*) get_phy_device(mdio, ph.phy_reg, false);
	Genode::log(__PRETTY_FUNCTION__, "err status of phy( ph: ", ph.phy_reg, " ):", IS_ERR(phy));

	if (!phy || IS_ERR(phy)) return 1;

	Genode::log(__PRETTY_FUNCTION__, " We aren't doing anything here right now ", phy);

	//phy->irq         = ph.gpio_irq;
	phy->dev.of_node = (device_node*) &ph;

	/* All data is now stored in the phy struct;
	 * register it */
	int rc = phy_device_register(phy);
	if (rc) {
		phy_device_free(phy);
		return 1;
	}

	ph.phy_dev = phy;

	dev_dbg(&mdio->dev, "registered phy at address %i\n", ph.phy_reg);
	dev_info(&mdio->dev, "registered phy at address %i\n", ph.phy_reg);

	return 0;
}

int of_mdiobus_register(struct mii_bus *mdio, struct device_node *np)
{
	Genode::log(__PRETTY_FUNCTION__, "searching breakage");
	Cpsw::Mdio * cpsw_m = (Cpsw::Mdio*) np;

	mdio->phy_mask = ~0;

	Genode::log(__PRETTY_FUNCTION__, "searching breakage");
	/* Clear all the IRQ properties */
	if (mdio->irq)
		for (unsigned i = 0; i<PHY_MAX_ADDR; i++)
			mdio->irq[i] = PHY_POLL;
	Genode::log(__PRETTY_FUNCTION__, "searching breakage");

	mdio->dev.of_node = np;

	Genode::log(__PRETTY_FUNCTION__, "searching breakage");
	/* Register the MDIO bus */
	int rc = mdiobus_register(mdio);
	Genode::log(__PRETTY_FUNCTION__, "searching breakage");
	if (rc) return rc;

	cpsw_m->for_each([&] (Cpsw::Mdio::Phy & phy) {
										 Genode::log("trying to register phy to mdiobus");
					of_mdiobus_register_phy(phy, mdio); });
	Genode::log(__PRETTY_FUNCTION__, "done, exiting...");
	return 0;
}


int of_driver_match_device(struct device *dev, const struct device_driver *drv)
{
	Cpsw::Mdio::Phy * phy = (Cpsw::Mdio::Phy*) dev->of_node;

	return phy ? (Genode::strcmp(drv->name,
	                             phy->phy_driver.string()) == 0) : 0;
}


// HVB-Todo
const void *of_get_property(const struct device_node *node, const char *name, int *lenp)
{
	if (Genode::strcmp("phy_id", name) == 0) {
		if (Genode::strcmp("phy0", node->name) == 0) {
		  *lenp = 2 * sizeof(u32);
		  Genode::log(__PRETTY_FUNCTION__, " get propterty of phy0");
		  return node->properties;
		} else if (Genode::strcmp("phy1", node->name) == 0) {
		  *lenp = 2 * sizeof(u32);
		  Genode::log(__PRETTY_FUNCTION__, " get propterty of phy1");
		  return node->properties;
		} else {
		Genode::log(__PRETTY_FUNCTION__, " getting phy_id/not implemented for node: ", node->name);
		}
	}

	TRACE_AND_STOP;
	return nullptr;
}


void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
	if (size > 2048) Genode::warning("devm_kzalloc ", size);
	return Lx::Malloc::mem().alloc(size);
}


int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev)
{
	//new (Lx::Malloc::mem()) Gpio_irq(Lx_kit::env().env(), irq, handler, dev);
	Genode::log("request_irq");
	return 0;
}


int enable_irq(unsigned int irq)
{
	Genode::log("enable_irq");
	//for (Gpio_irq *girq = Gpio_irq::list()->first(); girq; girq = girq->next())
	//	if (girq->irq_nr == irq) {
	//		girq->enable();
	//		return 0;
	//	}
	return 1;
}


int disable_irq_nosync(unsigned int irq)
{
	Genode::log("disable_irq_nosync");
	//for (Gpio_irq *girq = Gpio_irq::list()->first(); girq; girq = girq->next())
	//	if (girq->irq_nr == irq) {
	//		girq->disable();
	//		return 0;
	//	}
	return 1;
}


struct device_node *of_get_next_available_child(const struct device_node *node, struct device_node *prev)
{
	TRACE_AND_STOP;
	return nullptr;
}


u64 timecounter_read(struct timecounter *tc)
{
	u64 nsec;

	cycle_t cycle_now, cycle_delta;

	/* increment time by nanoseconds since last call */
	{
		/* read cycle counter: */
		cycle_now = tc->cc->read(tc->cc);

		/* calculate the delta since the last timecounter_read_delta(): */
		cycle_delta = (cycle_now - tc->cycle_last) & tc->cc->mask;

		/* convert to nanoseconds: */
		nsec = cyclecounter_cyc2ns(tc->cc, cycle_delta,
								   tc->mask, &tc->frac);

		/* update time stamp of timecounter_read_delta() call: */
		tc->cycle_last = cycle_now;
	}

	nsec += tc->nsec;
	tc->nsec = nsec;

	return nsec;
}

/*********************
 ** DUMMY FUNCTIONS **
 *********************/

void clk_disable_unprepare(struct clk * c)
{
	TRACE;
}

int clk_prepare_enable(struct clk * c)
{
	TRACE;
	return 0;
}

void device_initialize(struct device *dev)
{
	TRACE;
}

int device_init_wakeup(struct device *dev, bool val)
{
	TRACE;
	return 0;
}

struct regulator *__must_check devm_regulator_get(struct device *dev, const char *id)
{
	TRACE;
	return nullptr;
}

struct netdev_queue *netdev_get_tx_queue(const struct net_device *dev, unsigned int index)
{
	TRACE;
	return nullptr;
}

bool of_phy_is_fixed_link(struct device_node *np)
{
	TRACE;
	return 0;
}

int pinctrl_pm_select_default_state(struct device *dev)
{
	TRACE;
	return -1;
}

int pinctrl_pm_select_sleep_state(struct device *dev)
{
	TRACE;
	return -1;
}

struct resource *platform_get_resource(struct platform_device *dev, unsigned type, unsigned num)
{
	unsigned i;

	for (i = 0; i <dev->num_resources; i++) {
		struct resource *r = &dev->resource[i];

		if ((type == r->flags) && num-- == 0)
			return r;
	}

	TRACE;
	return nullptr;
}

void pm_runtime_enable(struct device *dev)
{
	TRACE;
}

void pm_runtime_disable(struct device *dev)
{
	TRACE;
}

void pm_runtime_get_noresume(struct device *dev)
{
	TRACE;
}

int  pm_runtime_set_active(struct device *dev)
{
	TRACE;
	return 0;
}

void pm_runtime_set_autosuspend_delay(struct device *dev, int delay)
{
	TRACE;
}

void pm_runtime_use_autosuspend(struct device *dev)
{
	TRACE;
}

struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info, struct device *parent)
{
	TRACE;
	return (ptp_clock*)0xdeadbeef;
}

int regulator_enable(struct regulator * d)
{
	TRACE;
	return 0;
}

int class_register(struct class_ *cls)
{
	TRACE;
	return 0;
}

int try_module_get(struct module *mod)
{
	TRACE;
	return -1;
}

struct device *get_device(struct device *dev)
{
	TRACE;
	return dev;
}

int device_bind_driver(struct device *dev)
{
	TRACE;
	return 0;
}

void netif_tx_start_all_queues(struct net_device *dev)
{
	TRACE;
}

void netif_tx_lock_bh(struct net_device *dev)
{
	TRACE;
}

int  device_set_wakeup_enable(struct device *dev, bool enable)
{
	TRACE;
	return 0;
}

void trace_consume_skb(struct sk_buff * sb)
{
	TRACE;
}

void trace_kfree_skb(struct sk_buff * sb, void * p)
{
	TRACE;
}

void netif_tx_unlock_bh(struct net_device *dev)
{
	TRACE;
}

void netif_wake_queue(struct net_device * d)
{
	TRACE;
}

bool netdev_uses_dsa(struct net_device *dev)
{
	TRACE;
	return false;
}

void dma_sync_single_for_device(struct device *dev, dma_addr_t addr,
                                size_t size, enum dma_data_direction dir)
{
	TRACE;
}

void dma_sync_single_for_cpu(struct device *dev, dma_addr_t addr, size_t size,
                             enum dma_data_direction dir)
{
	TRACE;
}

/* Used for reference counting only */
void of_node_put(struct device_node *node)
{
	TRACE;
}

const void *of_get_mac_address(struct device_node *np)
{
	TRACE;
	return nullptr;
}

void rtnl_lock(void)
{
	TRACE;
}

void rtnl_unlock(void)
{
	TRACE;
}

int request_module(const char *fmt, ...)
{
	TRACE;
	return 0;
}

int  bus_register(struct bus_type *bus)
{
	TRACE;
	return 0;
}
// HVB


struct gpio_descs *__must_check
devm_gpiod_get_array_optional(struct device *dev, const char *con_id,
			      enum gpiod_flags flags)
{
	/*
   * Searches for con_id "mode", wich results in either "mode-gpio" or "mode-gpios"
   * which both isn't contained in the device tree
	 * If I get the code correctly we are expected to return NULL here on cpsw hardware.
	 *
   */
	return NULL;
}
/*
 * fill in the appropriate values from the device tree files from the linux sources
 *
 * Maybe a Genode devicetree parser would be a good idea in the future.
 */
int of_property_read_u32(const struct device_node *np, const char *propname, u32 *out_value)
{

	Cpsw * cpsw = (Cpsw*) np;

 	if (Genode::strcmp("slaves", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: slaves");
		*out_value = 2;
	}

 	else if (Genode::strcmp("active_slave", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: active_slave");
		*out_value = 0;
	}

 	else if (Genode::strcmp("cpts_clock_mult", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: clock_mult");
		*out_value = 0x80000000;
	}

 	else if (Genode::strcmp("cpts_clock_shift", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: clock_shift");
		*out_value = 29;
	}

 	else if (Genode::strcmp("cpdma_channels", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: cpdma_channels");
		*out_value = 8;
	}

 	else if (Genode::strcmp("ale_entries", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: ale_entries");
		*out_value = 1024;
	}

 	else if (Genode::strcmp("bd_ram_size", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: bd_ram_size");
		*out_value = 0x2000;
	}

 	else if (Genode::strcmp("rx_descs", propname) == 0) {
		*out_value = 0x20;
		// TODO: in the device tree the value is 0x40
	}

 	else if (Genode::strcmp("mac_control", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: mac_control");
		*out_value = 0x20;
	}

 	else if (Genode::strcmp("dual_emac_res_vlan", propname) == 0) {
		Genode::log("of_property_read_u32: query for: dual_emac_res_vlan, for node:", np->name);
	  if (Genode::strcmp("phy0", np->name) == 0) {
		  *out_value = 1;
	  } else if (Genode::strcmp("phy1", np->name) == 0) {
		  *out_value = 2;
	  } else {
	  Genode::log(__PRETTY_FUNCTION__, " dual_emac_res_vlan is no property of node: ", np->name);
		  *out_value = -1;
	  }
	}

 	else if (Genode::strcmp("bus_freq", propname) == 0) {
		Cpsw * cpsw = (Cpsw *) np;
		Genode::log("of_property_read_u32: bus_freq for node " , cpsw->name);
	  if (Genode::strcmp(cpsw->name.string(), "mdio") == 0) {
		  Genode::log("of_property_read_u32: in bus_freq mdio branch ");
		  *out_value = 0xf4240;
		} else {
		  Genode::log("of_property_read_u32: in bus_freq else branch else: " , cpsw->name);
	    *out_value = 1000000;
		}
	}

 	else if (Genode::strcmp("reg", propname) == 0) {
		Cpsw * cpsw = (Cpsw *) np;
		Genode::log("of_property_read_u32: reg for node " , cpsw->name);
	  if (Genode::strcmp(cpsw->name.string(), "mdio") == 0) {
		  Genode::log("of_property_read_u32: in reg mdio branch ");
		  *out_value = 0x4a101000; // Adress of MDIO, in dt there is <0x4a101000 0x100> is the 0x100 the size?
		} else {
		  Genode::log("of_property_read_u32: in reg else branch - which reg is asked for here?" , cpsw->name);
		  *out_value = 0x4a101000; // Adress of MDIO, in dt there is <0x4a101000 0x100> is the 0x100 the size?
		}

	}

 	else if (Genode::strcmp("max_speed", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: max_speed");
		TRACE_AND_STOP;
	}

 	else if (Genode::strcmp("big_endian", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: big_endian");
		TRACE_AND_STOP;
	}

 	else if (Genode::strcmp("little_endian", propname) == 0) {
		//		Genode::log("of_property_read_u32: query for: little_endian");
		TRACE_AND_STOP;
	}
 	else
 		TRACE_AND_STOP;

 	return 0;
}

bool of_property_read_bool(const struct device_node *np,
					 const char *propname)
{
	Cpsw * cpsw = (Cpsw*) np;

 	if (Genode::strcmp("dual_emac", propname) == 0) {
		//		Genode::log("of_property_read_bool: query for: dual_emac");
		return true;
	}

	return false;

}

/* Populates platform_devices from device tree data */
int of_platform_populate(struct device_node *root,
				const struct of_device_id *matches,
				const struct of_dev_auxdata *lookup,
				struct device *parent)
{
	TRACE;
	return 0;
}

struct device_node *of_get_next_child(const struct device_node *node,
					     struct device_node *prev)
{
	Genode::log("In of_get_next child, node: ",node," - prev: ", prev);
	Cpsw * cpsw = (Cpsw*) node;
	// TODO hvb - most likely extend/modify struct
	if (prev == (struct device_node*) 0xffffffff) return nullptr;
	return prev ? prev->sibling : node->child;
	//return cpsw->mdio.constructed() ? (device_node*) &*cpsw->mdio->phys[0] : nullptr;
}

extern "C" int strcmp(const char *s1, const char *s2) {
	// called in cpsw.c by "if(strcmp(slave_node->name, "slave")) return warning about no slave nodes)
	//return Genode::strcmp(s1, s2);
	TRACE_AND_STOP;
	return 0;
}

/* Returns positive integer if machine is compatible */
int of_machine_is_compatible(const char *compat)
{
 	if (Genode::strcmp("ti,am33xx", compat) == 0) {
		// Genode::log("of_device_is_compatible check: ti,33xx compatible");
		return 42;
	}
 	else if (Genode::strcmp("ti,am4372", compat) == 0) {
		//		Genode::log("of_device_is_compatible check: ti,am4372 not compatible");
		return 0;
	}
 	else if (Genode::strcmp("ti,dm8148", compat) == 0) {
		//Genode::log("of_device_is_compatible check: ti,dm8148 not compatible");
		return 0;
	}
 	else if (Genode::strcmp("ti,dra7", compat) == 0) {
		//Genode::log("of_device_is_compatible check: ti,dra7 not compatible");
		return 0;
	}
	else
		TRACE_AND_STOP;
		return 0;
}

struct regmap *syscon_regmap_lookup_by_phandle(struct device_node *np,
					const char *property)
{
}

void eth_random_addr(u8 *addr)
{
  get_random_bytes(addr, ETH_ALEN);
	addr[0] &= 0xfe;	/* clear multicast bit */
	addr[0] |= 0x02;	/* set local assignment bit (IEEE802) */
}

/* Just return 0 - just like the other pm_* functions */
int pm_runtime_put_sync(struct device *dev)
{
	TRACE;
	return 0;
}

/* Return 0 for no match, positive integer for match */
int of_device_is_compatible(const struct device_node *device,
				   const char * compat)
{
 	if (Genode::strcmp("ti,am3517-emac", compat) == 0) {
		 Genode::log("of_device_is_compatible check: ti,am3517-emac");
		return 0;
	}
 	else if (Genode::strcmp("ti,dm816-emac", compat) == 0) {
		 Genode::log("of_device_is_compatible check: ti,dm816-emac");
		return 0;
	}
 	else if (Genode::strcmp("syscon", compat) == 0) {
		 Genode::log("of_device_is_compatible check: syscon");
		return 0;
	}
	else
		TRACE_AND_STOP;
}

/* Is used to read the MAC address of the NICs */
int regmap_read(struct regmap *map, unsigned int reg, unsigned int *val) {
	Control_module ctr_mod(Lx_kit::env().env());
	return ctr_mod.regmap_read(reg, val);
}

u32 __raw_readl(const volatile void __iomem *addr)
{
  return *(const volatile u32 __force *) addr;
}

void __raw_writel(u32 val, volatile void __iomem *addr)
{
	 *(volatile u32 __force *) addr = val;
}

void free_netdev(struct net_device * d)
{
	Genode::log("In free_netdev");
	TRACE;
}

bool ether_addr_equal(const u8 *addr1, const u8 *addr2)
{
	TRACE;
	//HVB: TODO
	return true;
}

/**
 * bitmap_find_next_zero_area_off - find a contiguous aligned zero area
 * @map: The address to base the search on
 * @size: The bitmap size in bits
 * @start: The bitnumber to start searching at
 * @nr: The number of zeroed bits we're looking for
 * @align_mask: Alignment mask for zero area
 * @align_offset: Alignment offset for zero area.
 *
 * The @align_mask should be one less than a power of 2; the effect is that
 * the bit offset of all zero areas this function finds plus @align_offset
 * is multiple of that power of 2.
 */
unsigned long bitmap_find_next_zero_area_off(unsigned long *map,
					     unsigned long size,
					     unsigned long start,
					     unsigned int nr,
					     unsigned long align_mask,
					     unsigned long align_offset)
{
	unsigned long index, end, i;
again:
	index = find_next_zero_bit(map, size, start);

	/* Align allocation */
	index = __ALIGN_MASK(index + align_offset, align_mask) - align_offset;

	end = index + nr;
	if (end > size)
		return end;
	i = find_next_bit(map, end, index);
	if (i < end) {
		start = i + 1;
		goto again;
	}
	return index;
}

unsigned long bitmap_find_next_zero_area(unsigned long *map,
			   unsigned long size,
			   unsigned long start,
			   unsigned int nr,
			   unsigned long align_mask)
{
	return bitmap_find_next_zero_area_off(map, size, start, nr,
					      align_mask, 0);
}

void bitmap_set(unsigned long *map, unsigned int start, int len)
{
        unsigned long *p = map + BIT_WORD(start);
        const unsigned int size = start + len;
        int bits_to_set = BITS_PER_LONG - (start % BITS_PER_LONG);
        unsigned long mask_to_set = BITMAP_FIRST_WORD_MASK(start);

        while (len - bits_to_set >= 0) {
                *p |= mask_to_set;
                len -= bits_to_set;
                bits_to_set = BITS_PER_LONG;
                mask_to_set = ~0UL;
                p++;
        }
        if (len) {
                mask_to_set &= BITMAP_LAST_WORD_MASK(size);
                *p |= mask_to_set;
        }
}


void bitmap_clear(unsigned long *map, unsigned int start, int len)
{
	unsigned long *p = map + BIT_WORD(start);
	const unsigned int size = start + len;
	int bits_to_clear = BITS_PER_LONG - (start % BITS_PER_LONG);
	unsigned long mask_to_clear = BITMAP_FIRST_WORD_MASK(start);

	while (len - bits_to_clear >= 0) {
		*p &= ~mask_to_clear;
		len -= bits_to_clear;
		bits_to_clear = BITS_PER_LONG;
		mask_to_clear = ~0UL;
		p++;
	}
	if (len) {
		mask_to_clear &= BITMAP_LAST_WORD_MASK(size);
		*p &= ~mask_to_clear;
	}
}


struct device_node *of_find_node_by_phandle(phandle handle)
{
	Cpsw * cpsw = (Cpsw*) &cpsw_device;
	Genode::log(__PRETTY_FUNCTION__, " phandle:", handle);
	// TODO hvb - most likely extend/modify struct
	return (device_node*) cpsw->mdio->mdio_dn;
}

struct platform_device *of_find_device_by_node(struct device_node *np)
{
	Cpsw * cpsw = (Cpsw*) &cpsw_device;
	Genode::log(__PRETTY_FUNCTION__, " np: ", np);

	return cpsw->mdio->pdev;
}

// Davinci mdio
void *devres_alloc(dr_release_t release, size_t size, gfp_t gfp)
{
	Cpsw * cpsw = (Cpsw*) &cpsw_device;
	struct mii_bus * mb = (struct mii_bus*) Lx::Malloc::dma().alloc(size);
  TRACE;
  cpsw->mdio->mii_bus = mb;
	Genode::log(__PRETTY_FUNCTION__, " - returning: ", &cpsw->mdio->mii_bus);
	return &cpsw->mdio->mii_bus;
}


void devres_add(struct device *dev, void *res)
{
  TRACE;
}

int of_get_child_count(const struct device_node *np)
{
	struct device_node *child;
	int num = 0;

	for_each_child_of_node(np, child)
		num++;

	Genode::log(__PRETTY_FUNCTION__, " returning num: ", num);
	return num;
}


const struct of_device_id *of_match_node(
	const struct of_device_id *matches, const struct device_node *node)
{
	if (Genode::strcmp(matches->compatible, "ti,am3352-cpsw-phy-sel") == 0)
	  return &matches[0];
	TRACE_AND_STOP;
}

struct resource *platform_get_resource_byname(struct platform_device *dev,
					      unsigned int type,
					      const char *name)
{
	if (Genode::strcmp(name, "gmii-sel") == 0)
		return &_cpsw_phy_sel_resource[0];
	TRACE_AND_STOP;
}


struct property *of_find_property(const struct device_node *np,
					 const char *name,
					 int *lenp)
{
	/* The pointer is not used afterwards and 0xAFFE is easier to grep for than say 1 */
	if (Genode::strcmp(name, "rmii-clock-ext") == 0)
		return (struct property *) 0xAFFE;
	TRACE_AND_STOP;
}

}


void spin_lock(spinlock_t *lock)
{
  TRACE;
}
