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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sh_utility.h"

static size_t _array_size(const char *array)
{
	const char *str_ptr = array;
	char quote_char;
	int in_quote = 0;
	size_t count = 0;
	int end_of_array;

	/* Skip left parenthesis, otherwise attempt to count elements anyway */
	if(*str_ptr == '(') {
		str_ptr++;
	}

	end_of_array = *array == ')' || *array == '\0';

	while(!end_of_array) {
		switch(*str_ptr) {
			/* Entering or exitting quote */
			case '\'':
			case '"':
				if(*(str_ptr - 1) != '\\') {
					in_quote = !(in_quote && *str_ptr == quote_char);
					quote_char = *str_ptr;
				}
				break;
			/* Element separator, if not in quote */
			case ' ':
			case '\t':
			case '\n':
				if(!in_quote && *(str_ptr - 1) != '\\') {
					count++;
				}
				break;
			case ')':
				if(!in_quote && *(str_ptr - 1) != '\\') {
					count++;
					end_of_array = 1;
				}
				break;
			case '\0':
				end_of_array = 1;
				break;
			default:
				break;
		}
		str_ptr++;
	}
	return count;
}

char **sh_array(char *string)
{
	size_t count_elem;
	int in_quote = 0;
	unsigned int elem;
	char quote_char = '\0';
	char *str_cpy;
	char *str_ptr;
	char *start_ptr;
	char **array;
	int end_of_array;

	/* Copy string as we will be mutating it */
	str_cpy = strdup(string);

	count_elem = _array_size(str_cpy);

	array = malloc((count_elem + 1) * sizeof(*array));
	array[count_elem] = NULL;
	elem = 0;

	end_of_array = count_elem == 0;

	/* Skip the left parenthesis */
	str_ptr = str_cpy + 1;
	start_ptr = str_ptr;
	/* Loop until end of string, and extract the elements */
	while(!end_of_array) {
		switch(*str_ptr) {
			/* Entering or exitting quote */
			case '\'':
			case '"':
				if(*(str_ptr - 1) != '\\') {
					in_quote = !(in_quote && *str_ptr == quote_char);
					quote_char = *str_ptr;
				}
				break;
			/* Element separator, if not in quote */
			case ' ':
			case '\t':
			case '\n':
				if(!in_quote && *(str_ptr - 1) != '\\') {
					*str_ptr = '\0';
					/* Skip multiple spaces */
					if(strlen(start_ptr) != 1 && !isspace(*start_ptr)) {
						array[elem] = sh_unquote(start_ptr);
						elem++;
					}
					start_ptr = str_ptr + 1;
				}
				break;
			case ')':
				if(!in_quote && *(str_ptr - 1) != '\\') {
					*str_ptr = '\0';
					array[elem] = sh_unquote(start_ptr);
					end_of_array = 1;
				}
				break;
			case '\0':
				end_of_array = 1;
				break;
			default:
				break;
		}
		str_ptr++;
	}

	free(str_cpy);

	return array;
}

/* TODO: Unquote strings in the middle of a string, e.g.
 * "foo'bar baz'zer" */
char *sh_unquote(char *string)
{
	size_t len = 0;
	char *str_ptr = string;
	char *return_string = NULL;

	if(*str_ptr == '\'' || *str_ptr == '"') {
		str_ptr++;
		len = strlen(str_ptr);
		return_string = malloc(len * sizeof(*string));
		return_string = strncpy(return_string, str_ptr, len - 1);
		return_string[len - 1] = '\0';
	} else {
		/* Since we allocate when the string is quoted, we should do so
         * when it's not for consistency. */
		return_string = strdup(string);
	}

	return return_string;
}
