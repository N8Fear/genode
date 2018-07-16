/*
 * \brief  Pinmux driver for AM335x SoC/Wega Board Control module
 * \author Hinnerk van Bruinehsen
 * \date   2018-03-05
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_
#define _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_

#include <regulator/consts.h>
#include <regulator/driver.h>
#include <drivers/defs/am335x.h>
#include <os/attached_mmio.h>
#include <base/log.h>

using namespace Regulator;

class Pinmux : public Regulator::Driver,
              private Genode::Attached_mmio
{
	private:

	struct conf_gpmc_ad0   : Register<0x0, 32> { };
	struct conf_gpmc_ad1   : Register<0x4, 32> { };
	struct conf_gpmc_ad2   : Register<0x8, 32> { };
	struct conf_gpmc_ad3   : Register<0xC, 32> { };
	struct conf_gpmc_ad4   : Register<0x10,32> { };
	struct conf_gpmc_ad5   : Register<0x14,32> { };
	struct conf_gpmc_ad6   : Register<0x18,32> { };
	struct conf_gpmc_ad7   : Register<0x1c,32> { };
	struct conf_gpmc_ad8   : Register<0x20,32> { };
	struct conf_gpmc_ad9   : Register<0x24,32> { };
	struct conf_gpmc_ad10  : Register<0x28,32> { };
	struct conf_gpmc_ad11  : Register<0x2c,32> { };
	struct conf_gpmc_ad12  : Register<0x30,32> { };
	struct conf_gpmc_ad13  : Register<0x34,32> { };
	struct conf_gpmc_ad14  : Register<0x38,32> { };
	struct conf_gpmc_ad15  : Register<0x3c,32> { };

	struct conf_gpmc_a0    : Register<0x40, 32> { };
	struct conf_gpmc_a1    : Register<0x44, 32> { };
	struct conf_gpmc_a2    : Register<0x48, 32> { };
	struct conf_gpmc_a3    : Register<0x4C, 32> { };
	struct conf_gpmc_a4    : Register<0x50, 32> { };
	struct conf_gpmc_a5    : Register<0x54, 32> { };
	struct conf_gpmc_a6    : Register<0x58, 32> { };
	struct conf_gpmc_a7    : Register<0x5c, 32> { };
	struct conf_gpmc_a8    : Register<0x60, 32> { };
	struct conf_gpmc_a9    : Register<0x64, 32> { };
	struct conf_gpmc_a10   : Register<0x68, 32> { };
	struct conf_gpmc_a11   : Register<0x6c, 32> { };

	struct conf_gpmc_wait0 : Register<0x70, 32> { };
	struct conf_gpmc_wpn   : Register<0x74, 32> { };
	struct conf_gpmc_ben1  : Register<0x78, 32> { };

  /* Fill in some more missing pinmux registers : not required right now */

	/* MMC0 related pinmuxing */
	struct conf_mmc0_dat3  : Register<0xf0, 32> { };
	struct conf_mmc0_dat2  : Register<0xf4, 32> { };
	struct conf_mmc0_dat1  : Register<0xf8, 32> { };
	struct conf_mmc0_dat0  : Register<0xfc, 32> { };
	struct conf_mmc0_clk   : Register<0x100,32> { };
	struct conf_mmc0_cmd   : Register<0x104,32> { };

  /* CPSW/PHY related pinmuxing */
	struct conf_mii1_col      : Register<0x108, 32> { };
	struct conf_mii1_crs      : Register<0x10c, 32> { };
	struct conf_mii1_rx_er    : Register<0x110, 32> { };
	struct conf_mii1_tx_en    : Register<0x114, 32> { };
	struct conf_mii1_rx_dv    : Register<0x118, 32> { };
	struct conf_mii1_txd3     : Register<0x11c, 32> { };
	struct conf_mii1_txd2     : Register<0x120, 32> { };
	struct conf_mii1_txd1     : Register<0x124, 32> { };
	struct conf_mii1_txd0     : Register<0x128, 32> { };
	struct conf_mii1_tx_clk   : Register<0x12c, 32> { };
	struct conf_mii1_rx_clk   : Register<0x130, 32> { };
	struct conf_mii1_rxd3     : Register<0x134, 32> { };
	struct conf_mii1_rxd2     : Register<0x138, 32> { };
	struct conf_mii1_rxd1     : Register<0x13c, 32> { };
	struct conf_mii1_rxd0     : Register<0x140, 32> { };
	struct conf_rmii1_ref_clk : Register<0x144, 32> { };
	struct conf_mdio          : Register<0x148, 32> { };
	struct conf_mdc           : Register<0x14c, 32> { };


	void _cpsw_pinmux_setup()
	{
		/* Ethernet 0 from device tree */
		Genode::log("Setting up pinmuxing for CPSW ethernet");
		write<conf_mii1_crs>(0x21u);
		write<conf_mii1_rx_er>(0x21u);
		write<conf_mii1_tx_en>(0x9u);
		write<conf_mii1_txd1>(0x9u);
		write<conf_mii1_txd0>(0x9u);
		write<conf_mii1_rxd1>(0x21u);
		write<conf_mii1_rxd0>(0x21u);
		write<conf_rmii1_ref_clk>(0x20u);
		write<conf_mdio>(0x70u);

		/* Ethernet 1 from device tree */
		write<conf_gpmc_a0>(0x09u);
		write<conf_gpmc_a1>(0x21u);
		write<conf_gpmc_a2>(0x09u);
		write<conf_gpmc_a3>(0x09u);
		write<conf_gpmc_a4>(0x09u);
		write<conf_gpmc_a5>(0x09u);
		write<conf_gpmc_a6>(0x21u);
		write<conf_gpmc_a7>(0x21u);
		write<conf_gpmc_a8>(0x21u);
		write<conf_gpmc_a9>(0x21u);
		write<conf_gpmc_a10>(0x21u);
		write<conf_gpmc_a11>(0x21u);
		write<conf_gpmc_wpn>(0x21u);
		write<conf_gpmc_ben1>(0x21u);


		/* From starterware /camkes */
		//write<conf_mii1_rx_er>(0x0 | 0x20u);
		//write<conf_mii1_tx_en>(0x0);
		//write<conf_mii1_rx_dv>(0x0 | 0x20u);
		//write<conf_mii1_txd3>(0x0);
		//write<conf_mii1_txd2>(0x0);
		//write<conf_mii1_txd1>(0x0);
		//write<conf_mii1_txd0>(0x0);
		//write<conf_mii1_tx_clk>(0x0 | 0x20u);
		//write<conf_mii1_rx_clk>(0x0 | 0x20u);
		//write<conf_mii1_rxd3>(0x0 | 0x20u);
		//write<conf_mii1_rxd2>(0x0 | 0x20u);
		//write<conf_mii1_rxd1>(0x0 | 0x20u);
		//write<conf_mii1_rxd0>(0x0 | 0x20u);
		//write<conf_mdio>(0x0 | 0x10u);
		//write<conf_mdc>(0x0 | 0x10u);
    }

		void _enable(Regulator_id id)
		{
			switch (id) {
				//case CPSW_PINMUX:
				//	_cpsw_pinmux_setup();
				//	break;
			default:
				Genode::warning("enabling regulator unsupported for ", names[id].name);
			}
		}

		void _disable(Regulator_id id)
		{
			switch (id) {
			default:
				Genode::warning("disabling regulator unsupported for ", names[id].name);
			}
		}
