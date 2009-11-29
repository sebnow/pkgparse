/* Copyright (c) 2009 Sebastian Nowicki <sebnow@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "cmockery.h"

#include "pkgbuild_scanner.h"
#include "pkgbuild_parse.h"

void test_scan_simple(void **state)
{
	/* This string should lex to NAME EQUAL VALUE('bar') */
	const char *const string = "foo='bar'";
	scanner_t *scanner;
	token_t token;

	scanner = scanner_new_with_string(string);
	assert_true(scanner_next_token(scanner, &token));
	assert_true(token.id == TOKEN_NAME);
	assert_true(scanner_next_token(scanner, &token));
	assert_true(token.id == TOKEN_EQUAL);
	assert_true(scanner_next_token(scanner, &token));
	assert_true(token.id == TOKEN_VALUE);
	assert_string_equal(token.value, "'bar'");
	assert_false(scanner_next_token(scanner, &token));
}

