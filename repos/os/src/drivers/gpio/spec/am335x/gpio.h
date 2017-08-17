/*
 * \brief  AM335x GPIO definitions
 * \author Ivan Loskutov <ivan.loskutov@ksyslabs.org>
 * \author Stefan Kalkowski <stefan.kalkowski@genode-labs.com>
 * \author Hinnerk van Bruinehsen
 * \date   2017-08-17
 */

/*
 * Copyright (C) 2012 Ksys Labs LLC
 * Copyright (C) 2012-2017 Genode Labs GmbH
 * Copyright (C) 2017 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__GPIO__SPEC__OMAP4__GPIO_H_
#define _DRIVERS__GPIO__SPEC__OMAP4__GPIO_H_

/* Genode includes */
#include <base/attached_io_mem_dataspace.h>
#include <util/mmio.h>

struct Gpio_reg : Genode::Attached_io_mem_dataspace, Genode::Mmio
{
	Gpio_reg(Genode::Env &env,
	         Genode::addr_t const mmio_base,
	         Genode::size_t const mmio_size)
	: Genode::Attached_io_mem_dataspace(env, mmio_base, mmio_size),
	  Genode::Mmio((Genode::addr_t)local_addr<void>()) { }

// TODO: Check definitions
	struct Revision        : Register<0x0,32>
	{
		struct minor  : Bitfield<0,6> {};
		struct custom : Bitfield<6,2> {};
		struct major  : Bitfield<8,3> {};
		struct rtl    : Bitfield<11,5> {};
		struct func   : Bitfield<16,12> {};
		struct scheme : Bitfield<30,2> {};

	};
	struct Sysconfig       : Register<0x10,32>
	{
		struct autoidle : Bitfield<0,1>
		{
			enum { OCP_CLOCK_FREERUNNING = 0, OCP_CLOCK_GATING = 1,};
		};
		struct softreset : Bitfield<1,1>
		{
			enum { NORMAL_MODE = 0, MODULE_RESET = 1,};
		};
		struct enawakeup : Bitfield<2,1>
		{
			enum { WAKEUP_GENERATION_DISABLED = 0, WAKEUP_GENERATION_ENABLED = 1,};
		};
		struct idlemode : Bitfield<3,2>
		{
			enum {
				FORCE_IDLE        = 0,
				NO_IDLE           = 1,
				SMART_IDLE        = 2,
				SMART_IDLE_WAKEUP = 3,
			};
		};
	};
	struct Eoi             : Register<0x20,32>
	{
		struct dmaevent_ack : Bitfield<0,1> {};
	};
	struct Irqstatus_raw_0 : Register_array<0x24, 32, 32, 1> {};
	struct Irqstatus_raw_1 : Register_array<0x28, 32, 32, 1> {};
	struct Irqstatus_0     : Register<0x2c, 32> {}; //TODO: make array
	struct Irqstatus_1     : Register<0x30, 32> {}; //TODO: make array
	struct Irqstatus_set_0 : Register<0x34, 32> {}; //TODO: make array
	struct Irqstatus_set_1 : Register<0x38, 32> {}; //TODO: make array
	struct Irqstatus_clr_0 : Register<0x3c, 32> {}; //TODO: make array
	struct Irqstatus_clr_1 : Register<0x40, 32> {}; //TODO: make array
	struct Irqwaken_0      : Register_array<0x44, 32, 32, 1> {};
	struct Irqwaken_1      : Register_array<0x48, 32, 32, 1> {};
	struct Sysstatus			 : Register<0x114,32>
	{
		struct resetdone : Bitfield<0,1>
		{
			enum { RESET_ONGOING = 0, RESET_COMPLETE = 1,};
		};
	};
	struct Ctrl            : Register<0x130, 32>
	{
		struct disablemodule : Bitfield<0,1>
		{
			enum { MODULE_ENABLED = 0, MODULE_DISABLED = 1,};
		};
		struct gatingratio : Bitfield<1,2>
		{
			enum {
				FUNCTIONAL_CLOCK_IS_INTERFACE_CLOCK       = 0,
				FUNCTIONAL_CLOCK_IS_INTERFACE_CLOCK_DIV_2 = 1,
				FUNCTIONAL_CLOCK_IS_INTERFACE_CLOCK_DIV_4 = 2,
				FUNCTIONAL_CLOCK_IS_INTERFACE_CLOCK_DIV_8 = 3,
			};
		};
	};
	struct Oe              : Register_array<0x134, 32, 32, 1> {};
	struct Datain          : Register_array<0x138, 32, 32, 1> {};
	struct Dataout         : Register_array<0x13C, 32, 32, 1> {};
	struct Leveldetect0    : Register_array<0x140, 32, 32, 1> {};
	struct Leveldetect1    : Register_array<0x144, 32, 32, 1> {};
	struct Risingdetect    : Register_array<0x148, 32, 32, 1> {};
	struct Fallingdetect   : Register_array<0x14c, 32, 32, 1> {};
	struct Debounceenable  : Register_array<0x150, 32, 32, 1> {};
	struct Debouncingtime  : Register<0x154, 32>
	{
		struct Time : Bitfield<0, 8> {};
	};
	struct Cleardataout    : Register<0x190, 32> {};
	struct Setdataout      : Register<0x194, 32> {};
};

#endif /* _DRIVERS__GPIO__SPEC__OMAP4__GPIO_H_ */
