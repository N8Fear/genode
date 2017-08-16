/*
 * \brief  Driver for the AM335x UART \author Hinnerk van Bruinehsen \date
 * 2017-08-10
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed under the
 * terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__DRIVERS__UART__AM335X_H_
#define _INCLUDE__DRIVERS__UART__AM335X_H_

/* Genode includes */
#include <util/mmio.h>

namespace Genode { class AM335x_uart; }


/** AM335x UART driver base
 */
class Genode::AM335x_uart: Mmio {
	protected:
		/** THR Register The transmit holding register (THR) is selected with the
		 * register bit setting of LCR[7]=0. The transmitter section consists of
		 * the transmit holding register and the transmit shift register. The
		 * transmit holding register is a 64-byte FIFO. The MPU writes data to
		 * the THR. The data is in the transmit shift register where it is
		 * shifted out serially on the TX output. If the FIFO is disabled,
		 * location zero of the FIFO is used to store the data.
		 */
		struct thr : Register<0x0,16>
		{
			struct _thr : Bitfield<0,8> { }; /* value 0x0 to 0xFF*/
		};

		/** RHR Register The Receiver holding register (RHR) is selected with the
		 * register bit setting of LCR[7] = 0. The receiver section consists of
		 * the receiver holding register and the receiber shifting register. The
		 * RHR is actually a 64-byte FIFO. The receiver shift register receives
		 * serial data from RX input. The data is converted to parallel data and
		 * moved to the RHR. If the FIFO is disabled, location zero of the FIFO
		 * is used to store the single data character. If an overflow occurs, the
		 * data in the RHR is not overwritten.
		 */
		struct rhr : Register<0x0,16>
		{
			struct _rhr : Bitfield<0,8> { }; /* value 0x0 to 0xFF*/
		};

		/**
		 * DLL Register
		 * The divisor latches low register (DLL) is selected with a register bit
		 * setting of LCR[7] not equal to 0xBF or LCR[7]=0xBF. The divisor
		 * latches low register (DLL) with the DLH register stores the 14-bit
		 * divisor for generation of the baud clock in the baud rate generator.
		 * DLH stores the most significant part od the divisor, DLL stores the
		 * least significant part of the divisor. DLL and DLH can be written to
		 * only before sleep mode is enabled (before IER[4] is set).
		 */
		struct dll : Register<0x0,16>
		{
			struct clock_lsb : Bitfield<0,8> { };
		};

		/**
		 * IER_IRDA
		 * The following interrupt enable register (IER) description is for IrDA
		 * mode. The IrDA IER is selected with a register bit setting of LCR[7] =
		 * 0. In IrDA mode, EFR[4] has no impact on the access to IER[7:4]. The
		 * IrDA interrupt enable register (IER) can be programmed to
		 * enable/disable any interrupt. There are 8 types of interrupt in these
		 * modes, recceived EOF, LSR, TX status, status FIFO interrupt, RX
		 * overrun, last byte in RX FIFO, THR interrupt, and RHR interrupt. Each
		 * interrupt can be enabled/disabled individually. The TXSTAT USIT
		 * interrupt reflects two possible conditions. The MDR2[0] bit should be
		 * read to determine the status in the event of this interrupt.
		 */
		struct ier_irda : Register<0x4,16>
		{
			struct rhrit : Bitfield<0,1>
			{
				enum { RHR_INT_OFF = 0, RHR_INT_ON  = 1, };
			};

			struct thrit : Bitfield<1,1>
			{
				enum { THR_INT_OFF = 0, THR_INT_ON  = 1, };
			};

			struct lastrxbyteit : Bitfield<2,1>
			{
				enum {
					LASTRXBYTE_INT_OFF = 0, /* Disables the last byte of frame in RX FIFO interrupt */
					LASTRXBYTE_INT_ON  = 1, /* Enables the last byte of frame in RX FIFO interrupt */
				};
			};

