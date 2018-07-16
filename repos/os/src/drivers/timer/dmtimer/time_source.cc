/*
 * \brief  Time source that uses DMTIMER2 from the AM335x
 * \author Hinnerk van Bruinehsen <hvbruinehsen@stackptr.de>
 * \date   2018-07-16
 */

/*
 * Copyright (C) 2009-2018 Genode Labs GmbH
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* local includes */

#include <time_source.h>

using namespace Genode;


void Timer::Time_source::schedule_timeout(Genode::Microseconds  duration,
                                          Timeout_handler      &handler)
{
	unsigned long const ticks = (1ULL * duration.value * TICKS_PER_MS) / 1000;
	_handler = &handler;
	_timer_irq.ack_irq();
	_cleared_ticks = 0;

	/* disable timer */
	write<tclr::st>(tclr::st::STOP_TIMER);

	/* Reset counter register to 0 */
	write<tcrr>(0);

	/* clear interrupt and install timeout */
	write<irqstatus::mat_it_flag>(1);
	write<tmar>(ticks);
	int prep = tclr::prepare_one_shot();
	write<tclr>(prep);
	write<irq_enable_set::mat_en_flag>(irq_enable_set::mat_en_flag::IRQ_ENABLE);

	/* start timer */
	write<tclr::st>(tclr::st::START_TIMER);
}

Duration Timer::Time_source::curr_time()
{
	unsigned long const uncleared_ticks = read<tcrr>() - _cleared_ticks;
	unsigned long const uncleared_us    = timer_ticks_to_us(uncleared_ticks, TICKS_PER_MS);

	/* update time only on IRQs and if rate is under 1000 per second */
	if (_irq || uncleared_us > 1000) {
		_curr_time.add(Genode::Microseconds(uncleared_us));
		_cleared_ticks += uncleared_ticks;
	}
	return _curr_time;
}
