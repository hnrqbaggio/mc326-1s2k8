tp3: main.c leitura.o catalogo.o sk.o
	gcc -ggdb -ansi -pedantic -Wall main.c leitura.o sk.o catalogo.o  -o tp3

leitura.o: leitura.h leitura.c
	gcc -ggdb -ansi -pedantic -Wall -c leitura.c 

sk.o: sk.h sk.c
	gcc -ggdb -ansi -pedantic -Wall -c sk.c

catalogo.o: catalogo.h catalogo.c
	gcc -ggdb -ansi -pedantic -Wall -c catalogo.c


clean:
	rm -f *.o *~ tp3


