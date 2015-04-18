/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TEST_H
#define TEST_H

void test_begin();
void test_result(int succeeded, const char* name);
int test_retcode();

#endif // TEST_H
