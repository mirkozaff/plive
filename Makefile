#
# Makefile plive
# Sistema operativo Linux con compilatore gcc 4.7.3
#
plive: plive.o funzioni.o
	gcc -o plive plive.o funzioni.o -lncurses -lpthread
#
plive.o: plive.c
	gcc -c plive.c -lncurses -lpthread
#
funzioni.o: funzioni.c
	gcc -c funzioni.c -lncurses -lpthread
