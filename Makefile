CC = gcc
CFLAGS = -O3 -std=c11 -Wall -Iinclude
LDFLAGS = -lm

CORE_SRC = src/field.c src/kernels.c src/invariants.c src/io.c

all: field_engine field_live

field_engine: src/main.c $(CORE_SRC)
	$(CC) $(CFLAGS) -o field_engine src/main.c $(CORE_SRC) $(LDFLAGS)

field_live: src/live.c src/field.c src/kernels.c src/invariants.c
	$(CC) $(CFLAGS) -o field_live src/live.c src/field.c src/kernels.c src/invariants.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f field_engine field_live src/*.o