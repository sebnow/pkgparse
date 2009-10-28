DEBUG ?= 0
OBJ = pkgbuild.o symbol.o utility.o y.tab.o lex.yy.o
TEST_OBJ = utility_test.o symbol_test.o pkgbuild_test.o test_runner.o

YFLAGS += -d

ifneq ($(DEBUG), 0)
	CFLAGS += -g -DDEBUG
	YFLAGS += -t
	LFLAGS += -d
endif

all: $(OBJ)

test_runner: LDFLAGS += -lcmockery
test_runner: $(OBJ) $(TEST_OBJ)
	$(LINK.c) -o $@ $^

test: test_runner
	@./$<

clean-test:
	$(RM) $(TEST_OBJ) test_runner y.tab.h y.tab.c lex.yy.c

clean: clean-test
	$(RM) $(OBJ)

lex.yy.c: pkgbuild_scanner.l y.tab.h
	$(LEX.l) $< > $@

y.tab.c y.tab.h: pkgbuild_parse.y
	$(YACC.y) $^

y.tab.o: y.tab.c y.tab.h
	$(COMPILE.c) -o $@ $<

.PHONY: clean test clean-test
