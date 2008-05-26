tp2: main.c leitura.o catalogo.o
	gcc -ggdb -ansi -pedantic -Wall main.c leitura.o catalogo.o -o tp2

leitura.o: leitura.h leitura.c
	gcc -ggdb -ansi -pedantic -Wall -c leitura.c 

catalogo.o: catalogo.h catalogo.c
	gcc -ggdb -ansi -pedantic -Wall -c catalogo.c

clean:
	rm -f *.o *~ tp2


