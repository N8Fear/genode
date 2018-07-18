/*
 * \brief  Regulator driver for the DLL unit of the AM335x Soc
 * \author Hinnerk van Bruinehsen
 * \date   2018-03-05
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__PLATFORM__SPWC__AM335X__CMDPLL_H_
#define _DRIVERS__PLATFORM__SPWC__AM335X__CMDPLL_H_

#include <regulator/consts.h>
#include <regulator/driver.h>
#include <drivers/defs/am335x.h>
#include <os/attached_mmio.h>
#include <base/log.h>

using namespace Regulator;

class Cmdpll : public Regulator::Driver,
               private Genode::Attached_mmio
{
private:
		struct cm_dpll_clksel_timer7_clk : Register<0x4,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer2_clk : Register<0x8,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer3_clk : Register<0xc,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer4_clk : Register<0x10,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_cm_mac_clksel : Register<0x14,32>
		{
			struct mii_clk_sel : Bitfield<2,1> { };
		};
		struct cm_dpll_clksel_timer5_clk : Register<0x18,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_timer6_clk : Register<0x1c,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_cm_cpts_rft_clksel : Register<0x20,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_timer1ms_clk : Register<0x28,32>
		{
			struct clksel : Bitfield<0,3> { };
		};
		struct cm_dpll_clksel_gfx_fclk : Register<0x2c,32>
		{
			struct clkdiv_sel_gfx_fclk : Bitfield<0,1> { };
			struct clksel_gfx_clk : Bitfield<1,1> { };
		};
		struct cm_dpll_clksel_pru_icss_ocp_clk : Register<0x30,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_lcdc_pixel_clk : Register<0x34,32>
		{
			struct clksel : Bitfield<0,2> { };
		};
		struct cm_dpll_clksel_wdt1_clk : Register<0x38,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		struct cm_dpll_clksel_gpio0_dbclk : Register<0x3c,32>
		{
			struct clksel : Bitfield<0,1> { };
		};
		/*********************
		 ** Device funtions **
		 *********************/

		void _enable(Regulator_id id)
		{
			switch (id) {
			default:
				Genode::warning("enabling regulator unsupported for ", names[id].name);
			}
		}

		void _disable(Regulator_id id)
		{
			switch (id) {
			default:
				Genode::warning("disabling regulator unsupported for ", names[id].name);
			}
		}

public:

		/*
		 * Constructor
		 */

		Cmdpll(Genode::Env &env)
		: Genode::Attached_mmio(env, Am335x::CM_DPLL_BASE,
		                        Am335x::CM_DPLL_SIZE)
		{
			Genode::log("Instatiate CM_DPLL");
		}

		virtual ~Cmdpll() { }

		/********************************
		 ** Regulator driver interface **
		 ********************************/

		void level(Regulator_id id, unsigned long level)
		{
			switch (id) {
			default:
				Genode::warning("level setting unsupported for ", names[id].name);
			}
		}

		unsigned long level(Regulator_id id)
		{
			switch (id) {
			default:
				Genode::warning("level request unsupported for ", names[id].name);
			}
			return 0;
		}

		void state(Regulator_id id, bool enable)
		{
			if (id)
				_enable(id);
			else
				_disable(id);
		}

		bool state(Regulator_id id)
		{
			switch (id) {
			default:
				Genode::warning("state request unsupported for ", names[id].name);
			}
			return true;
		}
};

#endif /* _DRIVERS__PLATFORM__SPWC__AM335X__CMDPLL_H_ */
