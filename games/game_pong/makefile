CC = gcc
CFLAGS = -Wall -g

.PHONY: prepare
prepare:
	rm -rf out
	-mkdir out

game_pong: src/game_pong.c src/ball.c src/paddle.c
	$(CC) $(CFLAGS) -o $@ $^ -lfirefly
	mv game_pong out

run:
	cd out && ./game_pong