/*
 * \brief  Timer driver for core
 * \author Hinnerk van Bruinehsen
 * \date   2017-03-27
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__INCLUDE__SPEC__WEGA_AM335X__TIMER_H_
#define _CORE__INCLUDE__SPEC__WEGA_AM335X__TIMER_H_

/* base-hw includes */
#include <kernel/types.h>

/* Genode includes */
#include <util/mmio.h>

/* core includes */
#include <board.h>

namespace Genode { class Timer; }

/**
 * Timer driver for core
 *
 * Timer channel 0 apparently doesn't work on the WEGA_AM335X, so we use channel 1
 */
class Genode::Timer : public Mmio
{
	int dummy;
	private:

		using time_t = Kernel::time_t;

//		enum { TICS_PER_MS = Board::SYSTEM_TIMER_CLOCK / 1000 };
//
//		struct Cs  : Register<0x0, 32> { struct M1 : Bitfield<1, 1> { }; };
//		struct Clo : Register<0x4,  32> { };
//		struct Cmp : Register<0x10, 32> { };
//
	public:

		Timer();

		static unsigned interrupt_id(unsigned const) {
			return Board::SYSTEM_TIMER_IRQ; }

		void   start_one_shot(time_t const tics, unsigned const);
		time_t tics_to_us(time_t const tics) const;
		time_t us_to_tics(time_t const us) const;
		time_t max_value();
		time_t value(unsigned const);
};

namespace Kernel { using Genode::Timer; }

#endif /* _CORE__INCLUDE__SPEC__WEGA_AM335X__TIMER_H_ */
