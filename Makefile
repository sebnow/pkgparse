OBJ = pkgbuild.o symbol.o sh_utility.o
TEST_OBJ = sh_utility_test.o test_runner.o

all: $(OBJ)

test_runner: LDFLAGS += -lcmockery
test_runner: $(OBJ) $(TEST_OBJ)
	$(LINK.c) -o $@ $^

test: test_runner
	@./$<

clean-test:
	$(RM) $(TEST_OBJ) test_runner

clean: clean-test
	$(RM) $(OBJ)

.PHONY: clean test clean-test
