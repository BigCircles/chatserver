vpath %.o bin
vpath %.c src
vpath %.h include
CFLAGS = -Wextra -Wall -Wformat=2 -std=c11 -g -W

OBJS = main.o ircsocket.o peer_packet.o
SRCS = main.c ircsocket.c peer_packet.c

main : main.o ircsocket.o peer_packet.o
	gcc $(CFLAGS) $(addprefix bin/, $(OBJS)) -o main 

main.o : main.c
	gcc -c $(addprefix src/, main.c) -o bin/main.o

ircsocket.o : ircsocket.c ircsocket.h
	gcc -c $(addprefix src/, ircsocket.c) -o bin/ircsocket.o

peer_packet.o: peer_packet.c peer_packet.h
	gcc -c $(addprefix src/, peer_packet.c) -o bin/peer_packet.o

.PHONY: clean
run:
	./main






