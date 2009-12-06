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

#ifndef UTILITY_H
#define UTILITY_H

/* File: utility.h

A collection of utility functions, mainly dealing with parsing shell strings.
*/

#include "symbol.h"

/* Function sh_split_array

Split a shell array into an array of strings.

The array, and each string is dynamically allocated and * should be freed
by the user.

Parameters:
	string - The string containing an array. It should be in the format
       "(a b c)"

Returns:
	A NULL-terminated array of strings

See Also:
	<sh_unquote()>
*/
char **sh_split_array(char *string);

/* Function: sh_unquote

Remove quotes and unescape escaped quotes.

A string such as '"foo \"bar\""' will be unquoted to produce 'foo "bar"'.

Example:
	(start code)
	char *text = "\"foo \"bar\"\"";
	char *unquoted = sh_unquote(text);
	printf("%s\n", unquoted); // output: foo "bar"
	free(unquoted);
	(end)

Parameters:
	string - The string to be unquoted

Returns:
	A pointer to a new string on success, otherwise a NULL pointer. The
	returned string must be deallocated by the caller.
*/
char *sh_unquote(char *string);

/* Function: sh_parse_array

Parse a shell array into a native array of strings. Each element is further
parsed using <sh_parse_word>.

Example:
	(start code)
	char *shstr = "(foo bar baz)"
	char **str_array = sh_parse_array(shstr);

	if(str_array != NULL) {
		// Output: str_array[0] = "foo"
		printf("str_array[0] = \"%s\"\n", str_array[0]);
		// Output: str_array[1] = "bar"
		printf("str_array[1] = \"%s\"\n", str_array[1]);
		// Output: str_array[2] = "baz"
		printf("str_array[2] = \"%s\"\n", str_array[2]);

		free(str_array[0]);
		free(str_array[1]);
		free(str_array[2]);
		free(str_array);
	}
	(end)

Parameters:
	table - A symbol table used for word substitution.
	string - The string representation of an array to be parsed.

Returns:
	A native array of strings on success, otherwise a NULL pointer. The
	returned array, and its contained strings, must be deallocated by the
	caller.
*/
char **sh_parse_array(table_t *table, char *string);

/* Function: sh_parse_word

Normalize a shell string and substitute variables with their values.

Example:
	(start code)
	table_t *table = table_new();
	symbol_t *symbol = symbol_new("foo");
	char *text = "foo$foo";
	char *result;

	symbol_set_string(symbol, "bar");
	table_insert(table, symbol);
	result = sh_parse_word(table, text);
	printf("%s\n", result); // output: foobar

	free(result);
	symbol_release(symbol);
	table_release(table);
	(end)

Parameters:
	table - A symbol table used for word substitution.
	string - The string to be parsed.

Returns:
	A parsed string on success, otherwise NULL. The returned string must
	be deallocated by the caller.
*/
char *sh_parse_word(table_t *table, char *string);

#endif
