#include <lx_emul.h>
#include <linux/phy.h>
#include <linux/ptp_clock_kernel.h>
#include <linux/skbuff.h>
#include <linux/timecounter.h>
#include <net/tso.h>

size_t copy_from_user(void *to, void const *from, size_t len)
{
	TRACE_AND_STOP;
	return -1;
}

size_t copy_to_user(void *dst, void const *src, size_t len)
{
	TRACE_AND_STOP;
	return -1;
}

bool device_may_wakeup(struct device *dev)
{
	TRACE_AND_STOP;
	return -1;
}

void device_release_driver(struct device *dev)
{
	TRACE_AND_STOP;
}

int disable_irq(unsigned int irq)
{
	TRACE_AND_STOP;
	return -1;
}

int disable_irq_wake(unsigned int irq)
{
	TRACE_AND_STOP;
	return -1;
}

void dma_free_coherent(struct device * d, size_t size, void *vaddr, dma_addr_t bus)
{
	TRACE_AND_STOP;
}

void dst_release(struct dst_entry *dst)
{
	TRACE_AND_STOP;
}

int enable_irq_wake(unsigned int irq)
{
	TRACE_AND_STOP;
	return -1;
}

void eth_hw_addr_random(struct net_device *dev)
{
	TRACE_AND_STOP;
}

u32 ethtool_op_get_link(struct net_device * d)
{
	TRACE_AND_STOP;
	return -1;
}

int ethtool_op_get_ts_info(struct net_device * d, struct ethtool_ts_info * i)
{
	TRACE_AND_STOP;
	return -1;
}

bool file_ns_capable(const struct file *file, struct user_namespace *ns, int cap)
{
	TRACE_AND_STOP;
	return -1;
}

void free_irq(unsigned int irq, void *dev_id)
{
	TRACE_AND_STOP;
}

void free_netdev(struct net_device * d)
{
	TRACE_AND_STOP;
}

bool gfp_pfmemalloc_allowed(gfp_t g)
{
	TRACE_AND_STOP;
	return -1;
}

int in_irq(void)
{
	TRACE_AND_STOP;
	return -1;
}

struct iphdr *ip_hdr(const struct sk_buff *skb)
{
	TRACE_AND_STOP;
	return NULL;
}

void *kmap_atomic(struct page *page)
{
	TRACE_AND_STOP;
	return NULL;
}

void kunmap_atomic(void *addr)
{
	TRACE_AND_STOP;
}

void netif_tx_disable(struct net_device *dev)
{
	TRACE_AND_STOP;
}

//void netif_tx_stop_queue(struct netdev_queue *dev_queue)
//{
//	TRACE_AND_STOP;
//}

void netif_tx_wake_all_queues(struct net_device * d)
{
	TRACE_AND_STOP;
}

void netif_tx_wake_queue(struct netdev_queue *dev_queue)
{
	TRACE_AND_STOP;
}

int net_ratelimit(void)
{
	TRACE_AND_STOP;
	return -1;
}

ktime_t ns_to_ktime(u64 ns)
{
	ktime_t ret;
	TRACE_AND_STOP;
	return ret;
}

struct timespec64 ns_to_timespec64(const s64 nsec)
{
	struct timespec64 ret;
	TRACE_AND_STOP;
	return ret;
}

struct device_node *of_node_get(struct device_node *node)
{
	TRACE_AND_STOP;
	return NULL;
}

int of_phy_register_fixed_link(struct device_node *np)
{
	TRACE_AND_STOP;
	return -1;
}

void ptp_clock_event(struct ptp_clock *ptp, struct ptp_clock_event *event)
{
	TRACE_AND_STOP;
}

int ptp_clock_index(struct ptp_clock *ptp)
{
	TRACE_AND_STOP;
	return -1;
}

int ptp_clock_unregister(struct ptp_clock *ptp)
{
	TRACE_AND_STOP;
	return -1;
}

int regulator_disable(struct regulator *r)
{
	TRACE_AND_STOP;
	return -1;
}

resource_size_t resource_size(const struct resource *res)
{
	TRACE_AND_STOP;
	return -1;
}

bool page_is_pfmemalloc(struct page *page)
{
	TRACE_AND_STOP;
	return -1;
}

void put_page(struct page *page)
{
	TRACE_AND_STOP;
}

void read_lock_bh(rwlock_t * l)
{
	TRACE_AND_STOP;
}

void read_unlock_bh(rwlock_t * l)
{
	TRACE_AND_STOP;
}

unsigned int tcp_hdrlen(const struct sk_buff *skb)
{
	TRACE_AND_STOP;
	return -1;
}

u64 timecounter_cyc2time(struct timecounter *tc, cycle_t cycle_tstamp)
{
	TRACE_AND_STOP;
	return -1;
}

void tso_build_data(struct sk_buff *skb, struct tso_t *tso, int size)
{
	TRACE_AND_STOP;
}

void tso_build_hdr(struct sk_buff *skb, char *hdr, struct tso_t *tso, int size, bool is_last)
{
	TRACE_AND_STOP;
}

int tso_count_descs(struct sk_buff *skb)
{
	TRACE_AND_STOP;
	return -1;
}

