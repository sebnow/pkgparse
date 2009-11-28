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
	#include "utility.h"

	#define YYSTYPE char *

	extern FILE *yyin;
	extern int yydebug;
	extern int line;

	static void _handle_assignment(char *lvalue, char *rvalue);
	static void _set_splitpkgs_from_table(pkgbuild_t *pkgbuild, table_t *table);
	static void _set_pkgbuild_fields_from_table(pkgbuild_t *pkgbuild, table_t *g_table);
	static void _enter_function(char *name);
	static void _exit_function();

	/* TODO: Make this local somehow. */
	table_t *g_table;
%}

%token NAME
%token NEWLINE
%token ASSIGNMENT
%token IF THEN ELSE ELIF FI

%start compound_list

%%

compound_command: brace_group
	| if_clause
	;

command: NAME ASSIGNMENT { _handle_assignment($1, $2); free($1); free($2); }
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

function_declaration : NAME '(' ')' { _enter_function($1); free($1); }

function_definition : function_declaration whitespace linebreak function_body
	;

function_body: compound_command { _exit_function(); }
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

static void _set_splitpkgs_from_table(pkgbuild_t *pkgbuild, table_t *table)
{
	#define FNAME_LENTH 32
	int i;
	size_t size = 0;
	symbol_t *symbol;
	pkgbuild_t **splitpkgs;
	char *ptr;
	char **names;
	char function_name[FNAME_LENTH] = {'\0'};
	const char *function_prefix = "package_";

	if(pkgbuild == NULL) {
		return;
	}

	ptr = function_name;
	names = pkgbuild_names(pkgbuild);
	if(names == NULL) {
		return;
	}
	for(i = 0; names[i] != NULL; i++) {
		size++;
	}

	splitpkgs = malloc(sizeof(*splitpkgs) * (size + 1));
	splitpkgs = memset(splitpkgs, 0, sizeof(*splitpkgs) * (size + 1));
	splitpkgs[size] = NULL;

	for(i = 0; names[i] != NULL; i++) {
		ptr = strncat(ptr, function_prefix, FNAME_LENTH);
		ptr = strncat(ptr, names[i], FNAME_LENTH);
		symbol = symbol_retain(table_lookup(table, ptr));
		ptr[0] = '\0';
		if(symbol != NULL) {
			splitpkgs[i] = pkgbuild_new();
			_set_pkgbuild_fields_from_table(splitpkgs[i], symbol_function(symbol));
		}
		symbol_release(symbol);
	}

	pkgbuild_set_splitpkgs(pkgbuild, splitpkgs);
}

static void _set_pkgbuild_fields_from_table(pkgbuild_t *pkgbuild, table_t *table)
{
	symbol_t *symbol;
	char **str_array = NULL;
	table_retain(table);

	symbol = table_lookup(table, "pkgname");
	if(symbol != NULL) {
		if(symbol_type(symbol) == kSymbolTypeArray) {
			pkgbuild_set_names(pkgbuild, symbol_array(symbol));
		} else {
			str_array = malloc(sizeof(*str_array) * 2);
			str_array[0] = symbol_string(symbol);
			str_array[1] = NULL;
			pkgbuild_set_names(pkgbuild, str_array);
			free(str_array);
		}
	}

	symbol = table_lookup(table, "pkgbase");
	if(symbol != NULL) {
		pkgbuild_set_basename(pkgbuild, symbol_string(symbol));
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

	symbol = table_lookup(table, "license");
	if(symbol != NULL) {
		pkgbuild_set_licenses(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "install");
	if(symbol != NULL) {
		pkgbuild_set_install(pkgbuild, symbol_string(symbol));
	}

	symbol = table_lookup(table, "source");
	if(symbol != NULL) {
		pkgbuild_set_sources(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "noextract");
	if(symbol != NULL) {
		pkgbuild_set_noextract(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "md5sums");
	if(symbol != NULL) {
		pkgbuild_set_md5sums(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "sha1sums");
	if(symbol != NULL) {
		pkgbuild_set_sha1sums(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "sha256sums");
	if(symbol != NULL) {
		pkgbuild_set_sha256sums(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "sha384sums");
	if(symbol != NULL) {
		pkgbuild_set_sha384sums(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "sha512sums");
	if(symbol != NULL) {
		pkgbuild_set_sha512sums(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "groups");
	if(symbol != NULL) {
		pkgbuild_set_groups(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "arch");
	if(symbol != NULL) {
		pkgbuild_set_architectures(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "backup");
	if(symbol != NULL) {
		pkgbuild_set_backup(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "depends");
	if(symbol != NULL) {
		pkgbuild_set_depends(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "makedepends");
	if(symbol != NULL) {
		pkgbuild_set_makedepends(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "optdepends");
	if(symbol != NULL) {
		pkgbuild_set_optdepends(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "conflicts");
	if(symbol != NULL) {
		pkgbuild_set_conflicts(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "provides");
	if(symbol != NULL) {
		pkgbuild_set_provides(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "replaces");
	if(symbol != NULL) {
		pkgbuild_set_replaces(pkgbuild, symbol_array(symbol));
	}

	symbol = table_lookup(table, "options");
	if(symbol != NULL) {
		pkgbuild_set_options(pkgbuild, symbol_array(symbol));
	}

	_set_splitpkgs_from_table(pkgbuild, table);
	table_release(table);
}

static void _handle_assignment(char *lvalue, char *rvalue)
{
	symbol_t *symbol;
	char *str;
	char **array;
	char **array_ptr;

	symbol = symbol_new(lvalue);
	/* Are we assigning an array or string? */
	if(*rvalue == '(') {
		array = sh_parse_array(g_table, rvalue);
		symbol_set_array(symbol, array);
		if(array != NULL) {
			for(array_ptr = array; *array_ptr != NULL; array_ptr++) {
				free(*array_ptr);
			}
			free(array);
		}
	} else {
		str = sh_parse_word(g_table, rvalue);
		symbol_set_string(symbol, str);
		free(str);
	}
	table_insert(g_table, symbol);
	symbol_release(symbol);
}

static void _enter_function(char *name)
{
	table_t *table;
	symbol_t *symbol;

	table = table_new_with_parent(g_table);

	symbol = symbol_new(name);
	symbol_set_function(symbol, table);
	table_insert(g_table, symbol);

	table_release(g_table);
	g_table = table;
}

static void _exit_function()
{
	table_t *table;

	table = table_retain(table_parent(g_table));
	table_release(g_table);
	g_table = table;

}

pkgbuild_t *pkgbuild_parse(FILE *fp)
{
	pkgbuild_t *pkgbuild = NULL;
#if DEBUG
		yydebug = 1;
#endif

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
