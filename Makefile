OBJ = pkgbuild.o symbol.o sh_utility.o

all: $(OBJ)

clean:
	$(RM) $(OBJ)

.PHONY: clean