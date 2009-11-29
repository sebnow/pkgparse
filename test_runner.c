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

void test_unquote_simple_string(void **state);
void test_unquote_subsequenctly_quoted(void **state);
void test_split_array(void **state);
void test_symbol_new_retain_release(void **state);
void test_symbol_name(void **state);
void test_symbol_string(void **symbol);
void test_symbol_array(void **symbol);
void test_table_new_retain_release(void **state);
void test_table_insert_lookup_remove(void **state);
void test_table_lookup_recursive(void **state);
void test_sh_parse_array_simple_expanded(void **table);
void test_scan_simple(void **table);
void test_parse_pkgbuild_minimal(void **state);
void test_parse_pkgbuild_arrays(void **state);
void test_parse_pkgbuild_simple(void **state);
void test_parse_pkgbuild_splitpkg(void **state);

void create_symbol(void **symbol);
void release_symbol(void **symbol);
void create_table(void **symbol);
void release_table(void **symbol);

int main()
{
	const UnitTest tests[] = {
		unit_test(test_unquote_simple_string),
		unit_test(test_unquote_subsequenctly_quoted),
		unit_test(test_split_array),
		unit_test(test_symbol_new_retain_release),
		unit_test(test_symbol_name),
		unit_test_setup_teardown(test_symbol_string, create_symbol,
			release_symbol),
		unit_test_setup_teardown(test_symbol_array, create_symbol,
			release_symbol),
		unit_test(test_table_new_retain_release),
		unit_test(test_table_insert_lookup_remove),
		unit_test(test_table_lookup_recursive),
		unit_test_setup_teardown(test_sh_parse_array_simple_expanded,
			create_table, release_table),
		unit_test(test_scan_simple),
		unit_test(test_parse_pkgbuild_minimal),
		unit_test(test_parse_pkgbuild_arrays),
		unit_test(test_parse_pkgbuild_simple),
		unit_test(test_parse_pkgbuild_splitpkg),
	};
	return run_tests(tests);
}
