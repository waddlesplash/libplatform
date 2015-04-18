/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "testutils.h"

#include <stdio.h>

int tests_failed;
int tests_succeded;

#ifdef WIN32_LEAN_AND_MEAN
#  define COLOR_RED
#  define COLOR_GREEN
#  define COLOR_RESET
#else
#  define COLOR_RED "\e[97m\e[101m"
#  define COLOR_GREEN "\e[97m\e[42m"
#  define COLOR_RESET "\e[0m"
#endif

void test_begin()
{
	tests_failed = 0;
	tests_succeded = 0;
	puts("");
}

void test_result(int succeeded, const char* name)
{
	if (succeeded) {
		printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", name);
		tests_succeded++;
	} else {
		printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", name);
		tests_failed++;
	}
}

int test_retcode()
{
	printf("\nFinished: %d tests passed, %d failed.\n\n", tests_succeded,
		tests_failed);
	return tests_failed;
}
