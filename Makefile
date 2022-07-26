vpath %.o bin
vpath %.c src
vpath %.h include
CFLAGS = -Wextra -Wall -Wformat=2 -std=c11 -g -W

OBJS = main.o
SRCS = main.c

main : main.o 
	gcc $(CFLAGS) $(addprefix bin/, $(OBJS)) -o maketest

main.o : main.c
	gcc -c $(addprefix src/, $(SRCS)) -o bin/main.o






