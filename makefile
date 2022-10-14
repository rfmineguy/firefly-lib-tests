CC = gcc
CFLAGS = -Wall -g
TESTS = tests


#make test_refactor
#make test_<filename>
%: src/tests/%.c
	$(CC) $(CFLAGS) $^ -lfirefly

#make game_pong
#make game_<filename>
%: src/games/$@/%.c
	$(CC) $(CFLAGS) -o $@ $^ -lfirefly

valgrind:
	valgrind --tool=memcheck ./a.out

leaks:
	leaks -atExit -- ./a.out

run:
	./a.out
