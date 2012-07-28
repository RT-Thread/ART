/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <rthw.h>

#include "Arduino.h"
#include "wiring_private.h"

#include "HardwareSerial.h"

// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(const char* name)
{
	serial = (struct rt_serial_device*)rt_device_find(name);
	if (serial != RT_NULL)
	{
		rt_device_open(RT_DEVICE(serial), RT_DEVICE_OFLAG_RDWR);
	}
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud)
{
	struct serial_configure config;
	config.baud_rate = baud;
	config.bit_order = BIT_ORDER_LSB;
	config.data_bits = DATA_BITS_8;
	config.parity    = PARITY_NONE;
	config.stop_bits = STOP_BITS_1;
	config.invert    = NRZ_NORMAL;

	rt_device_control(RT_DEVICE(serial), RT_DEVICE_CTRL_CONFIG, &config);
}

void HardwareSerial::end()
{
}

int HardwareSerial::available(void)
{
	rt_uint32_t size;
	rt_base_t level;

	level = rt_hw_interrupt_disable();
	size = (serial->int_rx->put_index - serial->int_rx->get_index) & (SERIAL_RBUFFER_SIZE - 1);
	rt_hw_interrupt_enable(level);

	return size;
}

int HardwareSerial::peek(void)
{
	int ch;
	rt_base_t level;

	ch = -1;
	/* disable interrupt */
	level = rt_hw_interrupt_disable();
	if (serial->int_rx->get_index != serial->int_rx->put_index)
	{
		ch = serial->int_rx->buffer[serial->int_rx->get_index];
	}
	/* enable interrupt */
	rt_hw_interrupt_enable(level);

	return ch;
}

int HardwareSerial::read(void)
{
	rt_uint8_t ch;
	
	if (rt_device_read(RT_DEVICE(serial), -1, &ch, 1) == 1)
		return ch;
	return -1;
}

void HardwareSerial::flush()
{
	/* nothing */
}

size_t HardwareSerial::write(uint8_t c)
{
	return rt_device_write(RT_DEVICE(serial), -1, &c, 1);
}

HardwareSerial::operator bool() {
	return true;
}

// Preinstantiate Objects //////////////////////////////////////////////////////
HardwareSerial Serial("vcom");
