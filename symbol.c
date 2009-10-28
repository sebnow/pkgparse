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

#include "symbol.h"
#include "symbol_private.h"

static void _table_free(table_t *table)
{
	int i;
	for(i = 0; i < TABLE_SIZE; i++) {
		if(table->symbols[i] != NULL) {
			symbol_release(table->symbols[i]);
		}
	}
	free(table);
}

table_t *table_new()
{
	table_t *table;
	table = malloc(sizeof(*table));
	table = memset(table, 0, sizeof(*table));
	return table_retain(table);
}

table_t *table_retain(table_t *table)
{
	if(table != NULL) {
		table->refcount++;
	}
	return table;
}

void table_release(table_t *table)
{
	if(table != NULL) {
		table->refcount--;
		if(table->refcount == 0) {
			_table_free(table);
		}
	}
}

symbol_t *table_lookup(table_t *table, char *lvalue)
{
	symbol_t *symbol = NULL;
	int found = 0;
	int i;
	for(i = 0; i < TABLE_SIZE && found == 0; i++) {
		symbol = table->symbols[i];
		if(symbol != NULL && strcmp(symbol->lvalue, lvalue) == 0) {
			found = 1;
		}
	}
	return symbol;
}

int table_insert(table_t *table, symbol_t *symbol)
{
	int i;
	for(i = 0; i < TABLE_SIZE; i++) {
		if(table->symbols[i] == NULL) {
			table->symbols[i] = symbol_retain(symbol);
			return 1;
		}
	}
	return 0;
}

int table_remove(table_t *table, char *lvalue)
{
	int i;
	for(i = 0; i < TABLE_SIZE; i++) {
		if(table->symbols[i] != NULL) {
			if(strcmp(table->symbols[i]->lvalue, lvalue) == 0) {
				symbol_release(table->symbols[i]);
				table->symbols[i] = NULL;
				return 1;
			}
		}
	}
	return 0;
}

static void _symbol_free(symbol_t *symbol)
{
	char **ptr = NULL;
	free(symbol->lvalue);
	switch(symbol->type) {
		case kSymbolTypeString:
			free(symbol->rvalue.strval);
			break;
		case kSymbolTypeArray:
			for(ptr = symbol->rvalue.array; *ptr != NULL; ptr++) {
				free(*ptr);
			}
			ptr = NULL;
			free(symbol->rvalue.array);
			break;
		default:
			break;
	}
	free(symbol);
}

symbol_t *symbol_new(char *lvalue)
{
	symbol_t *symbol;
	symbol = malloc(sizeof(*symbol));
	symbol = memset(symbol, 0, sizeof(*symbol));
	symbol->lvalue = strdup(lvalue);
	return symbol_retain(symbol);
}

symbol_t *symbol_retain(symbol_t *symbol)
{
	if(symbol != NULL) {
		symbol->refcount++;
	}
	return symbol;
}

void symbol_release(symbol_t *symbol)
{
	if(symbol != NULL) {
		symbol->refcount--;
		if(symbol->refcount == 0) {
			_symbol_free(symbol);
		}
	}
}

void symbol_set_integer(symbol_t *symbol, int rvalue)
{
	if(symbol != NULL) {
		symbol->type = kSymbolTypeInteger;
		symbol->rvalue.intval = rvalue;
	}
}

void symbol_set_string(symbol_t *symbol, char *rvalue)
{
	if(symbol != NULL) {
		symbol->type = kSymbolTypeString;
		free(symbol->rvalue.strval);
		symbol->rvalue.strval = strdup(rvalue);
	}
}

void symbol_set_array(symbol_t *symbol, char **rvalue)
{
	char **ptr;
	int i;
	if(symbol != NULL) {
		symbol->type = kSymbolTypeArray;
		if(rvalue == NULL) {
			symbol->rvalue.array = NULL;
		} else {
			/* Count elements */
			for(i = 0; rvalue[i] != NULL; i++);
			ptr = malloc((i + 1) * sizeof(*ptr));
			ptr[i] = NULL;
			for(i = 0; rvalue[i] != NULL; i++) {
				ptr[i] = strdup(rvalue[i]);
			}
			symbol->rvalue.array = ptr;
		}
	}
}

char *symbol_name(symbol_t *symbol)
{
	return symbol->lvalue;
}

symbol_type_t symbol_type(symbol_t *symbol)
{
	symbol_type_t type;
	if(symbol != NULL) {
		type = symbol->type;
	}
	return type;
}

int symbol_integer(symbol_t *symbol)
{
	int number = 0;
	if(symbol != NULL && symbol->type == kSymbolTypeInteger) {
		number = symbol->rvalue.intval;
	}
	return number;
}

char *symbol_string(symbol_t *symbol)
{
	char *string = NULL;
	if(symbol != NULL && symbol->type == kSymbolTypeString) {
		string = symbol->rvalue.strval;
	}
	return string;
}

char **symbol_array(symbol_t *symbol)
{
	char **array = NULL;
	if(symbol != NULL && symbol->type == kSymbolTypeArray) {
		array = symbol->rvalue.array;
	}
	return array;
}
