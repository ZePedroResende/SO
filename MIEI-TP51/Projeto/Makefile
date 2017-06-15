menu: comunicador nodo menu.c menu.h filters readline
	$(CC) -c menu.c
	$(CC) menu.o comunicador.o nodo.o readline.o -o controlador

nodo: nodo.c nodo.h readline
	$(CC) -c nodo.c

comunicador: comunicador.c comunicador.h readline
	$(CC) -c comunicador.c

filters: filter const spawn readline window

filter: filter.c
	$(CC) filter.c -o filter

const: const.c readline
	$(CC) -c const.c
	$(CC) const.o readline.o -o const

spawn: spawn.c readline
	$(CC) -c spawn.c
	$(CC) spawn.o readline.o -o spawn

readline: readline.c
	$(CC) -c readline.c

window:	window.c readline
	$(CC) -c window.c
	$(CC) window.o readline.o -o window

clean: 
	rm *.o filter const spawn window
