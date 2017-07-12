/*
 * \brief  Board definitions for the Raspberry Pi
 * \author Norman Feske
 * \date   2013-04-05
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__SPEC__WEGA_AM335X__DRIVERS__BOARD_BASE_H_
#define _INCLUDE__SPEC__WEGA_AM335X__DRIVERS__BOARD_BASE_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode { struct Board_base; }


struct Genode::Board_base
{
	enum {
		RAM_0_BASE = 0xF00BA4FF,
		RAM_0_SIZE = 0xF00BA4FF, /* XXX ? */

		MMIO_0_BASE = 0xF00BA4FF,
		MMIO_0_SIZE = 0xF00BA4FF,

		/*
		 * IRQ numbers   0..7 refer to the basic IRQs.
		 * IRQ numbers  8..39 refer to GPU IRQs  0..31.
		 * IRQ numbers 40..71 refer to GPU IRQs 32..63.
		 */
		GPU_IRQ_BASE = 0xF00BA4FF,

		SYSTEM_TIMER_IRQ       = 0xF00BA4FF,
		SYSTEM_TIMER_MMIO_BASE = 0xF00BA4FF,
		SYSTEM_TIMER_MMIO_SIZE = 0xF00BA4FF,
		SYSTEM_TIMER_CLOCK     = 0xF00BA4FF,

		PL011_0_IRQ       = 0xF00BA4FF,
		PL011_0_MMIO_BASE = 0xF00BA4FF,
		PL011_0_MMIO_SIZE = 0xF00BA4FF,
		PL011_0_CLOCK     = 0xF00BA4FF,

		IRQ_CONTROLLER_BASE = 0xF00BA4FF,
		IRQ_CONTROLLER_SIZE = 0xF00BA4FF,

		GPIO_CONTROLLER_BASE = 0xF00BA4FF,
		GPIO_CONTROLLER_SIZE = 0xF00BA4FF,

		USB_DWC_OTG_BASE = 0xF00BA4FF,
		USB_DWC_OTG_SIZE = 0xF00BA4FF,

		/* timer */
		TIMER_IRQ = 0xF00BA4FF,

		/* USB host controller */
		DWC_IRQ = 0xF00BA4FF,

		SECURITY_EXTENSION = 0xF00BA4FF,

		/* CPU cache */
		CACHE_LINE_SIZE_LOG2 = 6 /* Change me */,

		/* SD card */
		SDHCI_BASE = 0xF00BA4FF,
		SDHCI_SIZE = 0xF00BA4FF,
		SDHCI_IRQ  = 0xF00BA4FF,
	};

//	enum Videocore_cache_policy { NON_COHERENT = 0xF00BA4FF,
//	                              COHERENT     = 0xF00BA4FF,
//	                              L2_ONLY      = 0xF00BA4FF,
//	                              UNCACHED     = 3 };
};

#endif /* _INCLUDE__SPEC__WEGA_AM335X__DRIVERS__BOARD_BASE_H_ */
