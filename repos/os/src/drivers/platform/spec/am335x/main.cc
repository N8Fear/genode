/*
 * \brief  Driver for AM335x specific platform devices (clocks, power, etc.)
 * \author Hinnerk van Bruinehsen <h.v.bruinehsen@stackptr.de
 * \date   2018-07-11
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/log.h>
#include <base/heap.h>
#include <base/component.h>
// #include <base/attached_rom_dataspace.h>
#include <regulator/component.h>
#include <regulator/consts.h>

#include <cmper.h>
#include <cmdpll.h>

using namespace Genode;

struct Driver_factory : Regulator::Driver_factory
{
	Cmper  _cmper;
	Cmdpll _cmdpll;

	Driver_factory(Genode::Env &env) : _cmper(env), _cmdpll(env) { }
	// Attached_rom_dataspace config {env, "config"};

	Regulator::Driver &create(Regulator::Regulator_id id)
	override
	{
		switch (id) {
			case Regulator::CLK_UART_1:
			//case Regulator::CLK_UART_2:
			//case Regulator::CLK_UART_3:
		//case Regulator::PWR_1: // for example
		//case Regulator::PWR_2: // for example
		  return _cmper;

		//case Regulator::CLK_1: // for example
		//case Regulator::CLK_2: // for example
		//  return _cmdpll;

	  default:
	  	throw Genode::Service_denied();
	  }
	}

	void destroy(Regulator::Driver &) override { }
};


struct Main
{
	Genode::Env &    env;
	Genode::Heap     heap {env.ram(), env.rm() };
	::Driver_factory factory { env };
	Regulator::Root  root {env, heap, factory };

	Main(Genode::Env & env) : env(env) {
		env.parent().announce(env.ep().manage(root));
	}

};

void Component::construct(Genode::Env &env)
{
	Genode::log(" --- AM335x platform driver ---");

	static Main main(env);
}
