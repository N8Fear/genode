/*
 * \brief  Regulator definitions for AM335x
 * \author Hinnerk van Bruinehsen <hvbruinehsen@stackptr.de>
 * \date   2018-07-18
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Hinnerk van Bruinehsen
 *
 * This file is part of the Genode OS Framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__SPEC__AM335X__REGULATOR__CONSTS_H_
#define _INCLUDE__SPEC__AM335X__REGULATOR__CONSTS_H_

#include <util/string.h>

namespace Regulator {

	enum Regulator_id {
		PWR_UART_1,
		PWR_UART_2,
		PWR_UART_3,
		PWR_GPIO_0,
		PWR_GPIO_1,
		PWR_GPIO_2,
		MAX,
		INVALID
	};

	struct Regulator_name {
		Regulator_id id;
		const char * name;
	};

	static constexpr Regulator_name names[] = {
		                                          { PWR_UART_1, "power-uart1"
},
		                                          { PWR_UART_2, "power-uart2"
},
		                                          { PWR_UART_3, "power-uart3"
},
		                                          { PWR_GPIO_0, "power-gpio0"
},
		                                          { PWR_GPIO_1, "power-gpio1"
},
		                                          { PWR_GPIO_2, "power-gpio2"
},
	                                          };

	inline Regulator_id regulator_id_by_name(const char * name)
	{
		for (unsigned i = 0; i < sizeof(names)/sizeof(names[0]); i++)
			if (Genode::strcmp(names[i].name, name) == 0)
				return names[i].id;
		return INVALID;
	}

	inline const char * regulator_name_by_id(Regulator_id id) {
		return (id < sizeof(names)/sizeof(names[0])) ? names[id].name : 0; }
}


#endif /* _INCLUDE__SPEC__AM335X__REGULATOR__CONSTS_H_ */
