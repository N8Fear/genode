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
#include <root/component.h>
#include <platform_session/platform_session.h>

namespace Platform {

	class Session_component;
	class Root;
}

class Platform::Session_component : public Genode::Rpc_object<Platform::Session>
{
 private:
 public:
/*********************************
 *  Platform session interface  **
 *********************************/

void enable(Device dev)
{
	switch (dev) {
	default:
		Genode::warning("invalid device");
	};
}

void disable(Device dev)
{
	switch (dev) {
	default:
		Genode::warning("invalid device");
	};
}

void clock_rate(Device dev, unsigned long /* rate */)
{
	switch (dev) {
	default:
		Genode::warning("invalid device");
	};
}

	Board_revision revision() { return UNKNOWN; }
};

class Platform::Root : public Genode::Root_component<Platform::Session_component>
{
	private:
		Genode::Env &_env;
	protected:

		Session_component *_create_session(const char *) {
			return new (md_alloc()) Session_component();
		}

	public:

  Root(Genode::Env &env, Genode::Allocator &md_alloc)
		: Genode::Root_component<Session_component>(env.ep(), md_alloc), _env(env)
		{ }
};

struct Main
{
	Genode::Env &  env;
	Genode::Heap   heap { env.ram(), env.rm() };
	Platform::Root root { env, heap };

	Main(Genode::Env &env) : env(env) {
		env.parent().announce(env.ep().manage(root)); }
};

void Component::construct(Genode::Env &env)
{
	Genode::log("--- AM335x platform driver ---");
	Genode::log("TODO/Work in Progress - nothing happens here right now");

	static Main main(env);
}
