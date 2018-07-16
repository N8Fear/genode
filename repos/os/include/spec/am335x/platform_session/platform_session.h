/*
 * \brief AM335x specific platform session
 * \author Hinnerk van Bruinehsen <h.v.bruinehsen@stackptr.de>
 * \date 2018-04-13
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__PLATFORM_SESSION__PLATFORM_SESSION_H_
#define  _INCLUDE__PLATFORM_SESSION__PLATFORM_SESSION_H_

#include <base/capability.h>
#include <base/rpc.h>

namespace Platform { struct Session; }

struct Platform::Session : Genode::Session
{
  enum Device {
               UART_0,
               UART_1,
               SDMMC_1,
               CPSW_ETHERNET_0,
               CPSW_ETHERNET_1,
  };

  enum Board_revision {
                       WEGA_AM335X = 2,
                       BEAGLE_BONE_BLACK = 3,
                       UNKNOWN
  };

  /**
   * \noapi
   */

  static const char *service_name() { return "Platform"; }

  enum { CAP_QUOTA = 2 };

  virtual ~Session() { }

  virtual void enable(Device dev) = 0;
  virtual void disable(Device dev) = 0;
  virtual void clock_rate(Device dev, unsigned long rate) = 0;
  virtual Board_revision revision() = 0;

  /*********************
   ** RPC declaration **
   *********************/

  GENODE_RPC(Rpc_enable, void, enable, Device);
  GENODE_RPC(Rpc_disable, void, disable, Device);
  GENODE_RPC(Rpc_clock_rate, void, clock_rate, Device, unsigned long);
  GENODE_RPC(Rpc_revision, Board_revision, revision);

  GENODE_RPC_INTERFACE(Rpc_enable, Rpc_disable, Rpc_clock_rate, Rpc_revision);
};

#endif /* _INCLUDE__PLATFORM_SESSION__PLATFORM_SESSION_H_ */
