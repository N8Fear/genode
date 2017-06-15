/*
 * \brief  Core implementation of the IO_PORT session interface
 * \author Alexander Boettcher
 * \date   2016-07-17
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/log.h>

/* core includes */
#include <io_port_session_component.h>

#include <sel4/sel4.h>

using namespace Genode;


/**************
 ** Port API **
 **************/
unsigned char Io_port_session_component::inb(unsigned short address)
{
	return 0;
}


unsigned short Io_port_session_component::inw(unsigned short address)
{
	return 0;
}


unsigned Io_port_session_component::inl(unsigned short address)
{
	return 0;
}


void Io_port_session_component::outb(unsigned short address, unsigned char value)
{
}


void Io_port_session_component::outw(unsigned short address, unsigned short value)
{
}


void Io_port_session_component::outl(unsigned short address, unsigned value)
{
}
