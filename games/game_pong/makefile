CC = gcc
CFLAGS = -Wall -g

FF_LIB := $(shell pkg-config --libs firefly)
FF_CFLAGS := $(shell pkg-config --cflags firefly)

.PHONY: prepare
prepare:
	rm -rf out
	-mkdir out

game_pong: src/game_pong.c src/ball.c src/paddle.c
	$(CC) $(CFLAGS) $(FF_CFLAGS) -o $@ $^ $(FF_LIB)
	mv game_pong out

run:
	cd out && ./game_pong