public:
	//	  	void _enable_uart_clock()
	//	  	{
	//	  		Genode::log("Enable UART clock...");
	//	  		write<cm_per_uart1_clkctrl::modulemode>(2);
	//	  		while (read<cm_per_uart1_clkctrl::modulemode>() != 2);
	//	  		while (!(read<cm_per_l4ls_clkstctrl::clkactivity_l4ls_gclk>() == 1
	//	  		      && read<cm_per_l4ls_clkstctrl::clkactivity_uart_gfclk>() == 1));
	//	  		Genode::log("UART clocks in CM_PER enabled.");
	//	  	}
	//
	//	  	void enable_cpsw_clock()
	//	  	{
	//	  		Genode::log("Enable CPSW clock...");
	//	  		Genode::log("Current state: ", read<cm_per_cpsw_clkstctrl::clkactivity_cpsw_125mhz_gclk>());
	//	  		write<cm_per_cpsw_clkstctrl::clktrctrl>(2);
	//	  		Genode::log("Current state: ", read<cm_per_cpsw_clkstctrl::clkactivity_cpsw_125mhz_gclk>());
	//	  	}

	/**
	 * Constructor
	 */
	Pinmux(Genode::Env &env)
		: Genode::Attached_mmio(env, (Am335x::CTR_MOD_BASE + 0x800),
	                        0x234)
	{
		Genode::log("Setting up pinmuxing");
		_cpsw_pinmux_setup();
	}

	virtual ~Pinmux() { }

	/********************************
	 ** Regulator driver interface **
	 ********************************/

	void level(Regulator_id id, unsigned long level)
	{
		switch (id) {
		default:
			Genode::warning("disabling regulator unsupported for ", names[id].name);
		}
	}

	unsigned long level(Regulator_id id)
	{
		switch (id) {
		default:
			Genode::warning("disabling regulator unsupported for ", names[id].name);
		}
		return 0;
	}

	void state(Regulator_id id, bool enable)
	{
		if (enable)
			_enable(id);
		else
			_disable(id);
	}

	bool state(Regulator_id id)
	{
		switch (id) {
		default:
			Genode::warning("state request unsupported for ", names[id].name);
		}
	}
};


#endif /* _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_ */
