/*
 * \brief  Driver for the AM335x UART
 * \author Hinnerk van Bruinehsen
 * \date   2017-08-10
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
#include <drivers/defs/am335x.h>

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
			struct it_pending : Bitfield<0,1>
			{
				enum {
					INT_PENDING = 0,
					INT_NOT_PENDING = 1,
				};
			};

			struct it_type : Bitfield<1,5>
			{
				enum {
					MODEM_INT = 0x0,
					THR_INT   = 0x1,
					RHR_INT   = 0x2,
					RECEIV_STATUS_LINE_ERROR = 0x3,
					RX_TIMEOUT = 0x6,
					XOFF_SPEC_CHAR = 0x8,
					CTS_RTS_DSR_CHANGE_STATE = 0x10,
				};
			};

			struct fcr_mirror : Bitfield<6,2> { };
		};

		/**
		 * IIR_CIR is shown in Figure 19-43 and described in Table 19-39.  The
		 * following interrupt identification register (IIR) description is for
		 * CIR mode. The CIR IIR is selected with a register bit setting of
		 * LCR[7] = 0 or LCR[7] not equal to BFh. The CIR interrupt
		 * identification register (IIR) is a read-only register that provides
		 * the source of the interrupt. An interrupt source can be flagged only
		 * if enabled in the IER register.
		 */
		struct iir_cir : Register<0x8,16>
		{
			struct rhrit_ : Bitfield<0,1>
			{
				enum {RHR_INT_INACTIVE = 0, RHR_INT_ACTIVE = 1,};
			};

			struct thrit_ : Bitfield<1,1>
			{
				enum {THR_INT_INACTIVE = 0, THR_INT_ACTIVE = 1,};
			};

			struct rxstopit : Bitfield<2,1>
			{
				enum {RXSTOP_INT_INACTIVE = 0, RXSTOP_INT_ACTIVE = 1,};
			};

			struct rxoeit : Bitfield<3,1>
			{
				enum {RXOVERRUN_INT_INACTIVE = 0, RXOVERRUN_INT_ACTIVE = 1,};
			};

			struct txstatusit : Bitfield<5,1>
			{
				enum {TXSTATUS_INT_INACTIVE = 0, TXSTATUS_INT_ACTIVE = 1,};
			};
		};

		/**
		 * FCR Register
		 * The FIFO control register (FCR) is selected with a register bit
		 * setting of LCR[7] = 0 or LCR[7] not equal to BFh. FCR[5:4] can only be
		 * written when EFR[4] = 1.
		 */
		struct fcr : Register<0x8,16>
		{
			struct fifo_en : Bitfield<0,1>
			{
				enum { DISABLE_RX_TX_FIFOS = 0, ENABLE_RX_TX_FIFOS = 1,};
			};

			struct rx_fifo_clear : Bitfield<1,1>
			{
				enum { NO_CHANGE = 0, CLEAR_RX_FIFO = 1,};
			};

			struct tx_fifo_clear : Bitfield<2,1>
			{
				enum { NO_CHANGE = 0, CLEAR_TX_FIFO = 1,};
			};

			struct dma_mode : Bitfield<3,1>
			{
				/* DMA_MODE0: No DMA
				 * DMA_MODE1: UART_NDMA_REQ[0] in TX, UART_NDMA_REQ[1] in RX
				 */
				enum { DMA_MODE0 = 0, DMA_MODE1 = 1,};
			};

			struct tx_fifo_trig : Bitfield<4,2>
			{
				enum {
					CHAR_COUNT_8  = 0,
					CHAR_COUNT_16 = 1,
					CHAR_COUNT_32 = 2,
					CHAR_COUNT_56 = 3,
				};
			};

			struct rx_fifo_trig : Bitfield<6,2>
			{
				enum {
					CHAR_COUNT_8  = 0,
					CHAR_COUNT_16 = 1,
					CHAR_COUNT_56 = 2,
					CHAR_COUNT_60 = 3,
				};
			};
		};

		struct iir_irda : Register<0x8,16>
		{
			struct rhr_it : Bitfield<0,1>
			{
				enum {RHR_INT_INACTIVE = 0, RHR_INT_ACTIVE = 1,};
			};

			struct thr_it : Bitfield<1,1>
			{
				enum {THR_INT_INACTIVE = 0, THR_INT_ACTIVE = 1,};
			};

			struct rx_fifo_last_byte_it : Bitfield<2,1>
			{
				enum { LAST_BYTE_RX_FIFO_INACTIVE = 0, LAST_BYTE_RX_FIFO_ACTIVE = 1,};
			};

			struct rx_oe_it : Bitfield<3,1>
			{
				enum { RX_OVERRUN_IT_INACTIVe = 0, RX_OVERRUN_IT_ACTIVE = 1,};
			};

			struct sts_fifo_it : Bitfield<4,1>
			{
				enum { STS_FIFO_TRIG_INT_INACTIVE = 0, STS_FIFO_TRIG_INT_ACTIVE = 1,};
			};

			struct tx_status_it : Bitfield<5,1>
			{
				enum { TX_STATUS_IT_INACTIVE = 0, TX_STATUS_IT_ACTIVE = 1,};
			};

			struct line_sts_it : Bitfield<6,1>
			{
				enum { LINE_STS_IT_INACTIVE = 0, LINE_STS_IT_ACTIVE = 1,};
			};

			struct eof_it : Bitfield<7,1>
			{
				enum { REC_EOF_INT_INACTIVE = 0, REC_EOF_INT_ACTIVE = 1,};
			};

		};

		/**
		 * LCR Register
		 * The line control register (LCR) is selected with a bit register
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. As
		 * soon as LCR[6] is set to 1, the TX line is forced to 0 and remains in
		 * this state as long as LCR[6] = 1.
		 */

		struct lcr : Register<0xC,16>
		{
			struct char_length : Bitfield<0,2>
			{
				enum {
					LEN_5_BITS = 0,
					LEN_6_BITS = 1,
					LEN_7_BITS = 2,
					LEN_8_BITS = 3,
				};
			};

			struct nb_stop : Bitfield<2,1>
			{
				enum {
					STOPBIT_1 = 0,
					STOPBIT_1_5 = 1,
					STOPBIT_2 =1,
				};
			};

			struct parity_en : Bitfield<3,1>
			{
				enum { NO_PARITY = 0, PARITY = 1,};
			};

			struct parity_type : Bitfield<4,1>
			{
				enum { ODD_PARITY = 0, EVEN_PARITY = 1,};
			};

			/* forced Parity format - TODO: implement if needed */
			struct parity_type2 : Bitfield<5,1> { };

			struct break_en : Bitfield<6,2>
			{
				enum { NORMAL_OPERATION = 0, ALERT_COM_TERMINAL = 1,};
			};

			struct div_en : Bitfield<7,1>
			{
				enum { NORMAL_OPERATION = 0, DIVISOR_LATCH_ENABLE = 1,};
			};
		};

		/**
		 * MCR Register
		 * The modem control register (MCR) is selected with a register bit
		 * setting of LCR[7] = 0 or LCR[7] not equal to BFh. MCR[7:5] can only be
		 * written when EFR[4] = 1. Bits 3-0 control the interface with the
		 * modem, data set, or peripheral device that is emulating the modem.
		 */
		struct mcr : Register<0x10,16>
		{
			struct dtr : Bitfield<0,0>
			{
				enum { FORCE_DTR_INACTIVE = 0, FORCE_DTR_ACTIVE =1,};
			};

			struct rts : Bitfield<1,1>
			{
				enum { FORCE_RTS_INACTIVE = 0, FORCE_RTS_ACTIVE = 1,};
			};

			struct ristsch : Bitfield<2,1>
			{
				enum { FORCE_RI_INACTIVE_IN_LOOPBACK = 0, FORCE_RI_ACTIVE_IN_LOOPBACK = 1,};
			};

			struct cdstsch : Bitfield<3,1>
			{
				enum { FORCE_DCD_INACTIVE_IN_LOOPBACK = 0 , FORCE_DCD_ACTIVE_IN_LOOPBACK = 1,};
			};

			struct loopbacken : Bitfield<4,1>
			{
				enum { NORMAL_OPERATION = 0, LOCAL_LOOPBACK =1,};
			};

			struct xonen : Bitfield<5,1>
			{
				enum { XON_DISABLE = 0, XON_ENABLE = 1,};
			};

			struct tcrtlr : Bitfield<6,1>
			{
				enum { NO_ACTION = 0, ENABLE_TCR_TLR_ACCESS = 1,};
			};
		};

		/**
		 * XON1_ADDR1 Register
		 * XON1_ADDR1 is shown in Figure 19-48 and described in Table 19-44.  The
		 * XON1/ADDR1 registers are selected with a register bit setting of
		 * LCR[7] = BFh. In UART mode, XON1 character; in IrDA mode, ADDR1
		 * address 1.
		 */

		struct xon1_addr1 : Register<0x10,16>
		{
			struct xonword1 : Bitfield<0,8> { };
		};

		/**
		 * XON2_ADDR2 Register
		 * XON2_ADDR2 is shown in Figure 19-49 and described in Table 19-45.  The
		 * XON2/ADDR2 registers are selected with a register bit setting of
		 * LCR[7] = BFh. In UART mode, XON2 character; in IrDA mode, ADDR2
		 * address 2.
		 */

		struct xon2_addr2 : Register<0x14,16>
		{
			struct xonword2 : Bitfield<0,8> { };
		};

		/**
		 * LSR_CIR is shown in Figure 19-50 and described in Table 19-46.  The
		 * following line status register (LSR) description is for CIR mode. The
		 * CIR LSR is selected with a register bit setting of LCR[7] = 0 or
		 * LCR[7] not equal to BFh.
		 */

		struct lsr_cir : Register<0x14,16>
		{
			struct rxfifoe : Bitfield<0,1>
			{
				enum { DATA_IN_RX_FIFO = 0, NO_DATA_IN_RX_FIFO = 1,};
			};

			struct rxstop : Bitfield<5,1>
			{
				enum { RECEPTION_GOING = 0, RECEPTION_COMPLETE = 1,};
			};

			struct thrempty : Bitfield<7,1>
			{
				enum { TRANSMIT_HOLD_NOT_EMPTY = 0, TRANSMIT_HOLD_EMPTY = 1,};
			};
		};

		/**
		 * LSR_IRDA Register
		 * LSR_IRDA is shown in Figure 19-51 and described in Table 19-47.  The
		 * following line status register (LSR) description is for IrDA mode. The
		 * IrDA LSR is selected with a register bit setting of LCR[7] = 0 or
		 * LCR[7] not equal to BFh. When the IrDA line status register (LSR) is
		 * read, LSR[4:2] reflect the error bits (FL, CRC, ABORT) of the frame at
		 * the top of the status FIFO (next frame status to be read).
		 */

		struct lsr_irda : Register<0x14,16>
		{
			struct rx_fifo_e : Bitfield<0,1>
			{
				enum { RX_FIFO_NOT_EMPTY = 0, RX_FIFO_EMPTY = 1,};
			};
			struct sts_fifo_e : Bitfield<1,1>
			{
				enum { STS_FIFO_NOT_EMPTY = 0, STS_FIFO_EMPTY = 1,};
			};

			struct crc : Bitfield<2,1>
			{
				enum { CRC_NO_ERROR = 0, CRC_ERROR = 1,};
			};

			struct abort : Bitfield<3,1>
			{
				enum { NO_ABORT_PATTERN_ERROR = 0, ABORT_PATTERN_RECEIVED = 1,};
			};

			struct frame_too_long : Bitfield<4,1>
			{
				enum { NO_FRAME_TOO_LONG_ERROR = 0, FRAME_TOO_LONG = 1,};
			};

			struct rx_last_byte : Bitfield<5,1>
			{
				enum { RX_FIFO_NOT_LAST_FRAME = 0, RX_FIFO_LAST_FRAME = 1,};
			};

			struct sts_fifo_full : Bitfield<6,1>
			{
				enum { STATUS_FIFO_NOT_FULL = 0, STATUS_FIFO_FULL = 1,};
			};

			struct thr_empty : Bitfield<7,1>
			{
				enum { THR_NOT_EMPTY = 0, THR_EMPTY = 1,};
			};
		};

		/**
		 * LSR_UART Register
		 * 19.5.1.19 LSR_UART Register (offset = 14h) [reset = 60h] LSR_UART is
		 * shown in Figure 19-52 and described in Table 19-48.  The following
		 * line status register (LSR) description is for UART mode. The UART LSR
		 * is selected with a register bit setting of LCR[7] = 0 or LCR[7] not
		 * equal to BFh. When the UART line status register (LSR) is read,
		 * LSR[4:2] reflect the error bits (BI, FE, PE) of the character at the
		 * top of the RX FIFO (next character to be read). Therefore, reading the
		 * LSR and then reading the RHR identifies errors in a character.
		 * Reading RHR updates BI, FE, and PE. LSR [7] is set when there is an
		 * error anywhere in the RX FIFO and is cleared only when there are no
		 * more errors remaining in the RX FIFO. Reading the LSR does not cause
		 * an increment of the RX FIFO read pointer. The RX FIFO read pointer is
		 * incremented by reading the RHR. Reading LSR clears OE if set.
		 */

		struct lsr_uart : Register<0x14,16>
		{
			struct rxfifoe : Bitfield<0,1>
			{
				enum { RX_FIFO_EMPTY = 0, RX_FIFO_NOT_EMPTY = 1,};
			};

			struct rxoe : Bitfield<1,1>
			{
				enum { NO_OVERRUN_ERROR = 0, OVERRRUN_ERROR = 1,};
			};

			struct rxpe : Bitfield<2,1>
			{
				enum { PARITY_NO_ERROR = 0, PARITY_ERROR = 1,};
			};

			struct rxfe : Bitfield<3,1>
			{
				enum { NO_FRAMING_ERROR = 0, FRAMING_ERROR = 1,};
			};

			struct rxbi : Bitfield<4,1>
			{
				enum { NO_BREAK_CONDITION = 0, BREAK_DETECTED = 1,};
			};

			struct txfifoe : Bitfield<5,1>
			{
				enum { TRANSMIT_HOLD_NOT_EMPTY = 0, TRANSMIT_HOLD_EMPTY = 1,};
			};

			struct txsre : Bitfield<6,1>
			{
				enum { TRANSMIT_HOLD_AND_SHIFT_REGS_NOT_EMPTY = 0, TRANSMIT_HOLD_AND_SHIFT_REGS_EMPTY = 1,};
			};

			struct rxfifosts : Bitfield<7,1>
			{
				enum { NORMAL_OPERATION = 0, ERROR_OCCURED = 1,};
			};
		};

		/**
		 * TCR Register
		 * The transmission control register (TCR) is selected with a register
		 * bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh.
		 * The TCR is accessible only when EFR[4] = 1 and MCR[6] = 1. The
		 * transmission control register (TCR) stores the receive FIFO threshold
		 * levels to start/stop transmission during hardware flow control.
		 * Trigger levels from 0-60 bytes are available with a granularity of 4.
		 * Trigger level = 4 x [4-bit register value]. You must ensure that
		 * TCR[3:0] > TCR[7:4], whenever auto-RTS or software flow control is
		 * enabled to avoid a misoperation of the device. In FIFO interrupt mode
		 * with flow control, you have to also ensure that the trigger level to
		 * HALT transmission is greater or equal to receive FIFO trigger level
		 * (either TLR[7:4] or FCR[7:6]); otherwise, FIFO operation stalls. In
		 * FIFO DMA mode with flow control, this concept does not exist because
		 * the DMA request is sent each time a byte is received.
		 */

		struct tcr : Register<0x18,16>
		{
			struct rxfifotrighalt  : Bitfield<0,4> { };
			struct rxfifotrigstart : Bitfield<4,4> { };
		};

		/**
		 * MSR Register
		 * The modem status register (MSR) is selected with a register bit
		 * setting of LCR[7] = 0 or LCR[7] not equal to BFh. The modem status
		 * register (MSR) provides information about the current state of the
		 * control lines from the modem, data set, or peripheral device to the
		 * Local Host. It also indicates when a control input from the modem
		 * changes state.
		 */

		struct msr : Register<0x18,16>
		{
			struct cts_sts : Bitfield<0,1>
			{
				enum { NO_CHANGE = 0, CTS_CHANGED_STATE = 1,};
			};

			struct dsr_sts : Bitfield<1,1>
			{
				enum { NO_CHANGE = 0, DSR_CHANGED_STATE = 1,};
			};

			struct ri_sts : Bitfield<2,1>
			{
				enum { NO_CHANGE = 0, RI_CHANGED_STATE = 1,};
			};

			struct dcd_sts : Bitfield<3,1>
			{
				enum { NO_CHANGE = 0, DCD_CHANGED = 1,};
			};

			struct nctr_sts : Bitfield<4,1> { };
			struct ndsr_sts : Bitfield<5,1> { };
			struct nri_sts  : Bitfield<6,1> { };
			struct ncd_sts  : Bitfield<7,1> { };
		};

		/**
		 * XOFF1 Register
		 * The XOFF1 register is selected with a register bit setting of LCR[7] =
		 * BFh. In UART mode, XOFF1 character.
		 */

		struct xoff1 : Register<0x18,16>
		{
			struct xoffword1 : Bitfield<0,8> { };
		};

		/**
		 * SPR Register
		 * The scratchpad register (SPR) is selected with a register bit setting
		 * of LCR[7] = 0 or LCR[7] not equal to BFh. The scratchpad register
		 * (SPR) is a read/write register that does not control the module. It is
		 * a scratchpad register used to hold temporary data.
		 */

		struct spr : Register<0x1C,16>
		{
			struct spr_word : Bitfield<0,8> { };
		};

		struct tlr : Register<0x1C,16>
		{
			/* TODO: check if more logic is required */
			struct tx_fifo_trig_dma : Bitfield<0,4> { };
			struct rx_fifo_trig_dma : Bitfield<4,4> { };
		};

		/**
		 * XOFF2 Register
		 * The XOFF2 register is selected with a register bit setting of LCR[7] =
		 * BFh. In UART mode, XOFF2 character.
		 */

		struct xoff2 : Register<0x1C,16>
		{
			struct xoffword2 : Bitfield<0,8> { };
		};

		/**
		 * MDR1 Register
		 * The mode definition register 1 (MDR1) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * mode of operation is programmed by writing to MDR1[2:0]; therefore,
		 * the mode definition register 1 (MDR1) must be programmed on startup
		 * after configuration of the configuration registers (DLL, DLH, and
		 * LCR). The value of MDR1[2:0] must not be changed again during normal
		 * operation. If the module is disabled by setting the MODESELECT field
		 * to 7h, interrupt requests can still be generated unless disabled
		 * through the interrupt enable register (IER). In this case, UART mode
		 * interrupts are visible. Reading the interrupt identification register
		 * (IIR) shows the UART mode
		 */

		struct mdr1 : Register<0x20,16>
		{
			struct modeselect : Bitfield<0,3>
			{
				enum {
					UART_16X_MODE     = 0,
					SIR_MODE          = 1,
					UART_16X_AUTOBAUD = 2,
					UART_13X_MODE     = 3,
					MIR_MODE          = 4,
					FIR_MODE          = 5,
					CIR_MODE          = 6,
					DISABLE           = 7,
				};
			};

			struct irsleep : Bitfield<3,1>
			{
				enum { IRDA_CIR_SLEEP_DISABLED = 0, IRDA_CIR_SLEEP_ENABLED = 1,};
			};

			/* TODO: IrDA logic, if required */
			struct settxir : Bitfield<4,1> { };

			struct sct : Bitfield<5,1>
			{
				enum { START_IR_ON_THR = 0, START_IR_ON_ACREG2 = 1,};
			};

			struct sipmode : Bitfield<6,1>
			{
				enum { MANUAL_SIP_MODE = 0, AUTO_SIP_MODE = 1,};
			};

			struct frameendmode : Bitfield<7,1>
			{
				enum { FRAME_LENGTH_METHOD = 0, SET_EOT_BIT_METHOD = 1,};
			};
		};

		/**
		 * MDR2 Register
		 * The mode definition register 2 (MDR2) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * MDR2[0] bit describes the status of the TX status interrupt in IIR[5].
		 * The IRTXUNDERRUN bit must be read after a TX status interrupt occurs.
		 * The MDR2[2:1] bits set the trigger level for the frame status FIFO (8
		 * entries) and must be programmed before the mode is programmed in
		 * MDR1[2:0]. The MDR2[6] bit gives the flexibility to invert the RX pin
		 * inside the UART module to ensure that the protocol at the input of the
		 * transceiver module has the same polarity at module level. By default,
		 * the RX pin is inverted because most of transceiver invert the IR
		 * receive pin.
		 */

		struct mdr2 : Register<0x24,16>
		{
			struct irtxunderrun : Bitfield<0,0>
			{
				enum { NO_ERROR = 0, TX_UNDERRUN = 1,};
			};

			struct stsfifotrig : Bitfield<1,2>
			{
				enum {
					ENTRYNUMBER_1 = 0,
					ENTRYNUMBER_4 = 1,
					ENTRYNUMBER_7 = 2,
					ENTRYNUMBER_8 = 3,
				};
			};

			struct uartpulse : Bitfield<3,1>
			{
				enum { NORMAL_UART = 0, UART_PULSE_SHAPING = 1,};
			};

			struct cirpulsemode : Bitfield<5,2>
			{
				enum {
					PULSE_WIDTH_3_PER_12 = 0,
					PULSE_WIDTH_4_PER_12 = 1,
					PULSE_WIDTH_5_PER_12 = 2,
					PULSE_WIDTH_6_PER_12 = 3,
				};
			};

			struct irrxinvert : Bitfield<6,1>
			{
				enum { DO_INVERSION = 0, NO_INVERSION = 1,};
			};

			struct settxiralt : Bitfield<7,1>
			{
				enum { NORMAL_MODE = 0, ALTERNATE_MODE_FOR_SETTXIR = 1,};
			};
		};

		/**
		 * TXFLL Register
		 * The transmit frame length low register (TXFLL) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. The transmit frame length low register (TXFLL) and the TXFLH
		 * register hold the 13-bit transmit frame length (expressed in bytes).
		 * TXFLL holds the LSBs and TXFLH holds the MSBs. The frame length value
		 * is used if the frame length method of frame closing is used.
		 */

		struct txfll : Register<0x28,16>
		{
			struct tx_frame_length : Bitfield<0,8> { };
		};

		/**
		 * SFLSR Register
		 * The status FIFO line status register (SFLSR) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. Reading the status FIFO line status register (SFLSR)
		 * effectively reads frame status information from the status FIFO. This
		 * register does not physically exist. Reading this register increments
		 * the status FIFO read pointer (SFREGL and SFREGH must be read first).
		 * Top of RX FIFO = Next frame to be read from RX FIFO.
		 */

		struct sflsr : Register<0x28,16>
		{
			struct crc_error : Bitfield<1,1>
			{
				enum { NO_ERROR = 0, CRC_ERROR = 1,};
			};

			struct abort_detect : Bitfield<2,1>
			{
				enum { NO_ERROR = 0, ABORT_PATTERN_RECEIVED = 1,};
			};

			struct frame_too_long : Bitfield<3,1>
			{
				enum { NO_ERROR = 0, FRAME_TOO_LONG = 1,};
			};

			struct oe_error : Bitfield<4,1>
			{
				enum { NO_ERROR = 0, OVERRRUN_ERROR = 1,};
			};
		};

		/**
		 * RESUME Register
		 * The RESUME register is selected with a register bit setting of LCR[7]
		 * = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The RESUME register is
		 * used to clear internal flags, which halt transmission/reception when
		 * an underrun/overrun error occurs. Reading this register resumes the
		 * halted operation. This register does not physically exist and always
		 * reads as 00.
		 */

		struct resume : Register<0x2C,16>
		{
			struct resume_ : Bitfield<0,8> { };
		};

		/**
		 * TXFLH Register
		 * The transmit frame length high register (TXFLH) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. The transmit frame length high register (TXFLH) and the TXFLL
		 * register hold the 13-bit transmit frame length (expressed in bytes).
		 * TXFLL holds the LSBs and TXFLH holds the MSBs. The frame length value
		 * is used if the frame length method of frame closing is used.
		 */

		struct txflh : Register<0x2C,16>
		{
			struct txflh_ : Bitfield<0,5> { };
		};

		/**
		 * RXFLL Register
		 * The received frame length low register (RXFLL) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. The received frame length low register (RXFLL) and the RXFLH
		 * register hold the 12-bit receive maximum frame length. RXFLL holds the
		 * LSBs and RXFLH holds the MSBs. If the intended maximum receive frame
		 * length is n bytes, program RXFLL and RXFLH to be n + 3 in SIR or MIR
		 * modes and n + 6 in FIR mode (+3 and +6 are the result of frame format
		 * with CRC and stop flag).
		 */

		struct rxfll : Register<0x30,16>
		{
			struct rxfll_ : Bitfield<0,8> { };
		};

		/**
		 * SFREGL Register
		 * The status FIFO register low (SFREGL) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * frame lengths of received frames are written into the status FIFO.
		 * This information can be read by reading the status FIFO register low
		 * (SFREGL) and the status FIFO register high (SFREGH). These registers
		 * do not physically exist. The LSBs are read from SFREGL and the MSBs
		 * are read from SFREGH. Reading these registers does not alter the
		 * status FIFO read pointer.  These registers must be read before the
		 * pointer is incremented by reading the SFLSR.
		 */

		struct sfregl : Register<0x30,16>
		{
			struct sfregl_ : Bitfield<0,8> { };
		};

		/**
		 * SFREGH Register
		 * The status FIFO register high (SFREGH) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * frame lengths of received frames are written into the status FIFO.
		 * This information can be read by reading the status FIFO register low
		 * (SFREGL) and the status FIFO register high (SFREGH). These registers
		 * do not physically exist. The LSBs are read from SFREGL and the MSBs
		 * are read from SFREGH. Reading these registers does not alter the
		 * status FIFO read pointer.
		 */

		struct sfregh : Register<0x34,16>
		{
			struct sfregh_ : Bitfield<0,8> { };
		};

		/**
		 * RXFLH Register
		 * The received frame length high register (RXFLH) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. The received frame length high register (RXFLH) and the RXFLL
		 * register hold the 12-bit receive maximum frame length. RXFLL holds the
		 * LSBs and RXFLH holds the MSBs. If the intended maximum receive frame
		 * length is n bytes, program RXFLL and RXFLH to be n + 3 in SIR or MIR
		 * modes and n + 6 in FIR mode (+3 and +6 are the result of frame format
		 * with CRC and stop flag; two bytes are associated with the FIR stop
		 * flag).
		 */

		struct rxflh : Register<0x34,16>
		{
			struct rxflh_ : Bitfield<0,8> { };
		};

		/**
		 * BLR Register
		 * he BOF control register (BLR) is selected with a register bit setting
		 * of LCR[7] = 0. The BLR[6] bit is used to select whether C0h or FFh
		 * start patterns are to be used, when multiple start flags are required
		 * in SIR mode. If only one start flag is required, this is always C0h.
		 * If n start flags are required, either (n - 1) C0h or (n -1) FFh flags
		 * are sent, followed by a single C0h flag (immediately preceding the
		 * first data byte).
		 */

		struct blr : Register<0x38,16>
		{
			struct xboftype : Bitfield<6,1>
			{
				enum { FF_START_PATTERN = 0, C0_START_PATTERN = 1,};
			};

			struct stsfiforeset : Bitfield<7,1>
			{
				enum { RESET_STATUS_FIFO = 1,};
			};
		};

		/**
		 * USAR Register
		 * The UART autobauding status register (UASR) is selected with a
		 * register bit setting of LCR[7] not equal to BFh or LCR[7] = BFh. The
		 * UART autobauding status register (UASR) returns the speed, the number
		 * of bits by characters, and the type of parity in UART autobauding
		 * mode. In autobauding mode, the input frequency of the UART modem must
		 * be fixed to 48 MHz. Any other module clock frequency results in
		 * incorrect baud rate recognition. This register is used to set up
		 * transmission according to characteristics of previous reception,
		 * instead of LCR, DLL, and DLH registers when UART is in autobauding
		 * mode. To reset the autobauding hardware (to start a new AT detection)
		 * or to set the UART in standard mode (no autobaud), MDR1[2:0] must be
		 * set to 7h (reset state), then set to 2h (UART in autobaud mode) or
		 * cleared to 0 (UART in standard mode). Usage limitation: Only 7 and 8
		 * bits character (5 and 6 bits not supported).  7 bits character with
		 * space parity not supported. Baud rate between 1200 and 115 200 bp/s
		 * (10 possibilities).
		 */

		struct uasr : Register<0x38,16>
		{
			struct speed : Bitfield<0,5>
			{
				enum {
					NO_SPEED_IDENTIFIED = 0x0,
					BAUD_115200         = 0x1,
					BAUD_57600          = 0x2,
					BAUD_38400          = 0x3,
					BAUD_28800          = 0x4,
					BAUD_19200          = 0x5,
					BAUD_14400          = 0x6,
					BAUD_9600           = 0x7,
					BAUD_4800           = 0x8,
					BAUD_2400           = 0x9,
					BAUD_1200           = 0xA,
				};
			};

			struct bitbychar : Bitfield<5,1>
			{
				enum { CHAR_7_BIT = 0, CHAR_8_BIT = 1,};
			};

			struct paritytype : Bitfield<6,2>
			{
				enum {
					NO_PARITY    = 0,
					PARITY_SPACE = 1,
					EVEN_PARITY  = 2,
					ODD_PARITY   = 3,
				};
			};
		};

		/**
		 * ACREG Register
		 * The auxiliary control register (ACREG) is selected with a register bit
		 * setting of LCR[7] = 0. If transmit FIFO is not empty and MDR1[5] = 1,
		 * IrDA starts a new transfer with data of previous frame as soon as
		 * abort frame has been sent. Therefore, TX FIFO must be reset before
		 * sending an abort frame. It is recommended to disable TX FIFO underrun
		 * capability by masking corresponding underrun interrupt. When disabling
		 * underrun by setting ACREG[4] = 1, unknown data is sent over TX line.
		 */

		struct acreg : Register<0x3C,16>
		{
			struct eoten : Bitfield<0,1> { };
			struct aborten : Bitfield<1,1> { };
			struct sctxen  : Bitfield<2,1> { };
			struct sendsip : Bitfield<3,1>
			{
				enum { NO_ACTION = 0, SEND_SIP_PULSE = 1,};
			};
			struct distxunderrun : Bitfield<4,1>
			{
				enum { LONG_STOP_BITS_CANNOT_BE_TRANSMITTED = 0, LONG_STOP_BITS_CAN_BE_TRANSMITTED = 1,};
			};
			struct disirrx : Bitfield<5,1>
			{
				enum { NORMAL_OPERATION = 0, DISABLE_RX_INPUT = 1,};
			};
			struct sdmod : Bitfield<6,1>
			{
				enum { SD_PIN_HIGH = 0, SD_PIN_LOW = 1,};
			};
			struct pulsetype : Bitfield<7,1>
			{
				enum { PULSE_3_16TH_BAUD_RATE = 0, PULSE_1_6_MICROSEC = 1,};
			};
		};

		/**
		 * SCR
		 * The supplementary control register (SCR) is selected with a register
		 * bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh.
		 * Bit 4 enables the wake-up interrupt, but this interrupt is not mapped
		 * into the IIR register. Therefore, when an interrupt occurs and there
		 * is no interrupt pending in the IIR register, the SSR[1] bit must be
		 * checked. To clear the wake-up interrupt, bit SCR[4] must be reset to
		 * 0.
		 */

		struct scr : Register<0x40,16>
		{
			struct dmamodectl : Bitfield<0,1>
			{
				enum { DMAMODE_SET_WITH_FCR3 = 0, DMAMODE_SET_WITH_SCR21 = 1,};
			};
			struct dmamode2 : Bitfield<1,2>
			{
				enum {
					NO_DMA = 0,
					DMA_RX_AND_TX = 1,
					DMA_RX = 2,
					DMA_TX = 3,
				};
			};
			struct txemptyctlit : Bitfield<3,1>
			{
				enum { THR_INT_NORMAL_MODE = 0, THR_INT_ON_TX_EMPTY = 1,};
			};
			struct rxctsdrwakeupenable : Bitfield<4,1>
			{
				enum { DISABLE_WAKE_UP_INT = 0, WAIT_ON_RX_CTS_DSR_INT = 1,};
			};
			struct dsrit : Bitfield<5,1>
			{
				enum { DISABLE_DSR_INT = 0, ENABLE_DSR_INT = 1,};
			};
			struct txtriggergranu1 : Bitfield<6,1>
			{
				enum { DISABLE_GRANULARITY1_FOR_TRIGGER_LEVEL = 0, ENABLE_GRANULARITY1_FOR_TRIGGER_LEVEL = 1,};
			};
			struct rxtriggergranu1 : Bitfield<7,1>
			{
				enum { DISABLE_GRANULARITY1_FOR_TRIGGER_LEVEL = 0, ENABLE_GRANULARITY1_FOR_TRIGGER_LEVEL = 1,};
			};

		};

		/**
		 * SSR Register
		 * The supplementary status register (SSR) is selected with a register
		 * bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh.
		 * Bit 1 is reset only when SCR[4] is reset to 0.
		 */

		struct ssr : Register<0x44,16>
		{
			struct txfifofull : Bitfield<0,1>
			{
				enum { TX_FIFO_FULL = 0, TX_FIFO_NOT_FULL = 1,};
			};
			struct rxctsdsrwakeupsts : Bitfield<1,1>
			{
				enum { NO_FALLING_EDGE_ON_RX = 0, FALLING_EDGE_ON_RX = 1,};
			};
			struct dmacounterrst : Bitfield<2,1>
			{
				enum { DONT_RESET_DMA_CTR_ON_FIFO_RESET = 0, RESET_DMA_CTR_ON_FIFO_RESET = 1,};
			};
		};

		/**
		 * EBLR Register
		 * The BOF length register (EBLR) is selected with a register bit setting
		 * of LCR[7] = 0. In IrDA SIR operation, the BOF length register (EBLR)
		 * specifies the number of BOF + xBOFs to transmit. The value set into
		 * this register must consider the BOF character; therefore, to send only
		 * one BOF with no XBOF, this register must be set to 1. To send one BOF
		 * with n XBOFs, this register must be set to n + 1. Furthermore, the
		 * value 0 sends 1 BOF plus 255 XBOFs. In IrDA MIR mode, the BOF length
		 * register (EBLR) specifies the number of additional start flags (MIR
		 * protocol mandates a minimum of 2 start flags). In CIR mode, the BOF
		 * length register (EBLR) specifies the number of consecutive zeros to be
		 * received before generating the RXSTOP interrupt (IIR[2]). All the
		 * received zeros are stored in the RX FIFO. When the register is cleared
		 * to 0, this feature is deactivated and always in reception state, which
		 * is disabled by setting the ACREG[5] bit to 1. If the RX_STOP interrupt
		 * occurs before a byte boundary, the remaining bits of the last byte are
		 * filled with zeros and then passed into the RX FIFO.
		 */

		struct eblr : Register<0x48,16>
		{
			struct eblr_ : Bitfield<0,8>
			{
				enum {
					DISABLE_FEATURE = 0,
					RXSTOP_AFTER_REC_1_ZERO_BIT = 1,
					RXSTOP_AFTER_REC_255_ZERO_BIT = 0xFF,
				};
			};
		};

		/**
		 * MVR Register
		 * The module version register (MVR) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * reset value is fixed by hardware and corresponds to the RTL revision
		 * of this module. A reset has no effect on the value returned.
		 */

		struct mvr : Register<0x50,16>
		{
			struct minorrev : Bitfield<0,6> { };
			struct majorrev : Bitfield<8,3> { };
		};

		/**
		 * SYSC Register
		 * The system configuration register (SYSC) is selected with a register
		 * bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh.
		 * The AUTOIDLE bit controls a power-saving technique to reduce the logic
		 * power consumption of the module interface; that is, when the feature
		 * is enabled, the interface clock is gated off until the module
		 * interface is accessed. When the SOFTRESET bit is set high, it causes a
		 * full device reset.
		 */

		struct sysc : Register<0x54,16>
		{
			struct autoidle : Bitfield<0,1>
			{
				enum { CLOCK_RUNNING = 0,};
			};
			struct softreset : Bitfield<1,1>
			{
				enum { NORMAL_MODE = 0, RESET = 1,};
			};
			struct enawakeup : Bitfield<2,1>
			{
				enum { DIABLE_WAKEUP = 0, ENABLE_WAKEUP = 1,};
			};
			struct idlemode : Bitfield<3,2>
			{
				enum {
					FORCE_IDLE = 0,
					NO_IDLE    = 1,
					SMART_IDLE = 2,
					SMART_IDLE_WAKEUP = 3,
				};
			};
		};

		/**
		 * SYSS Register
		 * The system status register (SYSS) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh.
		 */

		struct syss : Register<0x58,16>
		{
			struct resetdone : Bitfield<0,1>
			{
				enum { INTERNAL_RESET_ONGOING = 0, RESET_COMPLETE = 1,};
			};
		};

		/**
		 * WER Register
		 * The wake-up enable register (WER) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * wake-up enable register (WER) is used to mask and unmask a UART event
		 * that subsequently notifies the system. An event is any activity in the
		 * logic that can cause an interrupt and/or an activity that requires the
		 * system to wake up. Even if wakeup is disabled for certain events, if
		 * these events are also an interrupt to the UART, the UART still
		 * registers the interrupt as such.
		 */

		struct wer : Register<0x5C,16>
		{
			struct cts_activity : Bitfield<0,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct dsr_activity : Bitfield<1,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct ri_activity : Bitfield<2,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct dcd_activity : Bitfield<3,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct rx_activity : Bitfield<4,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct rhr_interrupt : Bitfield<5,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct rls_interrupt : Bitfield<6,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};

			struct txwakeupen : Bitfield<7,1>
			{
				enum { EVENT_CANNOT_WAKEUP_SYSTEM = 0, EVENT_CAN_WAKEUP_SYSTEM = 1,};
			};
		};

		/**
		 * CFPS Register
		 * The carrier frequency prescaler register (CFPS) is selected with a
		 * register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7]
		 * = BFh. Since the consumer IR (CIR) works at modulation rates of 30-
		 * 56.8 kHz, the 48 MHz clock must be prescaled before the clock can
		 * drive the IR logic. The carrier frequency prescaler register (CFPS)
		 * sets the divisor rate to give a range to accommodate the remote
		 * control requirements in BAUD multiples of 12x. The value of the CFPS
		 * at reset is 105 decimal (69h), which equates to a 38.1 kHz output from
		 * starting conditions. The 48 MHz carrier is prescaled by the CFPS that
		 * is then divided by the 12x BAUD multiple.
		 */

		struct cfps : Register<0x60,16>
		{
			struct cfps_ : Bitfield<0,8>
			{
				enum {
					FREQ_30_KHZ = 133,
					FREQ_32_75_KHZ = 122,
					FREQ_36_KHZ = 111,
					FREQ_36_7_KHZ = 109,
					FREQ_38_KHZ = 105,
					FREQ_40_KHZ = 100,
					FREQ_56_8_KHZ = 70,
				};
			};
		};

		struct rxfifo_lvl : Register<0x64,16>
		{
			struct rxfifo_lvl_ : Bitfield<0,8> { };
		};

		struct txfifo_lvl : Register<0x68,16>
		{
			struct txfifo_lvl_ : Bitfield<0,8> { };
		};

		/**
		 * IER2 Register
		 * The IER2 enables RX/TX FIFOs empty corresponding interrupts.
		 */

		struct ier2 : Register<0x6C,16>
		{
			struct en_rxfifo_empty : Bitfield<0,1>
			{
				enum { EN_RX_FIFO_EMPTY_INT_DISABLE = 0, EN_RX_FIFO_EMPTY_INT_ENABLE = 1,};
			};

			struct en_txfifo_empty : Bitfield<1,1>
			{
				enum { EN_TX_FIFO_EMPTY_INT_DISABLE = 0, EN_TX_FIFO_EMPTY_INT_ENABLE = 1,};
			};
		};

		/**
		 * ISR2 Register
		 * The interrupt status register 2 (ISR2) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * ISR2 displays the status of RX/TX FIFOs empty corresponding
		 * interrupts.
		 */

		struct isr2 : Register<0x70,16>
		{
			struct rxfifo_empty_sts : Bitfield<0,1>
			{
				enum { RX_FIFO_EMPTY_INT_NOT_PENDING = 0, RX_FIFO_EMPTY_INT_PENDING = 1,};
			};

			struct txfifo_empty_sts : Bitfield<1,1>
			{
				enum { TX_FIFO_EMPTY_INT_NOT_PENDING = 0, TX_FIFO_EMPTY_INT_PENDING = 1,};
			};
		};

		struct freq_sel : Register<0x74,16>
		{
			struct freq_sel_ : Bitfield<0,8> { };
		};

		/**
		 * MDR3 Register
		 * The mode definition register 3 (MDR3) is selected with a register bit
		 * setting of LCR[7] = 0, LCR[7] not equal to BFh, or LCR[7] = BFh. The
		 * DISABLE_CIR_RX_DEMOD register bit will force the CIR receiver to
		 * bypass demodulation of received data if set. See the CIR Mode Block
		 * Components. The NONDEFAULT_FREQ register bit allows the user to set
		 * sample per bit by writing it into FREQ_SEL register. Set it if
		 * non-default (48 MHz) fclk frequency is used to achieve a less than 2%
		 * error rate.  Changing this bit (to any value) will automatically
		 * disable the device by setting MDR[2:0] to 111 .
		 */

		struct mdr3 : Register<0x80,16>
		{
			struct disable_cir_rx_demod : Bitfield<0,1>
			{
				enum { ENABLE_CIR_RX_DEMODULATION = 0, DISABLE_CIR_RX_DEMODULATION = 1,};
			};
			struct nondefault_freq : Bitfield<1,1>
			{
				enum { DISABLE_NONDEFAULT_FDK_FREQ = 0, ENABLE_NONDEFAULT_FDK_FREQ = 1,};
			};
			struct set_dma_tx_threshold : Bitfield<2,1>
			{
				enum { DISABLE_TX_DMA_THRESHOLD = 0, ENABLE_TX_DMA_THRESHOLD = 1,};
			};
		};

		/** TX_DMA_THRESHOLD Register The TX DMA threshold register is selected
		 * with a register bit setting of LCR[7] = 0, LCR[7] not equal to BFh, or
		 * LCR[7] = BFh.
		 */

		struct tx_dma_threshold : Register<0x84,16>
		{
			struct tx_dma_threshold_ : Bitfield<0,5> { };
		};

		/**
		 * Functions that are required by the interface
		 */
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
			Genode::log("_rx_enable not implemented");
		}

		bool _rx_avail()
		{
//			return (read<Ufstat>() & (Ufstat::Rx_fifo_count::bits(0xff)
//			        | Ufstat::Rx_fifo_full::bits(1))); }
			Genode::log("_rx_avail not implemented");
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
			Genode::log("_rx_char not implemented");
			return 'x';
		}

	public:

		/**
		 * Constructor
		 *
		 * \param  base       MMIO base address
		 * \param  clock      reference clock
		 * \param  baud_rate  targeted baud rate
		 */
		AM335x_uart(Genode::Env &env, addr_t const base, unsigned const clock,
		            unsigned const baud_rate) : Mmio(base)
		{
			Genode::Cmper(env, Am335x::CM_PER_BASE,Am335x::CM_PER_SIZE).enable_uart_clock();
			Genode::Cmper(env, Am335x::CM_PER_BASE,Am335x::CM_PER_SIZE).enable_cpsw_clock();

				//cmper.enable_uart_clock();
			/* RX and TX FIFO reset */
//			write<Ufcon::Rx_fifo_rst>(1);
//			write<Ufcon::Tx_fifo_rst>(1);
//			while (read<Ufcon::Rx_fifo_rst>() || read<Ufcon::Tx_fifo_rst>()) ;
//
			/* init control registers */
//			write<Ulcon>(Ulcon::init_value());
//			write<Ucon>(Ucon::init_value());
//			write<Umcon>(Umcon::init_value());
//
			/* apply baud rate */
//			float const div_val = ((float)clock / (baud_rate * 16)) - 1;
//			Ubrdiv::access_t const ubrdiv = div_val;
//			Ufracval::access_t const ufracval =
//				((float)div_val - ubrdiv) * 16;
//			write<Ubrdiv::Baud_rate_div>(ubrdiv);
//			write<Ufracval::Baud_rate_frac>(ufracval);
			Genode::log("AM335x_uart contructor not implemented");
		}

		/**
		 * Print character 'c' through the UART
		 */
		void put_char(char const c)
		{
			while (read<lsr_uart::txsre>()) ;
			write<thr::_thr>(c);
		}
};

#endif /* _INCLUDE__DRIVERS__UART__AM335X_H_ */
