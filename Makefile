FLAGS = -ggdb -ansi -pedantic -Wall
IMGFLAGS = -ljpeg -lgif -lpng -lm

tp4: main.c avail.o hash.o leitura.o catalogo.o menu.o sk.o busca.o remove.o libimg.o descritor.o
	gcc $(FLAGS) $(IMGFLAGS) main.c avail.o hash.o leitura.o sk.o catalogo.o menu.o busca.o remove.o descritor.o libimg.o -o tp4

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

clean:
	rm -f *.o *~ tp4

del:
	rm -f /tmp/mc326/*.av /tmp/mc326/*.ch* /tmp/mc326/*.dsc

ef: main.c avail.o hash.o leitura.o catalogo.o menu.o sk.o busca.o remove.o libimg.o descritor.o
	gcc $(FLAGS) $(IMGFLAGS) -lefence main.c avail.o hash.o leitura.o sk.o catalogo.o menu.o busca.o remove.o descritor.o libimg.o -o tp4

all:
	${MAKE} tp4

efence:
	${MAKE} ef
