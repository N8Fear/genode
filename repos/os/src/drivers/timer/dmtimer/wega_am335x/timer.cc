/*
 * \brief  Time source for PhyBOARD Wega-AM335x
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

/* base include */
#include <drivers/defs/am335x.h>

/* local include */
#include <time_source.h>

using namespace Genode;

Timer::Time_source::Time_source(Env &env)
:
	Attached_mmio(env, Am335x::DMTIMER_2_BASE, Am335x::DMTIMER_2_SIZE),
	Signalled_time_source(env),
	_timer_irq(env, Am335x::T_INT_2)
{
	_timer_irq.sigh(_signal_handler);
	while (read<tiocp_cfg::softreset>()) ;
}
