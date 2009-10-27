OBJ = pkgbuild.o symbol.o

all: $(OBJ)

clean:
	$(RM) $(OBJ)

.PHONY: clean