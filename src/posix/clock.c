/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

// This *must* be included first, or else bad things happen.
#define _XOPEN_SOURCE 500 /* POSIX '95 */
#include <time.h>
#include <stdlib.h>

#include "platform/clock.h"

struct GettimeClock {
	int32_t timeSinceLast;
};

Clock clock_create()
{
	Clock clock = (Clock)malloc(sizeof(struct GettimeClock));
	clock_get_time(clock); // Initialize timeSinceLast.
	return clock;
}

void clock_destroy(Clock cl)
{
	struct GettimeClock* clock = (struct GettimeClock*)cl;
	free(clock);
}

int32_t clock_get_time(Clock cl)
{
	struct GettimeClock* clock = (struct GettimeClock*)cl;
	struct timespec t;
	int status = clock_gettime(CLOCK_MONOTONIC, &t);
	if (status != 0)
		exit(1); // TODO: report error

	int32_t previousTime = clock->timeSinceLast;
	clock->timeSinceLast = (t.tv_sec*1000 + t.tv_nsec/1000);
	return (clock->timeSinceLast - previousTime);
}
