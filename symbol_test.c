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

/* File: symbol_test.c
Unit tests for symbol and table related functions.

See Also:
	<symbol.h>
*/

#include "cmockery.h"
#include <stdlib.h>

#include "symbol.h"
#include "symbol_private.h"

/* Function: create_symbol
A setup function to create a symbol, named "foobar".

Parameters:
	symbol - The address where the new symbol should be stored.
*/
void create_symbol(void **symbol)
{
	*symbol = (void *)symbol_new("foobar");
}

/* Function: release_symbol
A teardown function to release a symbol created with <create_symbol()>.

Parameters:
	symbol - The address where a reference to the symbol is located.
*/
void release_symbol(void **symbol)
{
	symbol_release(*symbol);
}

void test_symbol_new_retain_release(void **state)
{
	symbol_t *symbol;
	symbol = symbol_new("foo");
	assert_true(symbol->refcount == 1);
	symbol_retain(symbol);
	symbol_retain(symbol);
	assert_true(symbol->refcount == 3);
	symbol_release(symbol);
	symbol_release(symbol);
	assert_true(symbol->refcount == 1);
	symbol_release(symbol);
}

void test_symbol_name(void **state)
{
	symbol_t *symbol;
	symbol = symbol_new("foo");
	assert_string_equal(symbol_name(symbol), "foo");
	symbol_release(symbol);
}

void test_symbol_string(void **symbol)
{
	symbol_set_string(*symbol, "test");
	assert_string_equal(symbol_string(*symbol), "test");
}

void test_symbol_array(void **symbol)
{
	char *array[] = {"foo", "bar", "spam", "eggs", "ham", NULL};
	char **ptr;
	symbol_set_array(*symbol, array);
	ptr = symbol_array(*symbol);
	assert_string_equal(ptr[0], "foo");
	assert_string_equal(ptr[1], "bar");
	assert_string_equal(ptr[2], "spam");
	assert_string_equal(ptr[3], "eggs");
	assert_string_equal(ptr[4], "ham");
}

void test_table_new_retain_release(void **state)
{
	table_t *table;
	table = table_new();

	assert_true(table->refcount == 1);
	table_retain(table);
	table_retain(table);
	assert_true(table->refcount == 3);
	table_release(table);
	table_release(table);
	assert_true(table->refcount == 1);
	table_release(table);
}

void test_table_insert_lookup_remove(void **state)
{
	table_t *table;
	symbol_t *symbol;
	table = table_new();

	symbol = symbol_new("foo");
	symbol_set_string(symbol, "foo");
	table_insert(table, symbol);
	symbol_release(symbol);

	symbol = symbol_new("bar");
	symbol_set_string(symbol, "bar");
	table_insert(table, symbol);
	symbol_release(symbol);

	symbol = symbol_new("eggs");
	table_insert(table, symbol);
	symbol_release(symbol);

	symbol = symbol_new("ham");
	symbol_set_string(symbol, "ham");
	table_insert(table, symbol);
	symbol_release(symbol);
	
	symbol = table_lookup(table, "foo");
	assert_true(symbol != NULL);
	assert_string_equal(symbol_string(symbol), "foo");

	symbol = table_lookup(table, "bar");
	assert_true(symbol != NULL);
	assert_string_equal(symbol_string(symbol), "bar");

	symbol = table_lookup(table, "ham");
	assert_true(symbol != NULL);
	assert_string_equal(symbol_string(symbol), "ham");
	
	table_release(table);
}

void test_table_lookup_recursive(void **state)
{
	table_t *parent, *table;
	symbol_t *symbol;

	parent = table_new();
	symbol = symbol_new("ham");
	symbol_set_string(symbol, "eggs");
	table_insert(parent, symbol);
	symbol_release(symbol);

	table = table_new_with_parent(parent);
	symbol = symbol_retain(table_lookupr(table, "ham"));

	table_release(table);
	table_release(parent);

	assert_true(symbol != NULL);
	assert_string_equal(symbol_string(symbol), "eggs");
	symbol_release(symbol);
}
