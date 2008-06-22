FLAGS = -ggdb -ansi -pedantic -Wall
IMGFLAGS = -ljpeg -lgif -lpng -lm

OBJ = bplus.o

tp5: main.c $(OBJ)
	gcc $(FLAGS) $(IMGFLAGS) main.c $(OBJ) -o tp5

bplus.o: bplus.h bplus.c
	gcc $(FLAGS) -c bplus.c 
	
clean:
	rm -f *.o *~ tp5

del:
	rm -f *.av *.ch* *.dsc *.flh *.bid *.nod
	
all:
	${MAKE} tp5

ef: main.c avail.o hash.o leitura.o catalogo.o menu.o sk.o busca.o remove.o libimg.o descritor.o
	gcc $(FLAGS) $(IMGFLAGS) -lefence main.c avail.o hash.o leitura.o sk.o catalogo.o menu.o busca.o remove.o descritor.o libimg.o -o tp4

efence:
	${MAKE} ef
