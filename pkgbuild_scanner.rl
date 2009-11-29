#include "utility.h"
#include "pkgbuild_parse.h"

%%{
	machine Shell;
	access scanner->;
	variable p = scanner->p;
	variable pe = scanner->pe;
	variable eof = scanner->eof;

	action mark {
		marker = fpc;
	}

	action push_name {
		token.id = TOKEN_NAME;
		token.value = strcpy_partial(marker, marker, fpc);
		fbreak;
	}

	name = (alpha | '_')(alnum | '_' | '-')*;

	main := |*
		name => >mark @push_name;
		"=" { token.id = TOKEN_EQUAL; fbreak; };
		0 => { token.id = TOKEN_EOF; fbreak; }
	*|;
	write data;
}%%


scanner_t *scanner_new_with_file(FILE *fp)
{
	scanner_t *scanner;
	scanner = malloc(sizeof(*scanner));
	if(scanner != NULL) {
		%% write init;
		scanner = memset(scanner, 0, sizeof(*scanner));
		scanner_retain(scanner);
	}
	return scanner;
}

scanner_t *scanner_new_with_string(const char *string)
{
	scanner_t *scanner;
	scanner = malloc(sizeof(*scanner));
	if(scanner != NULL) {
		%% write init;
		scanner = memset(scanner, 0, sizeof(*scanner));
		scanner_retain(scanner);
	}
	return scanner;
}

scanner_t *scanner_retain(scanner_t *scanner)
{
	if(scanner != NULL) {
		scanner->refcount++;
	}
	return scanner;
}

int scanner_next_token(scanner_t *scanner, token_t *token)
{
	const char *buf = "foo=bar";
	char *marker = NULL;

	assert(scanner != NULL);

	scanner->p = buf;
	scanner->eof = 0;

	token.value = NULL;

	%% write exec;
}

void scanner_release(scanner_t *scanner)
{
	free(scanner);
}
