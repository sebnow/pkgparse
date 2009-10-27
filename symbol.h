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

#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
	kSymbolTypeInteger,
	kSymbolTypeString,
	kSymbolTypeArray,
} symbol_type_t;

typedef struct _table_t table_t;
typedef struct _symbol_t symbol_t;

symbol_t *symbol_new(char *lvalue);
symbol_t *symbol_retain(symbol_t *symbol);
void symbol_release(symbol_t *symbol);

void symbol_set_integer(symbol_t *symbol, int rvalue);
void symbol_set_string(symbol_t *symbol, char *rvalue);
void symbol_set_array(symbol_t *symbol, char **rvalue);

char *symbol_name(symbol_t *symbol);
symbol_type_t symbol_type(symbol_t *symbol);
int symbol_integer(symbol_t *symbol);
char *symbol_string(symbol_t *symbol);
char **symbol_array(symbol_t *symbol);

table_t *table_new();
table_t *table_retain(table_t *table);
void table_release(table_t *table);

symbol_t *table_lookup(table_t *table, char *lvalue);
int table_insert(table_t *table, symbol_t *symbol);
int table_remove(table_t *table, char *lvalue);

#endif
