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

#ifndef _DMTIMER_1MS_H_
#define _DMTIMER_1MS_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode { class Dmtimer_base; }


class Genode::Dmtimer_1ms_base : public Mmio
{
	protected:

		enum { TICS_PER_MS = 66000 };

		/**
		 * TIDR: contains IP revision code
		 */
		struct tidr : Register<0x0,32>
		{
			struct tid_rev_min: Bitfield<0,4> { };
			struct tid_rev_maj: Bitfield<4,4> { };
		};

		/* Internal OCP clock gating strategy */
		struct tiocp_cfg : Register<0x10,32>
		{
			struct autoidle : Bitfield<0,1>
			{
				enum {
					DKFREE = 0, /* OCP clock is free-running */
					DKGATE = 1, /* Automatic OCP clock gating strategy is applied, based on the OCP interface activity */
				};
			};

			/* Software reset (automatically set by hardware) */
			struct softreset : Bitfield<1,1>
			{
				enum {
					NMODE   = 0, /* Normal mode */
					RSTMODE = 1, /* The module is reset */
				};
			};

			/* Wake-up feature global control */
			struct enawakeup : Bitfield<2,1>
			{
				enum {
					NOWAKE  = 0, /* No wakeup line assertion in idle mode */
					ENBWAKE = 1, /*Wakeup line assertion enabled in smart-idle mode */
				};
			};

			/* Powermanagement, req/ack control */
			struct idlemode : Bitfield<3,2>
			{
				enum {
					FIDLE = 0, /* Force-idle. An idle request is acknowledged unconditionally */
					NIDLE = 1, /* No-idle. An idle request is never acknowledged */
					SIDLE = 2, /* Smart-Idle. ACknowledgemenet to an idle re	quest is given based on the internal activity of the module */
					SIDLE_WAKE = 3, /* Smart-idle wackeup capable. Similar to DIDLE, but can generate Wakeup event in idle state */
				};
			};

			/* Sensitivity to emulation suspend events from Debug subsystem. */
			struct emulfree : Bitfield<5,1>
			{
				enum {
					TIMER_FROZEN = 0, /* Timer counter frozen during debug suspend */
					TIMER_FREE   = 1, /* Timer counter free-running, debug suspend is ignored */
				};
			};

			struct clockactivity : Bitfield<8,2> { };
		};

		/* Provides status information about the module, excluding interrupt status info */
		struct tistat : Register<0x14,32>
		{
			struct resetdone : Bitfield<0,1>
			{
				enum {
					RSTONGOING = 0, /* Internal module reset ongoing */
					RSTCOMP    = 1, /* Reset completed */
				};
			};
		};

		/* Timer Status Register, is used to determine which of the tier events requested an interrupt */
		struct tisr : Register<0x18,32>
		{
			/* The compare result of TCRR and TMAR */
			struct mat_it_flag : Bitfield<0,1>
			{
				enum {
					MAT_IT_FLAG_0 = 0, /* No compare interrupt request */
					MAT_IT_FLAG_1 = 1, /* Compare interrupt pendung */
				};
			};

			/* TCRR overflow */
			struct ovf_it_flag : Bitfield<1,1>
			{
				enum {
					OVF_IT_FLAG_0 = 0, /* No compare interrupt request */
					OVF_IT_FLAG_1 = 1, /* Compare interrupt pendung */
				};
			};

			/* Indicates when an external pulse transition of the correct polarity is detected on the external pin PIEVENTCAPT */
			struct tcar_it_flag : Bitfield<2,1>
			{
				enum {
					TCAR_IT_FLAG_0 = 0, /* No compare interrupt request */
					TCAR_IT_FLAG_1 = 1, /* Compare interrupt pendung */
				};
			};
		};

		/* This register controls (enables/disable) the interrupt events */
		struct tier : Register<0x1C,32>
		{
			/* Enable match interrupt */
			struct mat_it_ena : Bitfield<0,1>
			{
				enum {
					DSB_MATCH = 0, /* Disable match interrupt */
					ENB_MATCH = 1, /* Enable match interrupt */
				};
			};

			/* Enable overflow interrupt */
			struct ovf_it_ena : Bitfield<1,1>
			{
				enum {
					DSB_OVF = 0, /* Disable overflow interrupt */
					ENB_OVF = 1, /* Enable overflow interrupt */
				};
			};

			/* Enable capture interrupt */
			struct tcar_it_ena : Bitfield<2,1>
			{
				enum {
					DSB_CAPT = 0, /* Disable capture interrupt */
					ENB_CAPT = 1, /* Enable capture interrupt */
				};
			};
		};

