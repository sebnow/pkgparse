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

%{
	#include <stdlib.h>
	#include <string.h>

	#include "pkgbuild.h"
	#include "pkgbuild_private.h"
	#include "symbol.h"
	#include "sh_utility.h"

	#define YYSTYPE char *

	extern FILE *yyin;
	extern int line;

	static void _handle_assignment(char *string);
	static void _set_pkgbuild_fields_from_table(pkgbuild_t *pkgbuild, table_t *g_table);
	static void _strsplit(char *string, char split_by, char **left, char **right);

	/* TODO: Make these local somehow. */
	table_t *g_table;
	pkgbuild_t *g_pkgbuild;
%}

%token NAME
%token NEWLINE
%token ASSIGNMENT_WORD
%token IF THEN ELSE ELIF FI

%start compound_list

%%

compound_command: brace_group
	| if_clause
	;

command: ASSIGNMENT_WORD { _handle_assignment($1); free($1); }
	| compound_command
	| function_definition
	;

compound_list: term
	| newline_list term
	| term separator
	| newline_list term separator
	;

term: command
	| term separator command
	;

if_clause: IF compound_list THEN compound_list else_part FI
	| IF compound_list THEN compound_list FI
	;

else_part: ELIF compound_list THEN else_part
	| ELSE compound_list
	;

function_definition : NAME '(' ')' whitespace linebreak function_body
	;

function_body: compound_command
	;

brace_group: '{' compound_list '}'
	;

newline_list: NEWLINE
	| newline_list NEWLINE
	;

linebreak: newline_list
	| /* empty */
	;

separator: ';' linebreak
	| newline_list
	;

whitespace: whitespace ' '
	| whitespace '\t'
	|
	;

%%

static void _set_pkgbuild_fields_from_table(pkgbuild_t *pkgbuild, table_t *table)
{
	symbol_t *symbol;

	symbol = table_lookup(table, "pkgname");
	if(symbol != NULL) {
		pkgbuild_set_name(pkgbuild, symbol_string(symbol));
	}

	symbol = table_lookup(table, "pkgver");
	if(symbol != NULL) {
		pkgbuild_set_version(pkgbuild, symbol_string(symbol));
	}

	symbol = table_lookup(table, "pkgrel");
	if(symbol != NULL) {
		/* FIXME: Why doesn't it work with atoif()? */
		pkgbuild_set_rel(pkgbuild, atoi(symbol_string(symbol)));
	}

	symbol = table_lookup(table, "pkgdesc");
	if(symbol != NULL) {
		pkgbuild_set_desc(pkgbuild, symbol_string(symbol));
	}

	symbol = table_lookup(table, "url");
	if(symbol != NULL) {
		pkgbuild_set_url(pkgbuild, symbol_string(symbol));
	}

	symbol = table_lookup(table, "install");
	if(symbol != NULL) {
		pkgbuild_set_install(pkgbuild, symbol_string(symbol));
	}
}

static void _strsplit(char *string, char split_by, char **left, char **right)
{
	char *str_ptr;

	string = strdup(string);
	str_ptr = strchr(string, split_by);
	*str_ptr = '\0';
	*left = strdup(string);
	str_ptr++;
	*right = strdup(str_ptr);
	free(string);
}

static void _handle_assignment(char *string)
{
	symbol_t *symbol;
	char *lvalue;
	char *rvalue;
	char *unquoted;
	char **array;
	char **array_ptr;

	/* We are guaranteed to find an equals sign */
	_strsplit(string, '=', &lvalue, &rvalue);

	symbol = symbol_new(lvalue);

	/* Are we assigning an array or string? */
	if(*rvalue == '(') {
		array = sh_array(rvalue);
		symbol_set_array(symbol, array);
		if(array != NULL) {
			for(array_ptr = array; *array_ptr != NULL; array_ptr++) {
				free(*array_ptr);
			}
			free(array);
		}
	} else {
		unquoted = sh_unquote(rvalue);
		symbol_set_string(symbol, unquoted);
		free(unquoted);
	}
	table_insert(g_table, symbol);
	symbol_release(symbol);
	free(lvalue);
	free(rvalue);
}


pkgbuild_t *pkgbuild_parse(FILE *fp)
{
	pkgbuild_t *pkgbuild = NULL;

	if(fp != NULL) {
		g_table = table_new();
		fseek(fp, 0, SEEK_SET);
		yyin = fp;
		yyparse();

		pkgbuild = pkgbuild_new();
		_set_pkgbuild_fields_from_table(pkgbuild, g_table);

		table_release(g_table);
		g_table = NULL;
	}

	return pkgbuild;
}
