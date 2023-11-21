CC=g++
CFLAGS= -std=c++20 -O3

main: main.cpp
	$(CC) $(CFLAGS) $<