		/* This register controls (enable/disable) the wakeup feature on specific interrupt events */
		struct twer : Register<0x20,32>
		{
			/* Control match wakeup */
			struct mat_wup_ena : Bitfield<0,1>
			{
				enum {
					DSBWUPMAT = 0, /* Disable match wakeup */
					ENBWUPMAT = 1, /* Enable matchh wakeup */
				};
			};

			/* Control overflow wakeup */
			struct ovf_wup_ena : Bitfield<1,1>
			{
				enum {
					DSBWUPOVF = 0, /* Disable overflow wakeup */
					ENBWUPOVF = 1, /* Enable overflow wakeup */
				};
			};

			/* Control capture wakeup */
			struct tcar_wup_ena : Bitfield<2,1>
			{
				enum {
					DSBWUPCAP = 0, /* Disable capture wakeup */
					ENBWUPCAP = 1, /* Enable capture wakeup */
				};
			};
		};

		/* This register controls optional features specific to the timer functionality */
		struct tclr : Register<0x24,32>
		{
			/* Start/Stop timer control */
			struct st : Bitfield<0,1>
			{
				enum {
					CNT_STOP  = 0, /* Stop the timer */
					CNT_START = 1, /* Start the timer */
				};
			};

			/* Auto-reload mode */
			struct ar : Bitfield<1,1>
			{
				enum {
					ONE_SHOT = 0, /* One shot mode overflow */
					AUTO_REL = 1, /* Auto-reload mode overflow */
				};
			};

			/* Trigger output mode */
			struct ptv : Bitfield<2,3>
			{
			};

			/* Prescaler enable */
			struct pre : Bitfield<5,1>
			{
				enum {
					NO_PRESCAL = 0, /* Prescaler disabled */
					PRESCAL_ON = 1, /* Prescaler enabled */
				};
			};

			/* Compare enable */
			struct ce : Bitfield<6,1>
			{
				enum {
					DSB_CMP = 0, /* Compare disabled */
					ENB_CMP = 1, /* Compare enabled */
				};
			};

			/* Pulse width modulation output pin default value */
			struct scpwm : Bitfield<7,1>
			{
				enum {
					DEF_LOW  = 0, /* Default value for PORPWM: 0 */
					DEF_HIGH = 1, /* Default value for PORPWM: 1 */
				};
			};

			/* Transition capture mode */
			struct tcm : Bitfield<8,2>
			{
				enum {
					NO_EDGE    = 0, /* No capture */
					RISE_EDGE  = 1, /* Capture on rising edge of PIEVENTCAPT */
					FALL_EDGE  = 2, /* Capture on falling edge of PIEVENTCAPT */
					BOTH_EDGES = 3, /* Capture on both edges of PIEVENTCAPT */
				};
			};

			/* Trigger output mode */
			struct trg : Bitfield<10,2>
			{
				enum {
					NO_TRG      = 0, /* No trigger */
					OVF_TRG     = 1, /* Overflow trigger */
					OVF_MAT_TRG = 2, /* Overflow and match trigger */
				};
			};

			/* Pulse or Toggle select bit */
			struct pt : Bitfield<12,1>
			{
				enum {
					PULSE  = 0, /* Pulse modulation */
					TOGGLE = 1, /* Toggle modulation */
				};
			};

			/* Capture mode select bit (first/second) */
			struct capt_mode : Bitfield<13,1>
			{
				enum {
					FIRST_CAPT = 0, /* Capture the first enabled capture event in TCAR1 */
					SEC_CAPT   = 1, /* Capture the second enabled capture event in TCAR2 */
				};
			};

			struct gpo_cfg : Bitfield<14,1> { };

		};

		/* This register holds the value of the internal counter */
		struct tcrr : Register<0x28,32> { };

		/* This register holds the timer's load value */
		struct tldr : Register<0x2C,32> { };

		/* This register triggers a counter reload of timer by writing any value in it */
		struct ttgr : Register<0x30,32> { };

		/* This register contains the write posting bits for all writable functional registers */
		struct twps : Register<0x34,32>
		{
			struct w_pend_tclr : Bitfield<0,1>
			{
				enum {
					CLR_NPEND = 0, /* No control register write pending */
					CLR_PEND  = 1, /* Control register write pendung */
				};
			};

