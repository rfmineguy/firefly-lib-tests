CC = gcc
CFLAGS = -Wall -g

main: src/main.c
	$(CC) $(CFLAGS) src/main.c -lfirefly

testall: src/testall_main.c
	$(CC) $(CFLAGS) src/testall_main.c -lfirefly

testlog: src/testlog_main.c
	$(CC) $(CFLAGS) src/testlog_main.c -lfirefly

testwindow: src/testwindow_main.c
	$(CC) $(CFLAGS) src/testwindow_main.c -lfirefly

testrendering: src/testrendering_main.c
	$(CC) $(CFLAGS) src/testrendering_main.c -lfirefly

testresource: src/testresource_main.c
	$(CC) $(CFLAGS) src/testresource_main.c -lfirefly

leaks:
	leaks -atExit -- ./a.out

run:
	./a.out
