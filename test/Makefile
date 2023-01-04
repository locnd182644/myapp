CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = testLte.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
	rm *.o