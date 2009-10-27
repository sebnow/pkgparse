OBJ = pkgbuild.o

all: $(OBJ)

clean:
	$(RM) $(OBJ)

.PHONY: clean