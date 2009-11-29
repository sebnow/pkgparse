%token_prefix TOKEN_
%token_type token_type_t
%extra_argument { parser_state_t *state }

%include {
}

command_list ::= command.
command_list ::= command_list command EOF.

command ::= assignment.

assignment ::= NAME(n) EQUAL VALUE(s). {
	state->symbol = symbol_new(n);
	symbol_set_string(s);
	table_insert(state->namespace, state->symbol);
	symbol_release(state->symbol);
	state->symbol = NULL;
}
