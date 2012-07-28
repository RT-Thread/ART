/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id$
*/

#include "wiring_private.h"

extern "C" {
extern rt_uint32_t rt_hw_tick_get_millisecond(void);
extern rt_uint32_t rt_hw_tick_get_microsecond(void);
}

unsigned long millis()
{
	return rt_hw_tick_get_millisecond();
}

unsigned long micros() {
	return rt_hw_tick_get_microsecond();
}

void delay(unsigned long ms)
{
	rt_tick_t tick;

	tick = rt_tick_from_millisecond(ms);
	rt_thread_delay(tick);
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	rt_uint32_t ms;
	rt_uint32_t us_delay;
	rt_uint32_t us_current;

	ms = us/1000;
	us_delay = us % 1000;

	if (ms)
	{
		delay(ms);
	}

	if (us_delay)
	{
		us_current = micros();
		us_delay += us_current;

		while (us_current < us_delay)
		{
			us_current = micros();
		}
	}
}

void init()
{
}
