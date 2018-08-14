/*
 * \brief  Pinmux driver for AM335x SoC/Wega Board Control module
 * \author Hinnerk van Bruinehsen
 * \date   2018-03-05
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_
#define _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_

#include <util/mmio.h>
#include <base/attached_io_mem_dataspace.h>
#include <drivers/defs/am335x.h>
#include <base/log.h>


class Pinmux : public Genode::Attached_io_mem_dataspace,
              Genode::Mmio
{
	private:

	struct conf_gpmc_ad0   : Register<0x0, 32> { };
	struct conf_gpmc_ad1   : Register<0x4, 32> { };
	struct conf_gpmc_ad2   : Register<0x8, 32> { };
	struct conf_gpmc_ad3   : Register<0xC, 32> { };
	struct conf_gpmc_ad4   : Register<0x10,32> { };
	struct conf_gpmc_ad5   : Register<0x14,32> { };
	struct conf_gpmc_ad6   : Register<0x18,32> { };
	struct conf_gpmc_ad7   : Register<0x1c,32> { };
	struct conf_gpmc_ad8   : Register<0x20,32> { };
	struct conf_gpmc_ad9   : Register<0x24,32> { };
	struct conf_gpmc_ad10  : Register<0x28,32> { };
	struct conf_gpmc_ad11  : Register<0x2c,32> { };
	struct conf_gpmc_ad12  : Register<0x30,32> { };
	struct conf_gpmc_ad13  : Register<0x34,32> { };
	struct conf_gpmc_ad14  : Register<0x38,32> { };
	struct conf_gpmc_ad15  : Register<0x3c,32> { };

	struct conf_gpmc_a0    : Register<0x40, 32> { };
	struct conf_gpmc_a1    : Register<0x44, 32> { };
	struct conf_gpmc_a2    : Register<0x48, 32> { };
	struct conf_gpmc_a3    : Register<0x4C, 32> { };
	struct conf_gpmc_a4    : Register<0x50, 32> { };
	struct conf_gpmc_a5    : Register<0x54, 32> { };
	struct conf_gpmc_a6    : Register<0x58, 32> { };
	struct conf_gpmc_a7    : Register<0x5c, 32> { };
	struct conf_gpmc_a8    : Register<0x60, 32> { };
	struct conf_gpmc_a9    : Register<0x64, 32> { };
	struct conf_gpmc_a10   : Register<0x68, 32> { };
	struct conf_gpmc_a11   : Register<0x6c, 32> { };

	struct conf_gpmc_wait0 : Register<0x70, 32> { };
	struct conf_gpmc_wpn   : Register<0x74, 32> { };
	struct conf_gpmc_ben1  : Register<0x78, 32> { };

  /* Fill in some more missing pinmux registers : not required right now */

	/* MMC0 related pinmuxing */
	struct conf_mmc0_dat3  : Register<0xf0, 32> { };
	struct conf_mmc0_dat2  : Register<0xf4, 32> { };
	struct conf_mmc0_dat1  : Register<0xf8, 32> { };
	struct conf_mmc0_dat0  : Register<0xfc, 32> { };
	struct conf_mmc0_clk   : Register<0x100,32> { };
	struct conf_mmc0_cmd   : Register<0x104,32> { };

  /* CPSW/PHY related pinmuxing */
	struct conf_mii1_col      : Register<0x108, 32> { };
	struct conf_mii1_crs      : Register<0x10c, 32> { };
	struct conf_mii1_rx_er    : Register<0x110, 32> { };
	struct conf_mii1_tx_en    : Register<0x114, 32> { };
	struct conf_mii1_rx_dv    : Register<0x118, 32> { };
	struct conf_mii1_txd3     : Register<0x11c, 32> { };
	struct conf_mii1_txd2     : Register<0x120, 32> { };
	struct conf_mii1_txd1     : Register<0x124, 32> { };
	struct conf_mii1_txd0     : Register<0x128, 32> { };
	struct conf_mii1_tx_clk   : Register<0x12c, 32> { };
	struct conf_mii1_rx_clk   : Register<0x130, 32> { };
	struct conf_mii1_rxd3     : Register<0x134, 32> { };
	struct conf_mii1_rxd2     : Register<0x138, 32> { };
	struct conf_mii1_rxd1     : Register<0x13c, 32> { };
	struct conf_mii1_rxd0     : Register<0x140, 32> { };
	struct conf_rmii1_ref_clk : Register<0x144, 32> { };
	struct conf_mdio          : Register<0x148, 32> { };
	struct conf_mdc           : Register<0x14c, 32> { };

	struct conf_spi0_cs1      : Register<0x160, 32> { };

	struct conf_uart0_ctsn    : Register<0x168, 32> { };
	struct conf_uart0_rtsn    : Register<0x16c, 32> { };
	struct conf_uart0_rxd     : Register<0x170, 32> { };
	struct conf_uart0_txd     : Register<0x174, 32> { };
	struct conf_uart1_ctsn    : Register<0x178, 32> { };
	struct conf_uart1_rtsn    : Register<0x17c, 32> { };
	struct conf_uart1_rxd     : Register<0x180, 32> { };
	struct conf_uart1_txd     : Register<0x184, 32> { };
	/* mmc0 would be more sensible because of the register names, but it is mmc 1 in the device tree */