			struct rxoverrunit : Bitfield<3,1>
			{
				enum {
					RXOVERRUN_INT_OFF = 0, /* Disables the RX overrun interrupt */
					RXOVERRUN_INT_ON  = 1, /* Enables the RX overrun interrupt */
				};
			};

			struct stsfifotrigit : Bitfield<4,1>
			{
				enum {
					STSFIFOTRIG_INT_OFF = 0, /* Disables status FIFO trigger level interrupt */
					STSFIFOTRIG_INT_ON  = 1, /* Enables status FIFO trigger level interrupt */
				};
			};

			struct txstatusit : Bitfield<5,1>
			{
				enum {
					TXSTATUS_INT_OFF = 0, /* Disables the TX status interrupt */
					TXSTATUS_INT_ON  = 1, /* Enables the TX status interrupt */
				};
			};

			struct linestsit : Bitfield<6,1>
			{
				enum {
					LINESTS_INT_OFF = 0, /* Disables the receiver line status interrupt */
					LINESTS_INT_ON  = 1, /* Enables the receiver line status interrupt */
				};
			};

			struct eofit : Bitfield<7,1>
			{
				enum {
					EOF_INT_OFF = 0, /* Disables the received EOF interrupt */
					EOF_INT_ON  = 1, /* Enables the received EOF interrupt */
				};
			};
		};

		/**
		 * IER_CIR
		 * The following interrupt enable register (IER) description is for CIR
		 * mode. The CIR IER is selected with a register bit setting of LCR[7] =
		 * 0. In IrDA mode, EFR[4] has no impact on the access to IER[7:4]. The
		 * CIR interrupt enable register (IER) can be programmed to
		 * enable/disable any interrupt. There are 5 types of interrupt in these
		 * modes, TX status, RX overrun, RX stop interrupt, THR interrupt, and
		 * RHR interrupt.  Each interrupt can be enabled/disabled individually.
		 * In CIR mode, the TXSTATUSIT bit has only one meaning corresponding to
		 * the case MDR2[0] = 0. The RXSTOPIT interrupt is generated based on the
		 * value set in the BOF Length register (EBLR).
		 */
		struct ier_cir : Register<0x4,16>
		{
			struct rhrit : Bitfield<0,1>
			{
				enum { RHR_IT_OFF = 0, RHR_IT_ON  = 1, };
			};

			struct thrit : Bitfield<1,1>
			{
				enum { THR_IT_OFF = 0, THR_IT_ON  = 1, };
			};

			struct rxstopit : Bitfield<2,1>
			{
				enum {
					RX_STOP_IT_OFF = 0,
					RX_STOP_IT_ON  = 1,
				};
			};

			struct rxoverrunit : Bitfield<3,1>
			{
				enum {
					RX_OVERRUN_IT_OFF = 0,
					RX_OVERRUN_IT_ON  = 1,
				};
			};

			struct txstatusit : Bitfield<5,1>
			{
				enum {
					TX_STATUS_IT_OFF = 0,
					TX_STATUS_IT_ON  = 1,
				};
			};
		};

		/**
		 * IER_UART Register
		 * IER_UART is shown in Figure 19-39 and described in Table 19-35.  The
		 * following interrupt enable register (IER) description is for UART
		 * mode. The UART IER is selected with a register bit setting of LCR[7] =
		 * 0. In UART mode, IER[7:4] can only be written when EFR[4] = 1 The
		 * interrupt enable register (IER) can be programmed to enable/disable
		 * any interrupt. There are seven types of interrupt in this mode:
		 * receiver error, RHR interrupt, THR interrupt, XOFF received and CTS
		 * (active- low)/RTS (active-low) change of state from low to high. Each
		 * interrupt can be enabled/disabled individually. There is also a sleep
		 * mode enable bit. The UART interrupt enable register (IER) is shown in
		 * and described in .
		 */
		struct ier_uart : Register<0x4,16>
		{
			struct rhrit : Bitfield<0,1>
			{
				enum { RHR_IT_OFF = 0, RHR_IT_ON  = 1, };
			};

