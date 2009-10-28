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

#include "symbol.h"

/** Split \c string into two strings, separated by \split_by
 *
 * The string \string shall be split using \c split_by, resulting in the left
 * part being stored in \c left and the right part in \c right. If the \c
 * split_by character is not found, \c left and \c right will be NULL and 0
 * will be returned.
 *
 * The resulting strings \c left and \c right must be freed by the user.
 *
 * @param string the source string, to be split
 * @param split_by the separator character
 * @param left the address at which the left part shall be allocated
 * @param right the address at which the right part shall be allocated
 * @return 1 on success, 0 if \c split_by was not found.
 */
int strsplit(char *string, char split_by, char **left, char **right);

/** Split a bash array into a string array
 *
 * The array, and each string is dynamically allocated and * should be freed
 * by the user.
 *
 * @see sh_unquote()
 * @param string The string containing an array. It should be in the
 * format "(a b c)"
 * @return A NULL-terminated array of strings
 */
char **sh_split_array(char *string);

/** Remove quotes and unescape escaped quotes
 *
 * A string such as '"foo \"bar\""' will be unquoted to produce
 * 'foo "bar"'
 *
 * @param string The string to be unquoted
 * @return A pointer to a new string on success, otherwise a null pointer.
 */
char *sh_unquote(char *string);

char **sh_parse_array(table_t *table, char *string);
char *sh_parse_word(table_t *table, char *string);

#endif
