CC=gcc
CFlags = -I include/, -WWALL, -g, -Wextra,
DEPS = hellomake.h 
OBJ = hellomake.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: hellomake.o 
	$(CC) -o hellomake hellomake.o 
