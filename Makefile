TARGET = bustelo
SRC = src/main.c src/molido.c
LIBS = -lm
OBJ = bin/molido.o

all: build

build: $(OBJ)
	gcc -Wall -Werror -g -o bin/$(TARGET) $(SRC) $(LIBS)

bin/molido.o: src/molido.c src/molido.h
	gcc -Wall -Werror -g -c -o $@ src/molido.c

clean:
	rm -f bin/$(TARGET) bin/$(OBJ)

run:
	bin/$(TARGET)

br:
	make build && make run

