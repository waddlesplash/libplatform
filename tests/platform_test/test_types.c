/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "testutils.h"

#define USE_BUILTIN_TYPES
#include "platform/types.h"

#include <stdio.h>

void run_types_tests()
{
	bool one = false;
	test_result((one == false), "bool = false");

	bool two = true;
	test_result((two == true), "bool = true");

	bool three = 1;
	test_result((three == true), "bool = 1");

	bool four = 1.5;
	test_result((four == true), "bool = 1.5");

	bool five1 = false;
	bool five2 = true;
	test_result(!(five1 && five2), "false && true");
	test_result((five1 || five2), "false || true");

	puts("");

	test_result(sizeof(int8_t) == 1, "sizeof(int8_t) == 1");
	test_result(sizeof(int16_t) == 2, "sizeof(int16_t) == 2");
	test_result(sizeof(int32_t) == 4, "sizeof(int32_t) == 4");
	test_result(sizeof(int64_t) == 8, "sizeof(int64_t) == 8");
}
