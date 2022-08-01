vpath %.o bin
vpath %.c src
vpath %.h include
CFLAGS = -Wextra -Wall -Wformat=2 -std=c11 -g -W

OBJS = main.o ircsocket.o
SRCS = main.c ircsocket.c

main : main.o ircsocket.o
	gcc $(CFLAGS) $(addprefix bin/, $(OBJS)) -o main 

main.o : main.c
	gcc -c $(addprefix src/, main.c) -o bin/main.o

ircsocket.o : ircsocket.c
	gcc -c $(addprefix src/, ircsocket.c) -o bin/ircsocket.o

.PHONY: clean
run:
	./main






