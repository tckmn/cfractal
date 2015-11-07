OBJECTS = $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

.PHONY: all clean

all: bin/fractal

bin/%.o: src/%.c $(HEADERS)
	gcc -g -O0 -Wall -Wpedantic -Wextra -c $< -o $@ -std=c99

bin/fractal: $(OBJECTS)
	gcc -g -O0 -Wall -Wpedantic -Wextra $^ -o $@ -lm -std=c99

clean:
	-rm -f bin/*
