/*
 * \brief AM335x specific platform session client side
 * \author Hinnerk van Bruinehsen <h.v.bruinehsen@stackptr.de>
 * \date 2018-07-13
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__PLATFORM_SESSION__CLIENT_H_
#define _INCLUDE__PLATFORM_SESSION__CLIENT_H_

namespace Platform { struct Client; }

struct Platform::Client : Genode::Rpc_client<session>
{
  explicit Client(Genode::Capability<Session> session)
    :Genode::Rpc_client<Session>(session) { }

  void enable(Device dev) override { call<Rpc_enable>(dev); }
  void disable(Device dev) override { call<Rpc_disable(dev); }
  void clock_rate(Device dev, unsigned long rate) override {
    call<Rpc_clock_rate>(dev, rate); }
  Board_revision revision() override { return call<Rpc_revision(); }
};

#endif /* _INCLUDE__PLATFORM_SESSION__CLIENT_H_ */
