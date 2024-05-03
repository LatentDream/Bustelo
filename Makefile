TARGET = bustelo
SRC_CLI = src/cli.c src/molido.c
SRC_GUI = src/gui.c src/molido.c
LIBS_CLI = -lm
LIBS_GUI = -lm -lraylib -lgdi32 -lwinmm
OBJ = bin/molido.o

all: build-cli

build-cli: $(OBJ)
	gcc -Wall -Werror -g -o bin/$(TARGET) $(SRC_CLI) $(LIBS_CLI)

build-gui: $(OBJ)
	gcc -Wall -Werror -g -o bin/$(TARGET) $(SRC_GUI) $(LIBS_GUI)

bin/molido.o: src/molido.c src/molido.h
	gcc -Wall -Werror -g -c -o $@ src/molido.c

clean:
	rm -f bin/$(TARGET) bin/$(OBJ)

run:
	bin/$(TARGET) data/avatar.png

cr:
	make build-cli && make run

gr:
	make build-gui && make run


