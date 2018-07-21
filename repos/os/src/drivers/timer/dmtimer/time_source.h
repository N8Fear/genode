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

#ifndef _TIME_SOURCE_H_
#define _TIME_SOURCE_H_

/* Genode includes */
#include <irq_session/connection.h>
#include <os/attached_mmio.h>
#include <drivers/timer/util.h>

/* local includes */
#include <signalled_time_source.h>

namespace Timer { class Time_source; }


class Timer::Time_source : private Genode::Attached_mmio,
                           public Genode::Signalled_time_source
{
	private:

		enum { TICKS_PER_MS = 25000};

		struct tiocp_cfg : Register<0x10,32>
		{
			struct softreset : Bitfield<0,1>
			{
				enum { NO_ACTION = 0, RESET_ONGOING = 1, };
			};

		};

		struct irqstatus : Register<0x28,32>
		{
			struct mat_it_flag : Bitfield<0,1> {
				enum { NO_IRQ = 0, IRQ = 1, };
			};
		};

		struct irq_enable_set : Register<0x2c,32>
		{
			struct mat_en_flag : Bitfield<0,1>
			{
				enum { IRQ_DISABLE = 0, IRQ_ENABLE =1, };
			};
		};

		struct tclr : Register<0x38,32>
		{
			struct st : Bitfield<0,1>
			{
				enum { STOP_TIMER =0, START_TIMER = 1, };
			};

			struct ar : Bitfield<1,1>
			{
				enum { ONE_SHOT = 0, AUTORELOAD = 1, };
			};

			struct ptv : Bitfield<2,3> { };

			struct pre : Bitfield<5,1>
			{
				enum { PRESCALER_DISABLED = 0, PRESCALER_ENABLED = 1, };
			};

			struct ce : Bitfield<6,1>
			{
				enum { COMPARE_MODE_DISABLED = 0, COMPARE_MODE_ENABLED =1, };
			};
			static access_t prepare_one_shot()
			{
				access_t tclr_start = 0;
				ar::set(tclr_start, ar::ONE_SHOT);
				ce::set(tclr_start, ce::COMPARE_MODE_ENABLED);

				return tclr_start;
			}

		};

		struct tmar : Register<0x4c,32> { };
		struct tcrr : Register<0x3c,32> { enum { MAX = ~(access_t) 0 }; };

	Genode::Irq_connection     _timer_irq;
	Genode::Duration           _curr_time     { Genode::Microseconds(0) };
	Genode::Microseconds const _max_timeout   { Genode::timer_ticks_to_us(tcrr::MAX / 2, TICKS_PER_MS) };
	unsigned long              _cleared_ticks { 0 };

	public:

		Time_source(Genode::Env &env);


		/*************************
		 ** Genode::Time_source **
		 *************************/

		Genode::Duration curr_time() override;
		void schedule_timeout(Genode::Microseconds duration, Timeout_handler &handler) override;
		Genode::Microseconds max_timeout() const override { return _max_timeout; }
};

#endif /* _TIME_SOURCE_H_ */
