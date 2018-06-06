/*
 * \brief  Driver for AM335x UARTs
 * \author Hinnerk van Bruinehsen
 * \date   2018-05-11
 */

/*
 * Copyright (C) 2013-2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

/* Genode includes */
#include <base/attached_io_mem_dataspace.h>
#include <base/env.h>
#include <drivers/uart/am335x.h>
#include <drivers/defs/am335x.h>

enum { UARTS_NUM = 6 }; /* needed by base class definitions */

/* local includes */
#include <uart_driver_base.h>


class Uart::Driver : public Genode::Attached_io_mem_dataspace,
                     public Genode::AM335x_uart,
                     public  Uart::Driver_base
{
	private:

		enum { BAUD_115200 = 115200 };

		struct Uart {
			Genode::addr_t mmio_base;
			Genode::size_t mmio_size;
			int            irq_number;
		};

		Uart & _config(unsigned index)
		{
			using namespace Genode;

			static Uart cfg[UARTS_NUM] = {
				/*
				 * temporary workaround having first UART twice
				 * (most run-scripts have first UART reserved for the kernel)
				 */
				{ Am335x::UART_0_BASE, 4096, Am335x::UART_0_INT },
				{ Am335x::UART_0_BASE, 4096, Am335x::UART_0_INT },
				{ Am335x::UART_1_BASE, 4096, Am335x::UART_1_INT },
				{ Am335x::UART_2_BASE, 4096, Am335x::UART_2_INT },
				{ Am335x::UART_3_BASE, 4096, Am335x::UART_3_INT },
				{ Am335x::UART_4_BASE, 4096, Am335x::UART_4_INT },
				//{ Am335x::UART_5_BASE, 4096, Am335x::UART_5_INT },
			};
			return cfg[index];
		}

		unsigned _baud_rate(unsigned baud_rate)
		{
			if (baud_rate != BAUD_115200)
				Genode::warning("baud_rate ", baud_rate,
				                " not supported, set to default");
			return BAUD_115200;
		}

	public:

		Driver(Genode::Env &env, unsigned index,
		       unsigned baud_rate, Char_avail_functor &func)
		: Genode::Attached_io_mem_dataspace(env, _config(index).mmio_base,
		                                     _config(index).mmio_size),
		  AM335x_uart(env, (Genode::addr_t)local_addr<void>(),
		              Am335x::UART_1_CLOCK, _baud_rate(baud_rate)),
		  Driver_base(env, _config(index).irq_number, func) {
			_rx_enable(); }


		/***************************
		 ** UART driver interface **
		 ***************************/

		void put_char(char c) override { AM335x_uart::put_char(c); }
		bool char_avail()     override { return _rx_avail(); }
		char get_char()       override { return _rx_char();  }
};

#endif /* _UART_DRIVER_H_ */
