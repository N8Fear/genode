/*
 * \brief  MMIO and IRQ definitions for AM335x based boards
 * \author Hinnerk van Bruinehsen
 * \date   2017-08-07
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__DEFS__AM335X_H_
#define _INCLUDE__DRIVERS__DEFS__AM335X_H_

/* Genode includes */
//#include <util/mmio.h>

namespace Am335x {
	enum {
		RAM_0_BASE = 0x00000000,
		RAM_0_SIZE = 0x20000000, /* GPMC according to Memory Map*/

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

		GPIO_2_BASE = 0x481AC000,
		GPIO_2_SIZE = 0x00001000,

		GPIO_3_BASE = 0x481AE000,
		GPIO_3_SIZE = 0x00001000,

		/* MMC */

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
		GPIOINT_2_A        =  32,
		GPIOINT_2_B        =  33,
		USBWAKEUP          =  34,
		LCDC_INT           =  36,
		UART_3_INT         =  44,
		UART_4_INT         =  45,
		UART_5_INT         =  46,
		GPIOINT_3_A        =  62,
		GPIOINT_3_B        =  63,
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
		GPIOINT_0_A        =  96,
		GPIOINT_0_B        =  97,
		GPIOINT_1_A        =  98,
		GPIOINT_1_B        =  99,
		AES_0_IRQ_S        = 102,
		AES_0_IRQ_P        = 103,
		SHA_IRQ_S          = 108,
		SHA_IRQ_P          = 109,
		RNG_IRQ            = 111,
		DMA_INTR_PIN0      = 123,
		DMA_INTR_PIN1      = 124,


	};

};


#endif /* _INCLUDE__DRIVERS__DEFS__AM335X_H_ */
