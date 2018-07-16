/*
 * \brief  MMIO and IRQ definitions for AM335x based boards
 * \author Hinnerk van Bruinehsen
 * \date   2018-03-05
 */

/*
 * Copyright (C) 2013-2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__DEFS__AM335X_H_
#define _INCLUDE__DRIVERS__DEFS__AM335X_H_

/* Genode includes */
#include <util/mmio.h>
#include <base/env.h>
#include <base/attached_io_mem_dataspace.h>

// TODO: remove when finished:
#include<base/log.h>

namespace Genode {class Cmper; class Cmdpll;}

class Genode::Cmper : Genode::Attached_io_mem_dataspace, public Mmio
{
private:

//	 struct Cmper {
//		 Genode::addr_t mmio_base;
//		 Genode::size_t mmio_size;
//	 };

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

	struct cp_per_cpgmac0_clkctrl : Register<14,32>
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
	public:
		void enable_uart_clock()
		{
			Genode::log("Enable UART clock...");
			write<cm_per_uart1_clkctrl::modulemode>(2);
			while (read<cm_per_uart1_clkctrl::modulemode>() != 2);
			while (!(read<cm_per_l4ls_clkstctrl::clkactivity_l4ls_gclk>() == 1
			      && read<cm_per_l4ls_clkstctrl::clkactivity_uart_gfclk>() == 1));
			Genode::log("UART clocks in CM_PER enabled.");
		}

		void enable_cpsw_clock()
		{
			Genode::log("Enable CPSW clock...");
			Genode::log("Current state: ", read<cm_per_cpsw_clkstctrl::clkactivity_cpsw_125mhz_gclk>());
			write<cm_per_cpsw_clkstctrl::clktrctrl>(2);
			Genode::log("Current state: ", read<cm_per_cpsw_clkstctrl::clkactivity_cpsw_125mhz_gclk>());
		}

		Cmper(Genode::Env &env,
		      Genode::addr_t const mmio_base,
		      Genode::size_t const mmio_size)
		: Genode::Attached_io_mem_dataspace(env, mmio_base, mmio_size),
		  Genode::Mmio((Genode::addr_t)local_addr<void>()) {
			Genode::log("Instatiate CM_PER");
		}
};

/* TODO:
 * Add CM_DPLL
 * - timer2_clk (timer 2)
 * - mac_clksel (mII network)
 * - gpio0_dbclk (GPIO0 debouncing)
 */



class Genode::Cmdpll : Genode::Attached_io_mem_dataspace, public Mmio
{
private:
		struct cm_dpll_clksel_timer7_clk : Register<0x4,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer2_clk : Register<0x8,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer3_clk : Register<0xc,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer4_clk : Register<0x10,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_cm_mac_clksel : Register<0x14,32>
		{
			struct mii_clk_sel : Bitfield<2,1> { };
		};
		struct cm_dpll_clksel_timer5_clk : Register<0x18,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer6_clk : Register<0x1c,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_cm_cpts_rft_clksel : Register<0x20,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_timer1ms_clk : Register<0x28,32>
		{
			struct clksel : Bitfield<0,3> { };
		};
		struct cm_dpll_clksel_gfx_fclk : Register<0x2c,32>
		{
			struct clkdiv_sel_gfx_fclk : Bitfield<0,1> { };
			struct clksel_gfx_clk : Bitfield<1,1> { };
		};
		struct cm_dpll_clksel_pru_icss_ocp_clk : Register<0x30,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_lcdc_pixel_clk : Register<0x34,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_wdt1_clk : Register<0x38,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_gpio0_dbclk : Register<0x3c,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
public:

		Cmdpll(Genode::Env &env,
		       Genode::addr_t const mmio_base,
		       Genode::size_t const mmio_size)
		: Genode::Attached_io_mem_dataspace(env, mmio_base, mmio_size),
		  Genode::Mmio((Genode::addr_t)local_addr<void>()) {
			Genode::log("Instatiate CM_DPLL");
		}
};

namespace Am335x {

	enum {
		RAM_0_BASE = 0x00000000,
		RAM_0_SIZE = 0x20000000, /* GPMC according to Memory Map*/

		CM_PER_BASE = 0x44E00000,
		CM_PER_SIZE = 0x00000400,

		CM_WKUP_BASE = 0x44E00400,
		CM_WKUP_SIZE = 0x00000100,

		CM_DPLL_BASE = 0x44E00500,
		CM_DPLL_SIZE = 0x00000100,

		CM_MPU_BASE = 0x44E00600,
		CM_MPU_SIZE = 0x00000100,