			struct thrit : Bitfield<1,1>
			{
				enum { THR_IT_OFF = 0, THR_IT_ON  = 1, };
			};

			struct linestsit : Bitfield<2,1>
			{
				enum {
					LINE_STS_IT_OFF = 0,
					LINE_STS_IT_ON  = 1,
				};
			};

			struct modemstsit : Bitfield<3,1>
			{
				enum {
					MODEM_STS_IT_OFF = 0,
					MODEM_STS_IT_ON  = 1,
				};
			};

			struct sleepmode : Bitfield<4,1>
			{
				enum {
					SLEEPMODE_OFF = 0,
					SLEEPMODE_ON  = 1,
				};
			};

			struct xoffit : Bitfield<5,1>
			{
				enum { XOFF_IT_OFF = 0, XOFF_IT_ON  = 1, };
			};

			struct rtsit : Bitfield<6,1>
			{
				enum { RTS_IT_OFF = 0, RTS_IT_ON  = 1, };
			};

			struct ctsit : Bitfield<7,1>
			{
				enum { CTS_IT_OFF = 0, CTS_IT_ON  = 1, };
			};

			/**
			 * DLH Register
			 * DLH is shown in Figure 19-40 and described in Table 19-36.  The
			 * divisor latches high register (DLH) is selected with a register bit
			 * setting of LCR[7] not equal to BFh or LCR[7] = BFh. The divisor
			 * latches high register (DLH) with the DLL register stores the 14-bit
			 * divisor for generation of the baud clock in the baud rate generator.
			 * DLH stores the most-significant part of the divisor, DLL stores the
			 * least-significant part of the divisor. DLL and DLH can be written to
			 * only before sleep mode is enabled (before IER[4] is set).
			 */
			struct dlh : Register<0x4,16>
			{
				struct clock_msb : Bitfield<0,6> { };
			};

			/**
			 * EFR Register
			 * 1.8 EFR Register (offset = 8h) [reset = 0h] EFR is shown in Figure
			 * 19-41 and described in Table 19-37.  The enhanced feature register
			 * (EFR) is selected with a register bit setting of LCR[7] = BFh. The
			 * enhanced feature register (EFR) enables or disables enhanced features.
			 * Most enhanced functions apply only to UART modes, but EFR[4] enables
			 * write accesses to FCR[5:4], the TX trigger level, which is also used
			 * in IrDA modes.
			 */
			struct efr : Register<0x8,16>
			{
				struct swflowcontrol : Bitfield<0,4> { };
				/**
				 * En-/Disables write to IER[7:4], FCR[5:4], and MCR[7:5].
				 */
				struct enhanceden    : Bitfield<4,1>
				{
					enum {
						ENH_FUNC_WRITE_DISABLE = 0,
						ENH_FUNC_WRITE_ENABLE  = 1,
					};
				};

				/**
				 * Received data is compared with XOFF2 data. If a match occurs, the
				 * received data is transferred to RX FIFO and the IIR[4] bit is set to
				 * 1 to indicate that a special character was detected.
				 */

				struct specialchardetect : Bitfield<5,1>
				{
					enum {
						DISABLE_SPECIAL_CHAR_DETECT = 0,
						ENABLE_SPECIAL_CHAR_DETECT  = 1,
					};
				};

				/**
				 * RTS (active-low) pin goes high (inactive) when the receiver FIFO
				 * HALT trigger level, TCR[3:0], is reached and goes low (active) when the
				 * receiver FIFO RESTORE transmission trigger level is reached.<Paste>
				 */

				struct autortsen : Bitfield<6,1>
				{
					enum {
						DISABLE_AUTO_RTS = 0,
						ENABLE_AUTO_RTS  = 1,

					};
				};

