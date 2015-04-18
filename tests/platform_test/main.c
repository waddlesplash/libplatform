/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "testutils.h"

#include <stdio.h>

// Declarations of the test functions.
void run_types_tests();
void run_clock_tests();
void run_glwindow_tests();

int main(int argc, char* argv)
{
	test_begin();

	run_types_tests();
	puts("");
	run_clock_tests();
	puts("");
	run_glwindow_tests();

	return test_retcode();
}