		CM_DEVICE_BASE = 0x44E00700,
		CM_DEVICE_SIZE = 0x00000100,

		CM_RTC_BASE = 0x44E00800,
		CM_RTC_SIZE = 0x00000100,

		CM_GFX_BASE = 0x44E00900,
		CM_GFX_SIZE = 0x00000100,

		CM_CEFUSE_BASE = 0x44E0A00,
		CM_CEFUSE_SIZE = 0x00000100,

		DMTIMER_0_BASE = 0x44E07000,
		DMTIMER_0_SIZE = 0x00001000,

		GPIO_0_BASE = 0x44E07000,
		GPIO_0_SIZE = 0x00001000,

		UART_0_BASE = 0x44E09000,
		UART_0_SIZE = 0x00001000,

		CTR_MOD_BASE = 0x44E10000,
		CTR_MOD_SIZE = 0x00020000,

		DMTIMER_1_BASE = 0x44E31000,
		DMTIMER_1_SIZE = 0x00001000,

		UART_1_BASE = 0x48022000,
		UART_1_SIZE = 0x00001000,

		UART_2_BASE = 0x48024000,
		UART_2_SIZE = 0x00001000,

		DMTIMER_2_BASE = 0x48040000,
		DMTIMER_2_SIZE = 0x00001000,

		DMTIMER_3_BASE = 0x48042000,
		DMTIMER_3_SIZE = 0x00001000,

		DMTIMER_4_BASE = 0x48044000,
		DMTIMER_4_SIZE = 0x00001000,

		/* Possibily later add DMTIMER 5, 6 and 7 */

		GPIO_1_BASE = 0x4804C000,
		GPIO_1_SIZE = 0x00001000,

		/* UART 3, 4 ,5 */

		UART_3_BASE = 0x481A6000,
		UART_3_SIZE = 0x00001000,

		UART_4_BASE = 0x481A8000,
		UART_4_SIZE = 0x00001000,

		UART_5_BASE = 0x481AA000,
		UART_5_SIZE = 0x00001000,

		GPIO_2_BASE = 0x481AC000,
		GPIO_2_SIZE = 0x00001000,

		GPIO_3_BASE = 0x481AE000,
		GPIO_3_SIZE = 0x00001000,

		/* MMC */
		MMCHS_0_BASE = 0x48060000,
		MMCHS_0_SIZE = 0x00001000,

		MMC_1_BASE  = 0x481D8000,
		MMC_1_SIZE  = 0x00001000,

		INTCPS_BASE = 0x48200000,
		INTCPS_SIZE = 0x00001000,

		/* PWM 0, 1 ,2 and LCD */

		/* CPSW/NIC */

		/* Interrupt Numbers */
		WDT_0_INT          =  15,
		USBSSIN	           =  17,
		USBINT_0           =  18,
		USBINT_1           =  19,
		MMCSD_1_INT        =  28,
		MMCSD_2_INT        =  29,
		GPIO_2_IRQ         =  32,
		USBWAKEUP          =  34,
		LCDC_INT           =  36,
		UART_3_INT         =  44,
		UART_4_INT         =  45,
		UART_5_INT         =  46,
		GPIO_3_IRQ         =  62,
		MMCSD_0_INT        =  65,
		T_INT_0            =  66,
		T_INT_1_1MS        =  67,
		T_INT_2            =  68,
		T_INT_3            =  69,
		UART_0_INT         =  72,
		UART_1_INT         =  73,
		UART_2_INT         =  74,
		RTC_INT            =  75,
		RTC_ALARM_INT      =  76,
		DMA_INTR_PIN2      =  90,
		WDT_1_INT          =  91,
		T_INT_4            =  92,
		T_INT_5            =  93,
		T_INT_6            =  94,
		T_INT_7            =  95,
		GPIO_0_IRQ         =  96,
		GPIO_1_IRQ         =  98,
		AES_0_IRQ_S        = 102,
		AES_0_IRQ_P        = 103,
		SHA_IRQ_S          = 108,
		SHA_IRQ_P          = 109,
		RNG_IRQ            = 111,
		DMA_INTR_PIN0      = 123,
		DMA_INTR_PIN1      = 124,

		/* UART CLOCK */
		UART_0_CLOCK = 1000000,
		UART_1_CLOCK = 1000000,
		UART_2_CLOCK = 1000000,
		UART_3_CLOCK = 1000000,
		UART_4_CLOCK = 1000000,
		UART_5_CLOCK = 1000000,


	};

}


#endif /* _INCLUDE__DRIVERS__DEFS__AM335X_H_ */
