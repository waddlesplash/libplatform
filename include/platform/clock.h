/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PLATFORM_CLOCK_H
#define PLATFORM_CLOCK_H

#include "types.h"

typedef void* Clock;

Clock clock_create();
void clock_destroy(Clock clock);
int32_t clock_get_time(Clock clock);

void clock_sleep(int32_t ms);

#endif // PLATFORM_CLOCK_H
