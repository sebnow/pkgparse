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
	#include <stdio.h>
	#include <string.h>

	#define YYSTYPE char *
	#include "y.tab.h"

	extern YYSTYPE yylval;
	extern int yyleng;

	int line = 1;
%}

%%

"#"[^\n]*"\n" ;
^[\t ]+ ;

"if [" { return IF; }
"then" { return THEN; }
"else" { return ELSE; }
"elif" { return ELIF; }
"fi" { return FI; }

=[^#\n]* {
	yylval = strdup(yytext + 1);
	return ASSIGNMENT;
}

[a-zA-Z_][a-zA-Z0-9_-]* {
	yylval = strdup(yytext);
	return NAME;
}

"\n" { line++; return NEWLINE; }

. { return yytext[0]; }

%%

void yyerror(char *msg)
{
	fprintf(stderr, "ERROR:%d: %s\n", line, msg);
}

int yywrap() {
	return 1;
}
