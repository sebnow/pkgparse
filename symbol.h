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

/* File: symbol.h
A collection of functions and data structures related to symbols and symbol
lookup tables.
*/

/* Enumeration: symbol_type_t
An enumeration indicating the type of a symbol.

kSymbolTypeString - Indicates a symbol with a string value.
kSymbolTypeArray - Indicates a symbol with an array value.
kSymbolTypeFunction - Indicates a symbol containing a namespace.

See Also:
	<symbol_type()>
*/
typedef enum {
	kSymbolTypeString,
	kSymbolTypeArray,
	kSymbolTypeFunction,
} symbol_type_t;

/* Type: table_t
A symbol table container.
*/
typedef struct _table_t table_t;

/* Type: symbol_t
A symbol associating a name with a value.

See Also:
	<symbol_type_t> for possible values.
*/
typedef struct _symbol_t symbol_t;

/* Constructor: symbol_new
Initialize and return a new symbol with the given lvalue. The created symbol
should be released with <symbol_release()>.

Example:
	(start code)
	symbol_t *symbol = symbol_new("foo");
	symbol_set_string(symbol, "test");
	symbol_release(symbol);
	(end)

Parameters:
	lvalue - A string representing the name of the symbol.

Returns:
	An initialized symbol with the specified lvalue, or NULL on error.
*/
symbol_t *symbol_new(char *lvalue);

/* Function: symbol_retain
Increment the symbol's reference count. This should be used whenever you want
to prevent it from being deallocated without your express permission.

The symbol is deallocated automatically when its reference count reaches 0.
Calling <symbol_retain()> increments the reference count, and calling
<symbol_release()> decrements it.

Example:
	(start code)
	void use_symbol(symbol_t *symbol)
	{
	    symbol_retain(symbol);
	    // Do something with symbol
	    symbol_release(symbol);
	}
	(end)

Parameters:
	symbol - A reference to the symbol to be retained.

Returns:
	A reference to the symbol.

See Also:
	<symbol_release()>
*/
symbol_t *symbol_retain(symbol_t *symbol);

/* Function: symbol_release

Decrement the symbol's reference count.

The symbol is deallocated automatically when its reference count reaches 0.
The associated value, if set, will also be deallocated.

Parameters:
	symbol - A reference to the symbol to be released.

See Also:
	<symbol_retain()>
*/
void symbol_release(symbol_t *symbol);

/* Function: symbol_set_string
Assign a string value to the symbol. The symbol type is changed to
<kSymbolTypeString> implicitely, to reflect the new value type.

Parameters:
	symbol - A reference to the symbol being modified.
	rvalue - A string value to be assigned to the symbol.
*/
void symbol_set_string(symbol_t *symbol, char *rvalue);

/* Function: symbol_set_array
Assign an array value to the symbol. The symbol type is changed to
<kSymbolTypeArray> implicitely, to reflect the new value type.

Parameters:
	symbol - A reference to the symbol being modified.
	rvalue - An array of strings to be assigned to the symbol.
*/
void symbol_set_array(symbol_t *symbol, char **rvalue);

/* Function: symbol_set_function
Assign a namespace to the symbol. The symbol type is changed to
<kSymbolTypeFunction> implicitely, to reflect the new value type.

Parameters:
	symbol - A reference to the symbol being modified.
	rvalue - A symbol table to be assigned to the symbol.
*/
void symbol_set_function(symbol_t *symbol, table_t *table);

/* Function: symbol_name

Retrieve a string representing the name of the symbol.

Parameters:
	symbol - A reference to the symbol whose name is to be returned.

Returns:
	The name of the symbol.
*/
char *symbol_name(symbol_t *symbol);

/* Function: symbol_type
Retrieve a constant indicating the type of the symbol.

Parameters:
	symbol - A reference to the symbol whose name is to be returned.

Returns:
	A constant indicating the type of the symbol.

See Also:
	<symbol_type_t>
*/
symbol_type_t symbol_type(symbol_t *symbol);

/* Function: symbol_string
Retrieve the string value associated with the symbol. If the symbol does not
hold a string value, NULL is returned.

Parameters:
	symbol - The symbol to retrieve the value from.

Returns:
	The string value associated with the symbol.
*/
char *symbol_string(symbol_t *symbol);

