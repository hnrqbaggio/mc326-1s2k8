tp3: main.c avail.o hash.o leitura.o catalogo.o menu.o sk.o busca.o remove.o
	gcc -ggdb -ansi -pedantic -Wall main.c avail.o hash.o leitura.o sk.o catalogo.o menu.o busca.o remove.o -o tp3

leitura.o: leitura.h leitura.c
	gcc -ggdb -ansi -pedantic -Wall -c leitura.c 

sk.o: sk.h sk.c
	gcc -ggdb -ansi -pedantic -Wall -c sk.c

catalogo.o: catalogo.h catalogo.c
	gcc -ggdb -ansi -pedantic -Wall -c catalogo.c

menu.o: menu.h menu.c
	gcc -ggdb -ansi -pedantic -Wall -c menu.c 

busca.o: busca.h busca.c
	gcc -ggdb -ansi -pedantic -Wall -c busca.c

remove.o: remove.h remove.c
	gcc -ggdb -ansi -pedantic -Wall -c remove.c

avail.o: avail.h avail.c
	gcc -ggdb -ansi -pedantic -Wall -c avail.c
	
hash.o: hash.h hash.c
	gcc -ggdb -ansi -pedantic -Wall -c hash.c

clean:
	rm -f *.o *~ tp3

del:
	rm -f *.si *.pi *.av
	
all:
	${MAKE} tp3
