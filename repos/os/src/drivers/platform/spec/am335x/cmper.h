/*
 * \brief  Regulator driver for clock management unit of AM335x SoC
 * \author Hinnerk van Bruinehsen
 * \date   2018-03-05
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__PLATFORM__SPEC__AM335X__CMPER_H_
#define _DRIVERS__PLATFORM__SPEC__AM335X__CMPER_H_

#include <regulator/consts.h>
#include <regulator/driver.h>
#include <drivers/defs/am335x.h>
#include <os/attached_mmio.h>
#include <base/log.h>

using namespace Regulator;

class Cmper : public Regulator::Driver,
              private Genode::Attached_mmio
{
	private:

		struct cm_per_l4ls_clkstctrl : Register<0x0,32>
		{
			struct clktrctrl : Bitfield<0,2> {};
			struct clkactivity_l4ls_gclk : Bitfield<8,1> {};
			struct clkactivity_uart_gfclk : Bitfield<10,1> {};
			struct clkactivity_can_clk : Bitfield<11,1> {};
			struct clkactivity_timer7_gclk : Bitfield<13,1> {};
			struct clkactivity_timer2_gclk : Bitfield<14,1> {};
			struct clkactivity_timer3_gclk : Bitfield<15,1> {};
			struct clkactivity_timer4_gclk : Bitfield<16,1> {};
			struct clkactivity_lcdc_gclk : Bitfield<17,1> {};
			struct clkactivity_gpio_1_gdbclk : Bitfield<19,1> {};
			struct clkactivity_gpio_2_gdbclk : Bitfield<20,1> {};
			struct clkactivity_gpio_3_gdbclk : Bitfield<21,1> {};
			struct clkactivity_i2c_fclk : Bitfield<24,1> {};
			struct clkactivity_spi_gclk : Bitfield<25,1> {};
			struct clkactivity_timer5_gclk : Bitfield<27,1> {};
			struct clkactivity_timer6_gclk : Bitfield<28,1> {};
		};

		struct cm_per_l3s_clkctrl : Register<0x4,32>
		{
			struct clkctrl : Bitfield<0,2> {};
			struct clkactivity_l3s_gclk : Bitfield<3,1> {};
		};

		struct cm_per_l3_clkstctrl : Register<0xc,32>
		{
			struct clktrcltrl : Bitfield<0,2> {};
			struct clkactivity_emif_gclk : Register<2,1> {};
			struct clkactivity_mmc_fclk : Register<3,1> {};
			struct clkactivity_l3_gclk : Register<4,1> {};
			struct clkactivity_cpts_rft_gclk : Register<6,1> {};
			struct clkactivity_mcasp_gclk : Register<7,1> {};
		};

		struct cm_per_cpgmac0_clkctrl : Register<14,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_lcdc_clkctrl : Register<0x18,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_usb0_clkctrl : Register<0x1c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_tptc0_clkctrl : Register<0x24,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_emif_clkctrl : Register<0x28,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_ocmcram_clkctrl : Register<0x2c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_gpmc_clkctrl : Register<0x30,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mcasp0_clkctrl : Register<0x34,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_uart5_clkctrl : Register<0x38,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mmc0_clkctrl : Register<0x3c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_elm_clkctrl : Register<0x40,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_i2c2_clkctrl : Register<0x44,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_i2c1_clkctrl : Register<0x48,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_spi0_clkctrl : Register<0x4c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_spi1_clkctrl : Register<0x50,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_l4ls_clkctrl : Register<0x60,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mcasp1_clkctrl : Register<0x68,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_uart1_clkctrl : Register<0x6c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_uart2_clkctrl : Register<0x70,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_uart3_clkctrl : Register<0x74,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_uart4_clkctrl : Register<0x78,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_timer7_clkctrl : Register<0x7c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_timer2_clkctrl : Register<0x80,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_timer3_clkctrl : Register<0x84,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_timer4_clkctrl : Register<0x88,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_gpio1_clkctrl : Register<0xac,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_gpio2_clkctrl : Register<0xb0,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_gpio3_clkctrl : Register<0xb4,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_tpcc_clkctrl : Register<0xbc,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_dcan0_clkctrl : Register<0xc0,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_dcan1_clkctrl : Register<0xc4,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_epwmss1_clkctrl : Register<0xcc,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_epwmss0_clkctrl : Register<0xd4,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_epwmss2_clkctrl : Register<0xd8,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_l3_instr_clkctrl : Register<0xdc,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_l3_clkctrl : Register<0xe0,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_ieee5000_clkctrl : Register<0xe4,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_pru_icss_clkctrl : Register<0xe8,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_timer5_clkctrl : Register<0xec,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_timer6_clkctrl : Register<0xf0,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mmc1_clkctrl : Register<0xf4,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mmc2_clkctrl : Register<0xf8,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_tptc1_clkctrl : Register<0xfc,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_tptc2_clkctrl : Register<0x100,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_spinlock_clkctrl : Register<0x10c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_mailbox0_clkctrl : Register<0x110,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_l4hs_clksctrl : Register<0x11c,32>
		{
			struct clkctrl : Bitfield<0,2> {};
			struct clkactivity_l4hs_gclk : Bitfield<3,1> {};
			struct clkactivity_cpsw_25mhz_gclk : Bitfield<4,1> {};
			struct clkactivity_cpsw_50mhz_gclk : Bitfield<5,1> {};
			struct clkactivity_cpsw_5mhz_gclk : Bitfield<6,1> {};
		};

		struct cm_per_l4hs_clkctrl : Register<0x120,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_ocpwp_l3_clksctrl : Register<0x12c,32>
		{
			struct clkctrl : Bitfield<0,2> {};
			struct clkactivity_ocpwp_l3_gclk : Bitfield<4,1> {};
			struct clkactivity_ocpwp_l4_gclk : Bitfield<5,1> {};
		};

		struct cm_per_ocpwp_clkctrl : Register<0x130,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
			struct stbyst : Bitfield<18,1> {};
		};

		struct cm_per_pru_icss_clksctrl : Register<0x140,32>
		{
			struct clktrctrl : Bitfield<0,2> {};
			struct clkactivity_pru_icss_ocp_gclk : Bitfield<4,1> {};
			struct clkactivity_pru_icss_iep_gclk : Bitfield<5,1> {};
			struct clkactivity_pru_icss_uart_gclk : Bitfield<6,1> {};
		};

		struct cm_per_cpsw_clkstctrl : Register<0x144,32>
		{
			struct clktrctrl : Bitfield<0,2> {};
			struct clkactivity_cpsw_125mhz_gclk : Bitfield<4,1> {};
		};

		struct cm_per_lcdc_clksstctrl : Register<0x148,32>
		{
			struct clktrctrl : Bitfield<0,2> {};
			struct clkactivity_lcdc_l3_ocp_gclk : Bitfield<4,1> {};
			struct clkactivity_lcdc_l4_ocp_gclk : Bitfield<5,1> {};
		};

		struct cm_per_clkdiv32k_clkctrl : Register<0x14c,32>
		{
			struct modulemode : Bitfield<0,2> {};
			struct idlest : Bitfield<16,2> {};
		};

		struct cm_per_clk_24mhz_clksstctrl : Register<0x150,32>
		{
			struct clktrctrl : Bitfield<0,2> {};
			struct clkactivity_clk_24mhz_gclk : Bitfield<4,1> {};
		};

		void _enable(Regulator_id id)
		{
			switch (id) {
			case CLK_UART_1:
				_enable_uart_clock();
				break;
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
		  	void _enable_uart_clock()
		  	{
		  		Genode::log("Enable UART clock...");
		  		write<cm_per_uart1_clkctrl::modulemode>(2);
		  		while (read<cm_per_uart1_clkctrl::modulemode>() != 2);
		  		while (!(read<cm_per_l4ls_clkstctrl::clkactivity_l4ls_gclk>() == 1
		  		      && read<cm_per_l4ls_clkstctrl::clkactivity_uart_gfclk>() == 1));
		  		Genode::log("UART clocks in CM_PER enabled.");
		  	}

		  	void _enable_cpsw_clock()
		  	{
          Genode::log("Enable CPSW Clock");

          // write<cm_per_cpgmac0_clkctrl::modulemode>(0x2u);
          //while (0 == (read<cm_per_cpgmac0_clkctrl::idlest>() & 0x3u));

		  		write<cm_per_cpsw_clkstctrl::clktrctrl>(0x2u);
          //while (0 == (read<cm_per_cpsw_clkstctrl::clkactivity_cpsw_125mhz_gclk>() & 0x1u));
		  	}

public:
	/**
	 * Constructor
	 */
	Cmper(Genode::Env &env)
	: Genode::Attached_mmio(env, Am335x::CM_PER_BASE,
	                        Am335x::CM_PER_SIZE)
	{
		Genode::log("Instatiate CM_PER");
    _enable_cpsw_clock();
	}

	virtual ~Cmper() { }

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


#endif /* _DRIVERS__PLATFORM__SPEC__AM335X__CMPER_H_ */
