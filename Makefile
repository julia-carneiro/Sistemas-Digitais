CC = gcc
CFLAGS = -std=c99 -pthread -lintelfpgaup

all: 
	$(CC) main.c -o main $(CFLAGS)
	sudo ./main

