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

#ifndef SYMBOL_PRIVATE_H
#define SYMBOL_PRIVATE_H

/* File: symbol_private.h
An internal header file to the project. It is used so that other files can
have access to the <table_t> and <symbol_t> data structures directly.

See Also:
	<symbol.h>
*/

#include "symbol.h"

/* The maximum table size. This should never be changed. If such a need
arises, the data structure should be changed instead. */
#define TABLE_SIZE 64

struct _table_t {
	/* The amount of references held for this table */
	unsigned int refcount;
	/* TODO: Use a more appropriate data structure, such as a hash table. */
	symbol_t *symbols[TABLE_SIZE];
	/* Reference to a parent "namespace" */
	table_t *parent;
};

struct _symbol_t {
	/* The amount of references held to this object */
	unsigned int refcount;
	/* The name of the symbol */
	char *lvalue;
	symbol_type_t type;
	union value {
		char *strval;
		char **array; /* NULL terminated */
		table_t *function;
	} rvalue;
};

#endif