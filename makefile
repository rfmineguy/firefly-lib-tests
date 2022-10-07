CC = gcc
CFLAGS = -Wall -g -DDEBUG
TESTS = tests


#make test_refactor
#make test_<filename>
%: src/tests/%.c
	$(CC) $(CFLAGS) $^ -lfirefly

#make game_pong
#make game_<filename>
%: src/games/%.c
	$(CC) $(CFLAGS) $^ -lfirefly

#testall: src/testall_main.c
#	$(CC) $(CFLAGS) src/testall_main.c -lfirefly
#
#testlog: src/testlog_main.c
#	$(CC) $(CFLAGS) src/testlog_main.c -lfirefly
#
#testwindow: src/testwindow_main.c
#	$(CC) $(CFLAGS) src/testwindow_main.c -lfirefly
#
#testrendering: src/testrendering_main.c
#	$(CC) $(CFLAGS) src/testrendering_main.c -lfirefly
#
#testresource: src/testresource_main.c
#	$(CC) $(CFLAGS) src/testresource_main.c -lfirefly
#
#testcpp: src/testcpp.cpp
#	clang++ $(CFLAGS) src/testcpp.cpp -lfirefly
#
#test_refactor: src/test_refactor.c
#	$(CC) $(CFLAGS) src/test_refactor.c -lfirefly

valgrind:
	valgrind --tool=memcheck ./a.out

leaks:
	leaks -atExit -- ./a.out

run:
	./a.out
