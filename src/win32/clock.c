/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "platform/clock.h"

#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

struct TimeClock {
	DWORD timeSinceLast;
};

Clock clock_create()
{
	/* Originally I wanted to use QPF/QPC, but according to
	* http://www.virtualdub.org/blog/pivot/entry.php?id=106, they're
	* often very flaky. */
	struct TimeClock* clock = malloc(sizeof(struct TimeClock));

	clock->timeSinceLast = timeGetTime();
	return (Clock)clock;
}

void clock_destroy(Clock cl)
{
	struct TimeClock* clock = (struct TimeClock*)cl;
	free(clock);
}

int32_t clock_get_time(Clock cl)
{
	struct TimeClock* clock = (struct TimeClock*)cl;
	DWORD previousTime = clock->timeSinceLast;
	clock->timeSinceLast = timeGetTime();
	return (clock->timeSinceLast - previousTime);
}

void clock_sleep(int32_t ms)
{
	Sleep(ms);
}