				struct autoctsen : Bitfield<7,1>
				{
					enum {
						DISABLE_AUTO_CTS = 0,
						ENABLE_AUTO_CTS  = 1,
					};
				};

			};
			struct iir_uart : Register<0x8,16>
			{
			};
};


//		/**
   //		 * Line control
   //		 */
//		struct Ulcon : Register<0x0, 32>
//		{
//			struct Word_length   : Bitfield<0, 2> { enum { _8_BIT = 3 }; };
//			struct Stop_bits     : Bitfield<2, 1> { enum { _1_BIT = 0 }; };
//			struct Parity_mode   : Bitfield<3, 3> { enum { NONE = 0 }; };
//			struct Infrared_mode : Bitfield<6, 1> { };
//
//			/**
   //			 * Initialization value
   //			 */
//			static access_t init_value()
//			{
//				return Word_length::bits(Word_length::_8_BIT) |
//				       Stop_bits::bits(Stop_bits::_1_BIT)     |
//				       Parity_mode::bits(Parity_mode::NONE)   |
//				       Infrared_mode::bits(0);
//			}
//		};
//
//		/**
   //		 * Control
   //		 */
//		struct Ucon : Register<0x4, 32>
//		{
//			struct Receive_mode    : Bitfield<0, 2> { enum { IRQ_POLL = 1 }; };
//			struct Transmit_mode   : Bitfield<2, 2> { enum { IRQ_POLL = 1 }; };
//			struct Send_brk_signal : Bitfield<4, 1> { };
//			struct Loop_back_mode  : Bitfield<5, 1> { };
//			struct Rx_err_irq      : Bitfield<6, 1> { };
//			struct Rx_timeout      : Bitfield<7, 1> { };
//			struct Rx_irq_type     : Bitfield<8, 1> { enum { LEVEL = 1 }; };
//			struct Tx_irq_type     : Bitfield<9, 1> { enum { LEVEL = 1 }; };
//			struct Rx_to_dma_susp  : Bitfield<10, 1> { };
//			struct Rx_to_empty_rx  : Bitfield<11, 1> { };
//			struct Rx_to_interval  : Bitfield<12, 4> { };
//			struct Rx_dma_bst_size : Bitfield<16, 3> { };
//			struct Tx_dma_bst_size : Bitfield<20, 3> { };
//
//			/**
   //			 * Initialization value
   //			 */
//			static access_t init_value()
//			{
//				return Receive_mode::bits(Receive_mode::IRQ_POLL)   |
//				       Transmit_mode::bits(Transmit_mode::IRQ_POLL) |
//				       Rx_timeout::bits(1);
//			}
//		};
//
//		/**
   //		 * FIFO control
   //		 */
//		struct Ufcon : Register<0x8, 32>
//		{
//			struct Fifo_en         : Bitfield<0, 1> { };
//			struct Rx_fifo_rst     : Bitfield<1, 1> { };
//			struct Tx_fifo_rst     : Bitfield<2, 1> { };
//		};
//
//		/**
   //		 * Modem control
   //		 */
//		struct Umcon : Register<0xc, 32>
//		{
//			struct Send_request  : Bitfield<0, 1> { };
//			struct Modem_irq     : Bitfield<3, 1> { };
//			struct Auto_flow_ctl : Bitfield<4, 1> { };
//			struct Rts_trigger   : Bitfield<5, 3> { };
//
//			/**
   //			 * Initialization value
   //			 */
//			static access_t init_value()
//			{
//				return Send_request::bits(0)  |
//				       Modem_irq::bits(0)     |
//				       Auto_flow_ctl::bits(0) |
//				       Rts_trigger::bits(0);
//			}
//		};
//
//		/**
   //		 * FIFO status
   //		 */
//		struct Ufstat : Register<0x18, 32>
//		{
//			struct Rx_fifo_count : Bitfield<0, 8>  { };
//			struct Rx_fifo_full  : Bitfield<8, 1>  { };
//			struct Tx_fifo_full  : Bitfield<24, 1> { };
//		};
//
//		/**
   //		 * Transmit buffer
   //		 */
//		struct Utxh : Register<0x20, 32>
//		{
//			struct Transmit_data : Bitfield<0, 8> { };
//		};
//
//		/**
   //		 * Receive buffer
   //		 */
//		struct Urxh : Register<0x24, 32>
//		{
//			struct Receive_data : Bitfield<0, 8> { };
//		};
//
//		/**
   //		 * Baud Rate Divisor
   //		 */
//		struct Ubrdiv : Register<0x28, 32>
//		{
//			struct Baud_rate_div : Bitfield<0, 16> { };
//		};
//
//		/**
   //		 * Fractional part of Baud Rate Divisor
   //		 */
//		struct Ufracval : Register<0x2c, 32>
//		{
//			struct Baud_rate_frac : Bitfield<0, 4> { };
//		};
//
//		/**
   //		 * Interrupt mask register
   //		 */
//		template <unsigned OFF>
//		struct Uintx : Register<OFF, 32>
//		{
//			struct Rxd   : Register<OFF, 32>::template Bitfield<0, 1> { };
//			struct Error : Register<OFF, 32>::template Bitfield<1, 1> { };
//			struct Txd   : Register<OFF, 32>::template Bitfield<2, 1> { };
//			struct Modem : Register<OFF, 32>::template Bitfield<3, 1> { };
//		};

//		using Uintp = Uintx<0x30>;
//		using Uintm = Uintx<0x38>;

