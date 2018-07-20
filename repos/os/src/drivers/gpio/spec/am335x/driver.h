/*
 * \brief  Gpio driver for the AM335X
 * \author Ivan Loskutov <ivan.loskutov@ksyslabs.org>
 * \author Stefan Kalkowski <stefan.kalkowski@genode-labs.com>
 * \author Hinnerk van Bruinehsen
 * \date   2017-07-17
 */

/*
 * Copyright (C) 2012 Ksys Labs LLC
 * Copyright (C) 2012-2017 Genode Labs GmbH
 * Copyright (C) 2017 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__GPIO__SPEC__AM335X__DRIVER_H_
#define _DRIVERS__GPIO__SPEC__AM335X__DRIVER_H_

/* Genode includes */
#include <drivers/defs/am335x.h>
#include <gpio/driver.h>
#include <irq_session/connection.h>
#include <timer_session/connection.h>

/* local includes */
#include "gpio.h"


class Am335x_driver : public Gpio::Driver
{
	private:

		enum {
			PIN_SHIFT = 5,
			MAX_BANKS = 4,
			MAX_PINS  = 32
		};


		class Gpio_bank
		{
			private:

				Gpio_reg                             _reg;
				Genode::Irq_connection               _irq;
				Genode::Signal_handler<Gpio_bank>    _dispatcher;
				Genode::Signal_context_capability    _sig_cap[MAX_PINS];
				bool                                 _irq_enabled[MAX_PINS];

				void _handle()
				{
					_reg.write<Gpio_reg::Irqstatus_0>(0xffffffff);

					unsigned long status = _reg.read<Gpio_reg::Irqstatus_0>();

					for(unsigned i = 0; i < MAX_PINS; i++) {
						if ((status & (1 << i)) && _irq_enabled[i] &&
						    _sig_cap[i].valid())
							Genode::Signal_transmitter(_sig_cap[i]).submit();
					}

					_irq.ack_irq();
				}


			public:

				Gpio_bank(Genode::Env &env,
				          Genode::addr_t base, Genode::size_t size,
				          unsigned irq)
				: _reg(env, base, size), _irq(env, irq),
				  _dispatcher(env.ep(), *this, &Gpio_bank::_handle)
				{
					for (unsigned i = 0; i < MAX_PINS; i++)
						_irq_enabled[i] = false;

					_irq.sigh(_dispatcher);
					_irq.ack_irq();
				}

				Gpio_reg* regs() { return &_reg; }

				void irq(int pin, bool enable)
				{
					if (enable) {
						_reg.write<Gpio_reg::Irqstatus_0>(1 << pin);
						_reg.write<Gpio_reg::Irqstatus_set_0>(1 << pin);
					}
					else
						_reg.write<Gpio_reg::Irqstatus_clr_0>(1 << pin);
					_irq_enabled[pin] = enable;
				}

				void ack_irq(int pin) {
					_reg.write<Gpio_reg::Irqstatus_0>(1 << pin);
					_irq.ack_irq();
				}

				void sigh(int pin, Genode::Signal_context_capability cap) {
					_sig_cap[pin] = cap; }
		};

		Gpio_bank _gpio_bank_0;
		Gpio_bank _gpio_bank_1;
		Gpio_bank _gpio_bank_2;
		Gpio_bank _gpio_bank_3;

		Gpio_bank *_gpio_bank(int gpio)
		{
			switch (gpio >> PIN_SHIFT) {
			case 0:
				return &_gpio_bank_0;
			case 1:
				return &_gpio_bank_1;
			case 2:
				return &_gpio_bank_2;
			case 3:
				return &_gpio_bank_3;
			}

			Genode::error("no Gpio_bank for pin ", gpio, " available");
			return 0;
		}

		int _gpio_index(int gpio)       { return gpio & 0x1f; }

		Am335x_driver(Genode::Env &env)
		:
			_gpio_bank_0(env, Am335x::GPIO_0_BASE, Am335x::GPIO_0_SIZE,
			             Am335x::GPIO_0_IRQ),
			_gpio_bank_1(env, Am335x::GPIO_1_BASE, Am335x::GPIO_1_SIZE,
			             Am335x::GPIO_1_IRQ),
			_gpio_bank_2(env, Am335x::GPIO_2_BASE, Am335x::GPIO_2_SIZE,
			             Am335x::GPIO_2_IRQ),
			_gpio_bank_3(env, Am335x::GPIO_3_BASE, Am335x::GPIO_3_SIZE,
			             Am335x::GPIO_3_IRQ)
		{ }

	public:

		static Am335x_driver& factory(Genode::Env &env);


		/******************************
		 **  Gpio::Driver interface  **
		 ******************************/

		void direction(unsigned gpio, bool input)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Oe>(input ? 1 : 0, _gpio_index(gpio));
		}

		void write(unsigned gpio, bool level)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();

			if (level)
				gpio_reg->write<Gpio_reg::Setdataout>(1 << _gpio_index(gpio));
			else
				gpio_reg->write<Gpio_reg::Cleardataout>(1 << _gpio_index(gpio));
		}

		bool read(unsigned gpio)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			return gpio_reg->read<Gpio_reg::Datain>(_gpio_index(gpio));
		}

		void debounce_enable(unsigned gpio, bool enable)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Debounceenable>(enable ? 1 : 0,
			                                          _gpio_index(gpio));
		}

		void debounce_time(unsigned gpio, unsigned long us)
		{
			unsigned char debounce;

			if (us < 32)
				debounce = 0x01;
			else if (us > 7936)
				debounce = 0xff;
			else
				debounce = (us / 0x1f) - 1;

			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Debouncingtime::Time>(debounce);
		}

		void falling_detect(unsigned gpio)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Leveldetect0> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Leveldetect1> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Fallingdetect>(1, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Risingdetect> (0, _gpio_index(gpio));
		}

		void rising_detect(unsigned gpio)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Leveldetect0> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Leveldetect1> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Fallingdetect>(0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Risingdetect> (1, _gpio_index(gpio));
		}

		void high_detect(unsigned gpio)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Leveldetect0> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Leveldetect1> (1, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Fallingdetect>(0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Risingdetect> (0, _gpio_index(gpio));
		}

		void low_detect(unsigned gpio)
		{
			Gpio_reg *gpio_reg = _gpio_bank(gpio)->regs();
			gpio_reg->write<Gpio_reg::Leveldetect0> (1, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Leveldetect1> (0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Fallingdetect>(0, _gpio_index(gpio));
			gpio_reg->write<Gpio_reg::Risingdetect> (0, _gpio_index(gpio));
		}

		void irq_enable(unsigned gpio, bool enable)
		{
			_gpio_bank(gpio)->irq(_gpio_index(gpio), enable);
		}

		void ack_irq(unsigned gpio)
		{
			_gpio_bank(gpio)->ack_irq(_gpio_index(gpio));
		}

		void register_signal(unsigned gpio,
		                     Genode::Signal_context_capability cap)
		{
			_gpio_bank(gpio)->sigh(_gpio_index(gpio), cap); }

		void unregister_signal(unsigned gpio)
		{
			Genode::Signal_context_capability cap;
			_gpio_bank(gpio)->sigh(_gpio_index(gpio), cap);
		}

		bool gpio_valid(unsigned gpio) { return gpio < (MAX_PINS*MAX_BANKS); }
};

#endif /* _DRIVERS__GPIO__SPEC__AM335X__DRIVER_H_ */