public:
	void uart0_pinmux_setup()
	{
		Genode::log("Setting up pinmuxing for UART0");
		write<conf_uart0_rxd>(0x30);
		write<conf_uart0_txd>(0x00);
	}

	void uart1_pinmux_setup()
	{
		Genode::log("Setting up pinmuxing for UART1");
		write<conf_uart1_rxd>(0x30);
		write<conf_uart1_txd>(0x00);
		write<conf_uart1_ctsn>(0x28);
		write<conf_uart1_rtsn>(0x00);
	}

	void mmc1_pinmux_setup()
	{
		Genode::log("Setting up pinmuxing for SD/MMC 1");
		write<conf_mmc0_dat3>(0x30);
		write<conf_mmc0_dat2>(0x30);
		write<conf_mmc0_dat1>(0x30);
		write<conf_mmc0_dat0>(0x30);
		write<conf_mmc0_clk>(0x30);
		write<conf_mmc0_cmd>(0x30);
		write<conf_spi0_cs1>(0x37);

	}

	void cpsw_pinmux_setup()
	{
		/* Ethernet 0 from device tree */
		Genode::log("Setting up pinmuxing for CPSW ethernet");
		write<conf_mii1_crs>(0x21);
		write<conf_mii1_rx_er>(0x21);
		write<conf_mii1_tx_en>(0x9);
		write<conf_mii1_txd1>(0x9);
		write<conf_mii1_txd0>(0x9);
		write<conf_mii1_rxd1>(0x21);
		write<conf_mii1_rxd0>(0x21);
		write<conf_rmii1_ref_clk>(0x20);

		/* mdio */
		write<conf_mdio>(0x70);
		write<conf_mdc>(0x10);

		/* Ethernet 1 from device tree */
		write<conf_gpmc_a0>(0x09u);
		write<conf_gpmc_a1>(0x21u);
		write<conf_gpmc_a2>(0x09u);
		write<conf_gpmc_a3>(0x09u);
		write<conf_gpmc_a4>(0x09u);
		write<conf_gpmc_a5>(0x09u);
		write<conf_gpmc_a6>(0x21u);
		write<conf_gpmc_a7>(0x21u);
		write<conf_gpmc_a8>(0x21u);
		write<conf_gpmc_a9>(0x21u);
		write<conf_gpmc_a10>(0x21u);
		write<conf_gpmc_a11>(0x21u);
		write<conf_gpmc_wpn>(0x21u);
		write<conf_gpmc_ben1>(0x21u);


		/* From starterware /camkes */
		//write<conf_mii1_rx_er>(0x0 | 0x20u);
		//write<conf_mii1_tx_en>(0x0);
		//write<conf_mii1_rx_dv>(0x0 | 0x20u);
		//write<conf_mii1_txd3>(0x0);
		//write<conf_mii1_txd2>(0x0);
		//write<conf_mii1_txd1>(0x0);
		//write<conf_mii1_txd0>(0x0);
		//write<conf_mii1_tx_clk>(0x0 | 0x20u);
		//write<conf_mii1_rx_clk>(0x0 | 0x20u);
		//write<conf_mii1_rxd3>(0x0 | 0x20u);
		//write<conf_mii1_rxd2>(0x0 | 0x20u);
		//write<conf_mii1_rxd1>(0x0 | 0x20u);
		//write<conf_mii1_rxd0>(0x0 | 0x20u);
		//write<conf_mdio>(0x0 | 0x10u);
		//write<conf_mdc>(0x0 | 0x10u);
    }


	/**
	 * Constructor
	 */
	Pinmux(Genode::Env &env)
		: Genode::Attached_io_mem_dataspace(env, (Am335x::CTR_MOD_BASE + 0x800),
																				0x234),
																				Genode::Mmio((Genode::addr_t)local_addr<void>())
	{
		Genode::log("Setting up pinmuxing");
		cpsw_pinmux_setup();
		mmc1_pinmux_setup();
		uart0_pinmux_setup();
		uart1_pinmux_setup();
	}

	virtual ~Pinmux() { }

};


#endif /* _DRIVERS__PLATFORM__SPEC__AM335X__PINMUX_H_ */
