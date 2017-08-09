/*
 * \brief  Time source that uses AM335x's DMTIMER
 * \author Hinnek van Bruinehsen
 * \date   2017-08-04
 */

/*
 * Copyright (C) 2009-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* base include */
#include <drivers/defs/am335x.h>

/* local includes */
#include <time_source.h>

using namespace Genode;


Microseconds Timer::Time_source::max_timeout()
const {
	return Microseconds(_dmtimer.tics_to_us(~0U));
}


void Timer::Time_source::schedule_timeout(Microseconds     duration,
                                          Timeout_handler &handler)
{
	/* make swift current time steady */
	Duration const time = curr_time();
	_curr_time_us = time.trunc_to_plain_us().value;

	/*
	 * Program max timeout in case of duration 0 to avoid lost of accuracy
	 * due to wraps when value is chosen too small. Send instead a signal
	 * manually at end of this method.
	 */
//	unsigned const tics = _dmtimer.us_to_tics(duration.value ? duration.value
//	                                                      : max_timeout().value);

	_handler = &handler;

//	_timer_irq.ack_irq();

//	_dmtimer.start_one_shot(tics);
//

	/* trigger for a timeout 0 immediately the signal */
	if (!duration.value)
		Signal_transmitter(_signal_handler).submit();
}


Duration Timer::Time_source::curr_time()
{
	/* read dmtimer status */
	bool           wrapped   = false;
//	unsigned const max_value = _dmtimer.current_max_value();
//	unsigned const tic_value = _dmtimer.value(wrapped);
	unsigned       passed_tics = 0;

//	if (wrapped)
//		passed_tics += max_value;

//	passed_tics += max_value - tic_value;

//	return Duration(Microseconds(_curr_time_us + _dmtimer.tics_to_us(passed_tics)));
}

Timer::Time_source::Time_source(Env &env)
:
        Signalled_time_source(env),
        _io_mem(env, Am335x::DMTIMER_1_BASE , Am335x::DMTIMER_1_SIZE),
        _timer_irq(env, Am335x::T_INT_1_1MS),
        _dmtimer(reinterpret_cast<addr_t>(_io_mem.local_addr<addr_t>()))
{
	_timer_irq.sigh(_signal_handler);
}

