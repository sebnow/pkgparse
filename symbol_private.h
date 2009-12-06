#ifndef SYMBOL_PRIVATE_H
#define SYMBOL_PRIVATE_H

#include "symbol.h"

#define TABLE_SIZE 64

struct _table_t {
	unsigned int refcount;
	symbol_t *symbols[TABLE_SIZE];
	table_t *parent;
};

struct _symbol_t {
	unsigned int refcount;
	char *lvalue;
	symbol_type_t type;
	union value {
		char *strval;
		char **array; /* NULL terminated */
		table_t *function;
	} rvalue;
};

#endif