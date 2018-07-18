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

		/* CPSW NIC */
		CPSW_SS_BASE  = 0x4A100000,
		CPSW_SS_SIZE  = 0x00008000,
		CPSW_PORT     = 0x4A100100,
		CPSW_CPDMA    = 0x4A100800,
		CPSW_STATS    = 0x4A100900,
		CPSW_STATERAM = 0x4A100A00,
		CPSW_CPTS     = 0x4A100C00,
		CPSW_ALE      = 0x4A100D00,
		CPSW_SL1      = 0x4A100D80,
		CPSW_SL2      = 0x4A100DC0,
		MDIO          = 0x4A101000,
		CPSW_WR       = 0x4A101200,
		CPPI_RAM      = 0x4A102000,

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
		CPSW_RX_THRESH_PLS =  40,
		CPSW_RX_PULSE      =  41,
		CPSW_TX_PULSE      =  42,
		CPSW_MISC_PULSE    =  43,
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
		UART_CLOCK = 48*1000*1000,


	};

}


#endif /* _INCLUDE__DRIVERS__DEFS__AM335X_H_ */