			struct w_pend_tccr : Bitfield<1,1>
			{
				enum {
					CRR_NPEND = 0, /* No write pending */
					CRR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tldr : Bitfield<2,1>
			{
				enum {
					LDR_NPEND = 0, /* No write pending */
					LDR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_ttgr : Bitfield<3,1>
			{
				enum {
					TGR_NPEND = 0, /* No write pending */
					TGR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tmar : Bitfield<4,1>
			{
				enum {
					MAR_NPEND = 0, /* No write pending */
					MAR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tpir : Bitfield<5,1>
			{
				enum {
					PIR_NPEND = 0, /* No write pending */
					PIR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tnir : Bitfield<6,1>
			{
				enum {
					NIR_NPEND = 0, /* No write pending */
					NIR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tcvr : Bitfield<7,1>
			{
				enum {
					CVR_NPEND = 0, /* No write pending */
					CVR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_tocr : Bitfield<8,1>
			{
				enum {
					OCR_NPEND = 0, /* No write pending */
					OCR_PEND  = 1, /* Write pending */
				};
			};

			struct w_pend_towr : Bitfield<9,1>
			{
				enum {
					OWR_NPEND = 0, /* No write pending */
					OWR_PEND  = 1, /* Write pending */
				};
			};
		};

		/* This register holds the match value to be compared with the counter's value */
		struct tmar : Register<0x38,32> { };

		/* This register holds the value of the first counter register capture */
		struct tcar1 : Register<0x3C,32> { };

		/* Timer synchronous interface control register */
		struct tsicr : Register<0x40,32>
		{
			struct sft : Bitfield<1,1>
			{
				enum {
					SFT_0 = 0, /* Software reset is disabled */
					SFT_1 = 1, /* Software reset is enabled */
				};
			};

			/*
			 * 0x0 = posted mode inactive, 0x1 posted mode active (clocks radio needs to fit  freq (timer) less than freq
			 * (OCP)/4 frequency requirement.
			 */
			struct posted : Bitfield<2,1> { };
		};

		/* This register holds the value of the second counter register capture */
		struct tcar2 : Register<0x44,32> { };

		/*
		 * This register is used for 1ms tick generation. The TPIR register holds
		 * the value of the positive increment. The value of this register is added
		 * with the value of the TCVR to define wether next value loaded in TCRR will
		 * be the sub-period value or the over-period value.
		 */
		struct tpir : Register<0x48,32> { };

		/*
		 * This register is used for 1ms tick generation. The TNIR register holds
		 * the value of the negative increment. The value of this register is added
		 * with the value of the TCVR to define wether next value loaded in TCRR will
		 * be the sub-period value or the over-period value.
		 */
		struct tnir : Register<0x4C,32> { };

		/*
		 * This register is used for 1ms tick generation. The TCVR register defines
		 * wether next value loaded in TCRR will be the sub-period value or the
		 * over-period value.
		 */
		struct tcvr : Register<0x50,32> { };

		/* This register is used to mask the tick interrupt for a selected number of ticks. */
		struct tocr : Register<0x54,32>
		{
			struct ovf_counter_value : Bitfield<0,24> { };
		};

		/* This register holds the number of masked overflow interrupts */
		struct towr : Register<0x58,32>
			{
				struct ovf_wrapping_value : Bitfield<0,24> { };
			};


		//	/**
		//	 * Register value that configures the timer for a one-shot run
		//	 */
		//	static access_t prepare_one_shot()
		//	{
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
		//	}
		//};

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
//			while (read<Cr::Swr>()) ;

			/* disable timer */
//			write<Cr::En>(0);

			/* clear interrupt */
//			write<Sr::Ocif>(1);
		}

		void _start_one_shot(unsigned const tics)
		{
			/* stop timer */
			_reset();

			/* configure timer for a one-shot */
//			write<Cr>(Cr::prepare_one_shot());
//			write<Lr>(tics);
//			write<Cmpr>(0);

			/* start timer */
//			write<Cr::En>(1);
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
//			write<Cr::En>(0);
			return value(wrap);
		}

		/**
		 * Translate microseconds to a native timer value
		 */
		unsigned us_to_tics(unsigned const us) const
		{
			return (1ULL * us * TICS_PER_MS) / 1000;
		}

		/**
		 * Translate native timer value to microseconds
		 */
		unsigned tics_to_us(unsigned const tics) const
		{
			return (1ULL * tics * 1000) / TICS_PER_MS;
		}

		/**
		 * Return current native timer value
		 */
		unsigned value(bool &wrapped) const
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

#endif /* _DMTIMER_1MS_H_ */
