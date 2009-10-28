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
#include <stdlib.h>
#include <string.h>

#include "utility.h"

void test_unquote_simple_string(void **state)
{
	char string[] = "\"foo bar spam eggs ham\"";
	char *parsed = sh_unquote(string);
	assert_string_equal(parsed, "foo bar spam eggs ham");
	free(parsed);
}

void test_unquote_subsequenctly_quoted(void **state)
{
	char string[] = "foo \"bar spam\" eggs ham";
	char *parsed = sh_unquote(string);
	assert_string_equal(parsed, "foo \"bar spam\" eggs ham");
	free(parsed);
}

void test_parse_array(void **state)
{
	char *array1 = "(foo bar spam eggs ham)";
	char *array2 = "(spam \"eggs ham\")";
	char **parsed;

	parsed = sh_array(array1);
	assert_string_equal(parsed[0], "foo");
	assert_string_equal(parsed[2], "spam");
	assert_string_equal(parsed[4], "ham");
	free(parsed[0]);
	free(parsed[1]);
	free(parsed[2]);
	free(parsed[3]);
	free(parsed[4]);
	free(parsed);

	parsed = sh_array(array2);
	assert_string_equal(parsed[1], "eggs ham");
	free(parsed[0]);
	free(parsed[1]);
	free(parsed);
}
