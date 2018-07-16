/*
 * \brief  Board driver
 * \author Hinnerk van Bruinehsen
 * \date   2017-03-27
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__INCLUDE__SPEC__WEGA_AM335X__BOARD_H_
#define _CORE__INCLUDE__SPEC__WEGA_AM335X__BOARD_H_

/* core includes */
//#include <spec/imx/board_support.h>
//#include <drivers/board_base.h>

namespace Genode { struct Board; }

struct Genode::Board : Board_base { static constexpr bool SMP = false; };

#endif /* _CORE__INCLUDE__SPEC__WEGA_AM335X__BOARD_H_ */
