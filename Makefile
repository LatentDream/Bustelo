TARGET = bustelo
SRC = main.c
LIBS = -lm


build:
	gcc -Wall -Werror -g -o bin/$(TARGET) src/$(SRC) $(LIBS)

clean:
	rm -f bin/$(TARGET)

run:
	bin/$(TARGET)

br:
	make build && make run