	void _rx_enable()
	{
//			write<Ufcon::Fifo_en>(1);
//
//			/* mask all IRQs except receive IRQ */
//			write<Uintm>(Uintm::Error::bits(1) |
//			             Uintm::Txd::bits(1) |
//			             Uintm::Modem::bits(1));
//
//			/* clear pending IRQs */
//			write<Uintp>(Uintp::Rxd::bits(1) |
//			             Uintp::Error::bits(1) |
//			             Uintp::Txd::bits(1) |
//			             Uintp::Modem::bits(1));
	}

	bool _rx_avail()
	{
//			return (read<Ufstat>() & (Ufstat::Rx_fifo_count::bits(0xff)
//			        | Ufstat::Rx_fifo_full::bits(1))); }
	}

	/**
	 * Return character received via UART
	 */
	char _rx_char()
	{
//			read<Ufcon>();
//			char c = read<Urxh::Receive_data>();
//
//			/* clear pending RX IRQ */
//			write<Uintp>(Uintp::Rxd::bits(1));
//			return c;
	}

	public:

		/**
		 * Constructor
		 *
		 * \param  base       MMIO base address
		 * \param  clock      reference clock
		 * \param  baud_rate  targeted baud rate
		 */
		AM335x_uart(addr_t const base, unsigned const clock,
		            unsigned const baud_rate) : Mmio(base)
		{
//			/* RX and TX FIFO reset */
//			write<Ufcon::Rx_fifo_rst>(1);
//			write<Ufcon::Tx_fifo_rst>(1);
//			while (read<Ufcon::Rx_fifo_rst>() || read<Ufcon::Tx_fifo_rst>()) ;
//
//			/* init control registers */
//			write<Ulcon>(Ulcon::init_value());
//			write<Ucon>(Ucon::init_value());
//			write<Umcon>(Umcon::init_value());
//
//			/* apply baud rate */
//			float const div_val = ((float)clock / (baud_rate * 16)) - 1;
//			Ubrdiv::access_t const ubrdiv = div_val;
//			Ufracval::access_t const ufracval =
//				((float)div_val - ubrdiv) * 16;
//			write<Ubrdiv::Baud_rate_div>(ubrdiv);
//			write<Ufracval::Baud_rate_frac>(ufracval);
		}

		/**
		 * Print character 'c' through the UART
		 */
		void put_char(char const c)
		{
//			while (read<Ufstat::Tx_fifo_full>()) ;
//			write<Utxh::Transmit_data>(c);
		}
};

#endif /* _INCLUDE__DRIVERS__UART__AM335X_H_ */
