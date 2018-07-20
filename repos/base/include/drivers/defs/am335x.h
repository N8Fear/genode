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
		EMU_INT            =   0,
		COMMTX             =   1,
		COMMRX             =   2,
		BENCH              =   3,
		ELM_IRQ            =   4,
		SSM_WFI_IRQ        =   5,
		SSM_IRQ            =   6,
		NMI                =   7,
		SEC_EVNT           =   8,
		L3DEBUG            =   9,
		L3APP_INT          =  10,
		PRCM_INT           =  11,
		EDMACOMP_INT       =  12,
		EDMAMPERR          =  13,
		EDMAERR_INT        =  14,
		WDT_0_INT          =  15,
		ADC_TSC_GEN_INT    =  16,
		USBSSIN	           =  17,
		USB_INT_0          =  18,
		USB_INT_1          =  19,
		PRU_ICSS_EVTOUT_0  =  20,
		PRU_ICSS_EVTOUT_1  =  21,
		PRU_ICSS_EVTOUT_2  =  22,
		PRU_ICSS_EVTOUT_3  =  23,
		PRU_ICSS_EVTOUT_4  =  24,
		PRU_ICSS_EVTOUT_5  =  25,
		PRU_ICSS_EVTOUT_6  =  26,
		PRU_ICSS_EVTOUT_7  =  27,
		MMCSD_1_INT        =  28,
		MMCSD_2_INT        =  29,
		I2C_2_INT          =  30,
		E_CAP_0_INT        =  31,
		GPIO_2_IRQ         =  32, //TODO add _A
		GPIO_2_IRQ_B       =  33,
		USB_WAKEUP         =  34,
		PCIE_WAKEUP        =  35,
		LCDC_INT           =  36,
		GFX_INT            =  37,
		/* Reserved        =  38, */
		E_PWM_2_INT        =  39,
		CPSW_RX_THRESH_PLS =  40,
		CPSW_RX_PULSE      =  41,
		CPSW_TX_PULSE      =  42,
		CPSW_MISC_PULSE    =  43,
		UART_3_INT         =  44,
		UART_4_INT         =  45,
		UART_5_INT         =  46,
		E_CAP_INT          =  47,
		/* Reserved        =  48, */
		/* Reserved        =  49, */
		/* Reserved        =  50, */
		/* Reserved        =  51, */
		DCAN_0_INT_0       =  52,
		DCAN_0_INT_1       =  53,
		DCAN_0_PARITY      =  54,
		DCAN_1_INT_0       =  55,
		DCAN_1_INT_1       =  56,
		DCAN_1_PARITY      =  57,
		E_PWM_0_TZ_INT     =  58,
		E_PWM_1_TZ_INT     =  59,
		E_PWM_2_TZ_INT     =  60,
		E_CAP_2_INT        =  61,
		GPIO_3_IRQ         =  62, //TODO add_A
		GPIO_3_IRQ_B       =  63,
		MMCSD_0_INT        =  64,
		MC_SPI_0_INT       =  65,
		T_INT_0            =  66,
		T_INT_1_1MS        =  67,
		T_INT_2            =  68,
		T_INT_3            =  69,
		I2C_0_INT          =  70,
		I2C_1_INT          =  71,
		UART_0_INT         =  72,
		UART_1_INT         =  73,
		UART_2_INT         =  74,
		RTC_INT            =  75,
		RTC_ALARM_INT      =  76,
		MB_INT_0           =  77,
		M3_TXEV            =  78,
		E_QEP_0_INT        =  79,
		MCA_TX_INT_0       =  80,
		MCA_RX_INT_0       =  81,
		MCA_TX_INT_1       =  82,
		MCA_RX_INT_1       =  83,
		/* Reserved        =  84, */
		/* Reserved        =  85, */
		E_PWM_0_INT        =  86,
		E_PWM_1_INT        =  87,
		E_QEP_1_INT        =  88,
		E_QEP_2_INT        =  89,
		DMA_INTR_PIN2      =  90,
		WDT_1_INT          =  91,
		T_INT_4            =  92,
		T_INT_5            =  93,
		T_INT_6            =  94,
		T_INT_7            =  95,
		GPIO_0_IRQ         =  96, //TODO add _A
		GPIO_0_IRQ_B       =  97,
		GPIO_1_IRQ         =  98, //TODO add _A
		GPIO_1_IRQ_B       =  99,
		GPMC_INT           = 100,
		DDR_ERR_0          = 101,
		AES_0_IRQ_S        = 102,
		AES_0_IRQ_P        = 103,
		/* Reserved        = 104, */
		/* Reserved        = 105, */
		/* Reserved        = 106, */
		/* Reserved        = 107, */
		SHA_IRQ_S          = 108,
		SHA_IRQ_P          = 109,
		FPKA_SINTREQUEST_S = 110,
		RNG_IRQ            = 111,
		TC_ERR_INT_0       = 112,
		TC_ERR_INT_1       = 113,
		TC_ERR_INT_2       = 114,
		ADC_TSC_PENINT     = 115,
		/* Reserved        = 116, */
		/* Reserved        = 117, */
		/* Reserved        = 118, */
		/* Reserved        = 119, */
		SMRFLX_MPU         = 120,
		SMRFLX_CORE        = 121,
		/* Reserved        = 122, */
		DMA_INTR_PIN0      = 123,
		DMA_INTR_PIN1      = 124,
		MC_SPI_1_INT       = 125,
		/* Reserved        = 126, */
		/* Reserved        = 127, */

		/* UART CLOCK */
		UART_CLOCK = 48*1000*1000,


	};

}


#endif /* _INCLUDE__DRIVERS__DEFS__AM335X_H_ */
