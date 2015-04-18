/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

// This *must* be included first, or else bad things happen.
#define _XOPEN_SOURCE 500 /* POSIX '95 */
#include <unistd.h>

#include "platform/clock.h"

void clock_sleep(int32_t ms)
{
	usleep(ms * 1000);
}
