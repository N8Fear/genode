/*
 * \brief  Driver base for the AM335x DMTIMER
 * \author Hinnerk van Bruinehsen
 * \date   2017-08-09
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 * Copyright (C) 2017 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _DMTIMER_H_
#define _DMTIMER_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode { class Dmtimer_base; }


class Genode::Dmtimer_base : public Mmio
{
	protected:

		enum { TICS_PER_MS = 66000 };

		/**
		 * TIDR Register
		 * This read only register contains the revision number of the module. A
		 * write to this register has no effect.  This register is used by software
		 * to track features, bugs, and compatibility.
		 */
		struct tidr : Register<0x0,32>
		{
			struct y_minor : Bitfield<0,6> { };
			struct custom  : Bitfield<6,2> { };
			struct x_major : Bitfield<8,3> { };
			struct r_rtl   : Bitfield<11,5> { };
			struct func    : Bitfield<16,12> { };
			struct scheme  : Bitfield<30,2> { };
		};

		/**
		 * TIOCP_CFG Register
		 * This register allows controlling various parameters of the OCP interface.
		 */
		struct tiocp_cfg : Register<0x10,32>
		{
			/* Software reset (automatically set by hardware) */
			struct softreset : Bitfield<0,1>
			{
				enum {
					NO_ACTION     = 0,
					RESET_ONGOING = 1,
				};
			};

			/* Sensitivity to emulation suspend events from Debug subsystem. */
			struct emulfree : Bitfield<1,1>
			{
				enum { TIMER_FROZEN = 0, TIMER_FREE   = 1, };
			};

			/* Powermanagement, req/ack control */
			struct idlemode : Bitfield<2,2>
			{
				enum {
					FORCE_IDLE      = 0,
					NO_IDLE         = 1,
					SMART_IDLE      = 2,
					SIDLE_IDLE_WAKE = 3,
				};
			};
		};

		/**
		 * IRQ_EOI Register
		 * This module supports DMA events with its interrupt signal. This register
		 * must be written to after the DMA completes in order for subsequent DMA
		 * events to be triggered from this module.
		 */
		struct irq_eoi : Register<0x20,32>
		{
			struct dmaevent_ack : Bitfield<0,1>
			{
				enum { ACK_DMA_EVENT = 0, };
			};
		};

		/**
		 * IRQSTATUS_RAW
		 * Component interrupt request status. Check the corresponding secondary
		 * status register. Raw status is set even if event is not enabled. Write 1
		 * to set the (raw) status, mostly for debug.
		 */
		struct irqstatus_raw : Register<0x24,32>
		{
			struct mat_it_flag : Bitfield<0,1>
			{
				enum {
					MAT_IRQ_PENDING  = 0,
					MAT_IRQ_NO       = 1,
				};
			};

			struct ovf_it_flag : Bitfield<1,1>
			{
				enum {
					OVF_IRQ_PENDING = 0,
					OVF_IRQ_NO      = 1,
				};
			};

			struct tcar_it_flag : Bitfield<2,1>
			{
				enum {
					TCAR_IRQ_PENDING = 0,
					TCAR_IRQ_NO      = 1,
				};
			};
		};
		/**
		 * IRQENABLE_SET Register
		 * Component interrupt request enable. Write 1 to set (enable interrupt).
		 * Readout equal to corresponding _CLR register.
		 */
		struct irqenable_set: Register<0x2C,32>
		{
			struct mat_en_flag : Bitfield<0,1>
			{
				enum {
					MAT_IRQ_ENABLED  = 0,
					MAT_IRQ_DISABLED = 1,
				};
			};

			struct ovf_en_flag : Bitfield<1,1>
			{
				enum {
					OVF_IRQ_ENABLED  = 0,
					OVF_IRQ_DISABLED = 1,
				};
			};

			struct tcar_en_flag : Bitfield<2,1>
			{
				enum {
					TCAR_IRQ_ENABLED  = 0,
					TCAR_IRQ_DISABLED = 1,
				};
			};
		};

		/**
		 * IRQENABLE_CLR Register
		 * Component interrupt request enable. Write 1 to clear (disable
		 * interrupt). Readout equal to corresponding _SET register.
		 */
		struct irqenable_clr: Register<0x30,32>
		{
			struct mat_en_flag : Bitfield<0,1>
			{
				enum {
					MAT_CLR_ENABLED  = 0,
					MAT_CLR_DISABLED = 1,
				};
			};

			struct ovf_en_flag : Bitfield<1,1>
			{
				enum {
					OVF_CLR_ENABLED  = 0,
					OVF_CLR_DISABLED = 1,
				};
			};

			struct tcar_en_flag : Bitfield<2,1>
			{
				enum {
					TCAR_CLR_ENABLED  = 0,
					TCAR_CLR_DISABLED = 1,
				};
			};
		};

		/**
		 * IRQWAKEEN Register
		 * IRQWAKEEN is shown in Figure 20-13 and described in Table 20-18.
		 * Wakeup-enabled events taking place when module is idle will generate an
		 * asynchronous wakeup.
		 */
		struct irqwakeen : Register<0x34,32>
		{
			struct mat_wup_ena : Bitfield<0,1>
			{
				enum {
					MAT_WUP_DISABLED = 0,
					MAT_WUP_ENABLED  = 1,
				};
			};
			struct ovf_wup_ena : Bitfield<1,1>
			{
				enum {
					OVF_WUP_DISABLED = 0,
					OVF_WUP_ENABLED  = 1,
				};
			};
			struct tcar_wup_ena : Bitfield<2,1>
			{
				enum {
					TCAR_WUP_DISABLED = 0,
					TCAR_WUP_ENABLED  = 1,
				};
			};
		};

		/**
		 * TCLR Register
		 * When the TCM field passed from (00) to any other combination then the
		 * TCAR_IT_FLAG and the edge detection logic are cleared. The ST bit of
		 * TCLR register may be updated from the OCP interface or reset due to
		 * one-shot overflow. The OCP interface update has the priority.
		 */
		struct tclr : Register<0x38,32>
		{
			struct st : Bitfield<0,1>
			{
				enum { STOP_TIMER  = 0, START_TIMER = 1, };
			};

			struct ar : Bitfield<1,1>
			{
				enum { ONE_SHOT   = 0, AUTORELOAD = 1, };
			};


			struct ptv : Bitfield<2,3> { };

			struct pre : Bitfield<5,1>
			{
				enum {
					TIMER_PIN_INPUT_CLOCKS_COUNTER   = 0,
					DIVIDED_INPUT_PIN_CLOCKS_COUNTER = 1,
				};
			};

			struct ce : Bitfield<6,1>
			{
				enum {
					COMPARE_MODE_DISABLED = 0,
					COMPARE_MODE_ENABLED  = 1,
				};
			};

			struct scpwm : Bitfield<7,1>
			{
				enum {
					CLEAR_PORTIMERPWM_OUT_POS_PULSE = 0,
					SET_PORTIMERPWM_OUT_NEG_PULSE = 1,
				};
			};

			struct tcm : Bitfield<8,2>
			{
				enum {
					NO_CAPTURE          = 0,
					CAPTURE_LOW_TO_HIGH = 1,
					CAPTURE_HIGH_TO_LOW = 2,
					CAPTURE_BOTH_EDGES  = 3,
				};
			};

			struct trg : Bitfield<10,2>
			{
				enum {
					NO_TRIGGER = 0,
					TRIGGER_ON_OVERFLOW = 1,
					TRIGGER_ON_OVERFLOW_AND_MATCH = 2,
				};
			};

			struct pt : Bitfield<12,1>
			{
				enum { PULSE = 0, TOGGLE = 1, };
			};

			struct capt_mode : Bitfield<13,1>
			{
				enum {
					SINGLE_CAPTURE = 0,
					CAPTURE_ON_SECOND_EVENT = 1,
				};
			};

			struct gpo_cfg : Bitfield<14,1>
			{
				enum {
					PORGPOCFG_DRIVES_0_TIMER_PIN_OUTPUT = 0,
					PORGPOCFG_DRIVES_1_TIMER_PIN_INPUT  = 1,
				};
			};

			/**
			* Register value that configures the timer for a one-shot run
			*
			*/
			//static access_t prepare_one_shot()
			static int prepare_one_shot()
			{
			return ar::ONE_SHOT;
			//		return En::bits(0) |
			//		       En_mod::bits(En_mod::RELOAD) |
			//		       Oci_en::bits(1) |
			//		       Rld::bits(Rld::RELOAD_FROM_LR) |
			//		       Prescaler::bits(Prescaler::DIVIDE_BY_1) |
			//		       Swr::bits(0) |
			//		       Iovw::bits(0) |
			//		       Dbg_en::bits(0) |
			//		       Wait_en::bits(0) |
			//		       Doz_en::bits(0) |
			//		       Stop_en::bits(0) |
			//		       Om::bits(Om::DISCONNECTED) |
			//		       Clk_src::bits(Clk_src::HIGH_FREQ_REF_CLK);
	    };
		};

		/* This register holds the value of the internal counter */
		struct tcrr : Register<0x3C,32> { };

		/* This register holds the timer's load value */
		struct tldr : Register<0x40,32> { };

		/* This register triggers a counter reload of timer by writing any value in it */
		struct ttgr : Register<0x44,32> { };

		/* This register contains the write posting bits for all writable functional registers */
		struct twps : Register<0x48,32>
		{
			struct w_pend_tclr : Bitfield<0,1>
			{
				enum { CLR_NPEND = 0, CLR_PEND  = 1, };
			};

			struct w_pend_tccr : Bitfield<1,1>
			{
				enum { CRR_NPEND = 0, CRR_PEND  = 1, };
			};

			struct w_pend_tldr : Bitfield<2,1>
			{
				enum { LDR_NPEND = 0, LDR_PEND  = 1, };
			};

			struct w_pend_ttgr : Bitfield<3,1>
			{
				enum { TGR_NPEND = 0, TGR_PEND  = 1, };
			};

			struct w_pend_tmar : Bitfield<4,1>
			{
				enum { MAR_NPEND = 0, MAR_PEND  = 1, };
			};
		};

		/* This register holds the match value to be compared with the counter's value */
		struct tmar : Register<0x4c,32> { };

		/* This register holds the value of the first counter register capture */
		struct tcar1 : Register<0x50C,32> { };

		/* Timer synchronous interface control register */
		struct tsicr : Register<0x54,32>
		{
			struct sft : Bitfield<1,1>
			{
				enum { SFT_0 = 0, SFT_1 = 1, };
			};

			struct posted : Bitfield<2,1>
			{
				enum {
					POSTED_MODE_INACTIVE = 0,
					POSTED_MODE_ACTIVE   = 1,
				};
			};
		};

		/* This register holds the value of the second counter register capture */
		struct tcar2 : Register<0x44,32> { };


	/**
	 * Status register
	 */
	//struct Sr : Register<0x4, 32>
	//{
		//	struct Ocif : Bitfield<0, 1> { }; /* IRQ status, write 1 clears */
	//};

		//struct Lr   : Register<0x8,  32> { }; /* load value register */
		//struct Cmpr : Register<0xc,  32> { }; /* compare value register */
		//struct Cnt  : Register<0x10, 32> { }; /* counter register */

	/**
	 * Disable timer and clear its interrupt output
	 */
	void _reset()
	{
		/* wait until ongoing reset operations are finished */
			while (read<tiocp_cfg::softreset>()) ;

		/* disable timer */
			write<tclr::st>(tclr::st::STOP_TIMER);

		/* clear interrupt */
//			write<Sr::Ocif>(1);
	}

	void _start_one_shot(unsigned const tics)
	{
		/* stop timer */
		_reset();

		/* configure timer for a one-shot */
			write<tclr>(tclr::prepare_one_shot());
//			write<Lr>(tics);
//			write<Cmpr>(0);

		/* start timer */
			write<tclr::st>(tclr::st::START_TIMER);
	}

	public:

		/**
		 * Constructor
		 */
		Dmtimer_base(addr_t base) : Mmio(base) { _reset(); }

		/**
		 * Start single timeout run
		 *
		 * \param tics  delay of timer interrupt
		 */
		void start_one_shot(unsigned const tics)
		{
			_start_one_shot(tics);
		}

		/**
		 * Stop the timer from a one-shot run
		 *
		 * \return  last native timer value of the one-shot run
		 */
		unsigned stop_one_shot(bool &wrap)
		{
			/* disable timer */
			write<tclr::st>(tclr::st::STOP_TIMER);
			return value(wrap);
		}

		/**
		 * Translate microseconds to a native timer value
		 */
		unsigned us_to_tics(unsigned const us)
		const
		{
			return (1ULL * us * TICS_PER_MS) / 1000;
		}

		/**
		 * Translate native timer value to microseconds
		 */
		unsigned tics_to_us(unsigned const tics)
		const
		{
			return (1ULL * tics * 1000) / TICS_PER_MS;
		}

		/**
		 * Return current native timer value
		 */
		unsigned value(bool &wrapped)
		const
		{
//			unsigned cnt = read<Cnt>();
//			wrapped = (bool)read<Sr::Ocif>();
//			return wrapped ? read<Cnt>() : cnt;
		}

		/**
		 * Current maximum programmed timeout value
		 */
		unsigned current_max_value() const { return 0; /*read<Lr>(); */ }
};

#endif /* _DMTIMER_H_ */
