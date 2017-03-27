/*
 * \brief  Programmable interrupt controller for core
 * \author Norman Feske
 * \date   2013-04-05
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__INCLUDE__SPEC__RPI__PIC_H_
#define _CORE__INCLUDE__SPEC__RPI__PIC_H_

/* Genode includes */
#include <util/mmio.h>

/* core includes */
#include <board.h>

namespace Genode
{
	/**
	 * Programmable interrupt controller for core
	 */
	class Pic;

//	class Usb_dwc_otg;
}


//class Genode::Usb_dwc_otg : Mmio
//{
//	private:
//
//		struct Core_irq_status : Register<0x14, 32>
//		{
//			struct Sof : Bitfield<3, 1> { };
//		};
//
//		struct Guid : Register<0x3c, 32>
//		{
//			struct Num : Bitfield<0, 14> { };
//
//			/*
//			 * The USB driver set 'Num' to a defined value
//			 */
//			struct Num_valid : Bitfield<31, 1> { };
//
//			/*
//			 * Filter is not used, overridden by the USB driver
//			 */
//			struct Kick : Bitfield<30, 1> { };
//		};
//
//		struct Host_frame_number : Register<0x408, 32>
//		{
//			struct Num : Bitfield<0, 14> { };
//		};
//
//		bool _is_sof() const
//		{
//			return read<Core_irq_status::Sof>();
//		}
//
//		static bool _need_trigger_sof(uint32_t host_frame,
//		                              uint32_t scheduled_frame);
//
//	public:
//
//		Usb_dwc_otg();
//
//		bool handle_sof();
//};


class Genode::Pic : Mmio
{
//	public:
//
//		enum {
//			NR_OF_IRQ = 64,
//
//			/*
//			 * dummy IPI value on non-SMP platform,
//			 * only used in interrupt reservation within generic code
//			 */
//			IPI,
//		};
//
//	private:
//
//		struct Irq_pending_basic : Register<0x0, 32>
//		{
//			struct Timer : Bitfield<0, 1> { };
//			struct Gpu   : Bitfield<8, 2> { };
//		};
//
//		struct Irq_pending_gpu_1  : Register<0x04, 32> { };
//		struct Irq_pending_gpu_2  : Register<0x08, 32> { };
//		struct Irq_enable_gpu_1   : Register<0x10, 32> { };
//		struct Irq_enable_gpu_2   : Register<0x14, 32> { };
//		struct Irq_enable_basic   : Register<0x18, 32> { };
//		struct Irq_disable_gpu_1  : Register<0x1c, 32> { };
//		struct Irq_disable_gpu_2  : Register<0x20, 32> { };
//		struct Irq_disable_basic  : Register<0x24, 32> { };
//
//		Usb_dwc_otg _usb;
//
//		/**
//		 * Return true if specified interrupt is pending
//		 */
//		static bool _is_pending(unsigned i, uint32_t p1, uint32_t p2)
//		{
//			return i < 32 ? (p1 & (1 << i)) : (p2 & (1 << (i - 32)));
//		}
//
//	public:
//
//		Pic();
//
//		void init_cpu_local() { }
//		bool take_request(unsigned &irq);
//		void finish_request() { }
//		void mask();
//		void unmask(unsigned const i, unsigned);
//		void mask(unsigned const i);
};

namespace Kernel { using Genode::Pic; }

#endif /* _CORE__INCLUDE__SPEC__RPI__PIC_H_ */