/* Function: symbol_array
Retrieve the array value associated with the symbol. If the symbol does not
hold an array value, NULL is returned.

Parameters:
	symbol - The symbol to retrieve the value from.

Returns:
	The array value associated with the symbol.
*/
char **symbol_array(symbol_t *symbol);

/* Function: symbol_function
Retrieve the namespace value associated with the symbol. If the symbol does
not hold a namespace value, NULL is returned.

Parameters:
	symbol - The symbol to retrieve the value from.

Returns:
	The namespace value associated with the symbol.
*/
table_t *symbol_function(symbol_t *symbol);

/* Constructor: table_new
Initialize and return a new table. The created table should be released with
<table_release()>.

Returns:
	An initialized table, or NULL on error.

See Also:
	<table_release()>
*/
table_t *table_new();

/* Constructor: table_new_with_parent
Initialize and return a new table with the specified table as the parent.
This is the designated constructor. The created table should be released with
<table_release()>.

Parameters:
	parent - A parent table. This is used when searching recursively.

Returns:
	An initialized table, or NULL on error.

See Also:
	<table_release()>, <table_lookupr()>
*/
table_t *table_new_with_parent(table_t *parent);

/* Function: table_retain
Increment the tables's reference count. This should be used whenever you want
to prevent it from being deallocated without your express permission.

The table is deallocated automatically when its reference count reaches 0.
Calling <table_retain> increments the reference count, and calling
<table_release> decrements it.

Example:
	(start code)
	void use_table(table_t *table)
	{
	    table_retain(table);
	    // Do something with table
	    table_release(table);
	}
	(end)

Parameters:
	table - A reference to the table to be retained.

Returns:
	A reference to the table.

See Also:
	<table_release()>
*/
table_t *table_retain(table_t *table);

/* Function: table_release
Decrement the table's reference count.

The table is deallocated automatically when its reference count reaches 0.
Any contained symbols will also be deallocated.

Parameters:
	table - A reference to the table to be released.

See Also:
	<table_release()>
*/
void table_release(table_t *table);

/* Function: table_lookup
Search for a symbol with the name lvalue. If the symbol is not found, parent
namespaces are not searched, and NULL is returned.

Parameters:
	table - A reference to the table being searched.
	lvalue - A string representing the name of the symbol to be found.

Returns:
	A symbol whose name is lvalue, or NULL if such a symbol is not
	found.
*/
symbol_t *table_lookup(table_t *table, char *lvalue);

/* Function: table_lookupr
Search recusively for a symbol with the name lvalue. If the symbol is not
found in the current namespace, the parent namespace is searched, until there
are no parent namespaces. If the symbol is not found at this point, NULL is
returned.

Example:
	(start code)
	char *value = NULL;
	symbol_t *symbol = symbol_new("foo");
	table_t *table = table_new_with_parent(parent);

	symbol_set_string(symbol, "bar");
	table_insert(parent, symbol);
	symbol_string(table_lookupr(table, "foo")); // "bar"

	table_release(symbol);
	table_release(table);
	table_release(parent);
	(end)

Parameters:
	table - A reference to the table being searched.
	lvalue - A string representing the name of the symbol to be found.

Returns:
	A symbol whose name is lvalue, or NULL if such a symbol is not
	found.
*/
symbol_t *table_lookupr(table_t *table, char *lvalue);

/* Function: table_insert
Insert a symbol into the table. The symbol will be retained until it is
either removed with <table_remove()>, or the table is deallocated with
<table_release()>.

Parameters:
	table - A reference to the table being modified.
	symbol - The symbol to be inserted.

Returns:
	True (1) on success or false (0) on error.
*/
int table_insert(table_t *table, symbol_t *symbol);

/* Function: table_remove
Remove a symbol from the table.

Parameters:
	table - A reference to the table being modified.
	lvalue - A string representing the name of the symbol.

Returns:
	True (1) on success or false (0) on error.
*/
int table_remove(table_t *table, char *lvalue);

/* Function: table_parent
Retrieve the parent namespace of the table.

Parameters:
	table - A reference to the table to retrieve the parent from.

Returns:
	The parent namespace of table.
*/
table_t *table_parent(table_t *table);

#endif