void tso_start(struct sk_buff *skb, struct tso_t *tso)
{
	TRACE_AND_STOP;
}

void unregister_netdev(struct net_device * d)
{
	TRACE_AND_STOP;
}

void __vlan_hwaccel_put_tag(struct sk_buff *skb, __be16 vlan_proto, u16 vlan_tci)
{
	TRACE_AND_STOP;
}

void module_put(struct module *mod)
{
	TRACE_AND_STOP;
}

void put_device(struct device *dev)
{
	TRACE_AND_STOP;
}

int    strcmp(const char *s1, const char *s2)
{
	TRACE_AND_STOP;
	return -1;
}

void class_unregister(struct class *cls)
{
	TRACE_AND_STOP;
}

void bus_unregister(struct bus_type *bus)
{
	TRACE_AND_STOP;
}

void driver_unregister(struct device_driver *drv)
{
	TRACE_AND_STOP;
}


//HVB

u32 __raw_readl(const volatile void __iomem *addr)
{
	TRACE_AND_STOP;
}

void __raw_writel(u32 val, volatile void __iomem *addr)
{
	TRACE_AND_STOP;
}

int of_machine_is_compatible(const char *compat)
{
	TRACE_AND_STOP;
}

int pm_runtime_put_sync(struct device *dev)
{
	TRACE_AND_STOP;
}

struct regmap *syscon_regmap_lookup_by_phandle(struct device_node *np,
					const char *property)
{
	TRACE_AND_STOP;
}

//int netif_receive_skb(struct sk_buff *skb)
//{
//	TRACE_AND_STOP;
//}

void bitmap_set(unsigned long *map, unsigned int start, int len)
{
	TRACE_AND_STOP;
}

void bitmap_clear(unsigned long *map, unsigned int start, int len)
{
	TRACE_AND_STOP;
}

void of_device_unregister(struct platform_device *ofdev)
{
	TRACE_AND_STOP;
}

int of_device_is_compatible(const struct device_node *device,
				   const char * compat)
{
	TRACE_AND_STOP;
}

struct platform_device *of_find_device_by_node(struct device_node *np)
{
	TRACE_AND_STOP;
}

struct device_node *of_find_node_by_phandle(phandle handle)
{
	TRACE_AND_STOP;
}

struct property *of_find_property(const struct device_node *np,
					 const char *name,
					 int *lenp)
{
	TRACE_AND_STOP;
}

struct device_node *of_get_next_child(const struct device_node *node,
					     struct device_node *prev)
{
	TRACE_AND_STOP;
}

const struct of_device_id *of_match_node(
	const struct of_device_id *matches, const struct device_node *node)
{
	TRACE_AND_STOP;
}

struct phy_device *of_phy_find_device(struct device_node *phy_np)
{
	TRACE_AND_STOP;
}

int of_platform_populate(struct device_node *root,
				const struct of_device_id *matches,
				const struct of_dev_auxdata *lookup,
				struct device *parent)
{
	TRACE_AND_STOP;
	return -1;
}

bool of_property_read_bool(const struct device_node *np,
					 const char *propname)
{
	TRACE_AND_STOP;
}

struct resource *platform_get_resource_byname(struct platform_device *dev,
					      unsigned int type,
					      const char *name)
{
	TRACE_AND_STOP;
}

void iounmap(volatile void __iomem *cookie)
{
	TRACE_AND_STOP;
}

__iomem *ioremap(resource_size_t res_cookie, size_t size)
{
	TRACE_AND_STOP;
}

int netif_receive_skb(struct sk_buff *skb)
{
	TRACE_AND_STOP;
}

void devm_kfree(struct device *dev, void *p)
{
	TRACE_AND_STOP;
}

struct gpio_descs *__must_check
devm_gpiod_get_array_optional(struct device *dev, const char *con_id,
			      enum gpiod_flags flags)
{
	TRACE_AND_STOP;
}

int device_for_each_child(struct device *parent, void *data,
			  int (*fn)(struct device *dev, void *data))
{
	TRACE_AND_STOP;
}

struct device *bus_find_device(struct bus_type *bus,
			       struct device *start, void *data,
			       int (*match)(struct device *dev, void *data))
{
	TRACE_AND_STOP;
}
unsigned long
bitmap_find_next_zero_area(unsigned long *map,
			   unsigned long size,
			   unsigned long start,
			   unsigned int nr,
			   unsigned long align_mask)

{
	TRACE_AND_STOP;
}

void pm_runtime_disable(struct device *dev)
{
	TRACE_AND_STOP;
}

bool ether_addr_equal(const u8 *addr1, const u8 *addr2)
{
	TRACE_AND_STOP;
}

void eth_random_addr(u8 *addr)
{
	TRACE_AND_STOP;
}

bool is_broadcast_ether_addr(const u8 *addr)
{
	TRACE_AND_STOP;
}

u32 netif_msg_init(int debug_value, int default_msg_enable_bits)
{
	TRACE_AND_STOP;
}

// HVB end


struct user_namespace init_user_ns;
int sysctl_tstamp_allow_data;
