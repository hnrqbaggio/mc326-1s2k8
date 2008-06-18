FLAGS = -ggdb -ansi -pedantic -Wall
IMGFLAGS = -ljpeg -lgif -lpng -lm

OBJ = avail.o hash.o leitura.o catalogo.o menu.o sk.o busca.o remove.o libimg.o descritor.o bplus.o index.o

tp5: main.c $(OBJ)
	gcc $(FLAGS) $(IMGFLAGS) main.c $(OBJ) -o tp5

leitura.o: leitura.h leitura.c
	gcc $(FLAGS) -c leitura.c 

sk.o: sk.h sk.c
	gcc $(FLAGS) -c sk.c

catalogo.o: catalogo.h catalogo.c
	gcc $(FLAGS) -c catalogo.c

menu.o: menu.h menu.c
	gcc $(FLAGS) -c menu.c 

busca.o: busca.h busca.c
	gcc $(FLAGS) -c busca.c

remove.o: remove.h remove.c
	gcc $(FLAGS) -c remove.c

avail.o: avail.h avail.c
	gcc $(FLAGS) -c avail.c

hash.o: hash.h hash.c
	gcc $(FLAGS) -c hash.c

descritor.o: descritor.h descritor.c
	gcc $(FLAGS) -c descritor.c

libimg.o: libimg.h libimg.c
	gcc $(FLAGS) -c libimg.c 
	
bplus.o: bplus.h bplus.c
	gcc $(FLAGS) -c bplus.c
	
index.o: index.h index.c
	gcc $(FLAGS) -c index.c
	
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
