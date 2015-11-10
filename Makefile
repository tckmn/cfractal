OBJECTS = $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

.PHONY: all clean

all: bin/fractal

bin/%.o: src/%.c $(HEADERS)
	gcc -O3 -Wall -Wpedantic -Wextra -c $< -o $@

bin/fractal: $(OBJECTS)
	gcc -O3 -Wall -Wpedantic -Wextra $^ -o $@ -lm

clean:
	-rm -f bin/*
