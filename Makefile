FLAGS = -ggdb -ansi -pedantic -Wall

OBJ = bplus.o btree.o

tp5: main.c $(OBJ)
	gcc $(FLAGS) main.c $(OBJ) -o tp5

bplus.o: bplus.h bplus.c
	gcc $(FLAGS) -c bplus.c
	
btree.o: btree.h btree.c
	gcc $(FLAGS) -c btree.c
	
clean:
	rm -f *.o *~ tp5

del:
	rm -f *.bplus
	
all:
	${MAKE} tp5