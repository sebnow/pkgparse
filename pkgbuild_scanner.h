typedef int token_type_t;

typedef struct {
	token_type_t id;
	char *value;
} token_t;

typedef struct {
	int refcount;
	int cs;
	int act;
	char *ts;
	char *te;
	char *p;
	char *eof;
	FILE *fp;
} scanner_t;