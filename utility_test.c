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

/* File: utility_test.h
Unit tests for utility functions.

See Also:
	<utility.h>
*/

#include "cmockery.h"
#include <stdlib.h>
#include <string.h>

#include "utility.h"

/* Function: create_table
Setup function to create a table with test data

The following symbols are defined:
	- foo => foobar
	- ham => eggs and ham
	- _eggs => chickens

Parameters:
	table - A reference to the address where the table should be stored.
*/
void create_table(void **table)
{
	symbol_t *symbol;
	*table = table_new();

	symbol = symbol_new("foo");
	symbol_set_string(symbol, "foobar");
	table_insert(*table, symbol);
	symbol_release(symbol);

	symbol = symbol_new("ham");
	symbol_set_string(symbol, "eggs and ham");
	table_insert(*table, symbol);
	symbol_release(symbol);

	symbol = symbol_new("_eggs");
	symbol_set_string(symbol, "chickens");
	table_insert(*table, symbol);
	symbol_release(symbol);
}

/* Function: table_release
A teardown function to release a table created in create_table()

Parameters:
	table - A reference to the address where the table is stored
*/
void release_table(void **table)
{
	table_release(*table);
}

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

void test_split_array(void **state)
{
	char *array1 = "(foo bar spam eggs ham)";
	char *array2 = "(spam \"eggs ham\")";
	char **parsed;

	parsed = sh_split_array(array1);
	assert_string_equal(parsed[0], "foo");
	assert_string_equal(parsed[2], "spam");
	assert_string_equal(parsed[4], "ham");
	free(parsed[0]);
	free(parsed[1]);
	free(parsed[2]);
	free(parsed[3]);
	free(parsed[4]);
	free(parsed);

	parsed = sh_split_array(array2);
	assert_string_equal(parsed[1], "\"eggs ham\"");
	free(parsed[0]);
	free(parsed[1]);
	free(parsed);
}

/* TODO: What is expected if an invalid expansion like '${foo' is attempted?
 * Bash returns a "bad substitution" error. This behaviour has to be
 * implemented in the lexer. */
void test_sh_parse_array_simple_expanded(void **table)
{
	char *array = "(${foo} spam \"green $ham\")";
	char **parsed;
	char **ptr;
	
	parsed = sh_parse_array(*table, array);
	assert_true(parsed != NULL);
	assert_string_equal(parsed[0], "foobar");
	assert_string_equal(parsed[1], "spam");
	assert_string_equal(parsed[2], "green eggs and ham");

	for(ptr = parsed; *ptr != NULL; ptr++) {
		free(*ptr);
	}
	free(parsed);
}
