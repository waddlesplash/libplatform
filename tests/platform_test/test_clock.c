/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "testutils.h"

#include "platform/clock.h"

#include <stddef.h>

void run_clock_tests()
{
	Clock c = NULL;
	c = clock_create();
	test_result((c != NULL), "clock_create()");

	clock_sleep(50);
	test_result(clock_get_time(c), "clock_sleep(), clock_get_time()");
	clock_destroy(c);
}
