#
#  Anno Accademico: 2012-2013
#  Corso di Studio: Informatica
#  Titolo del progetto: Modifica utility di base di GNU/Linux
#  Autori: Zaffaroni Mirko n° 151818
#	Bonetta Giovanni n° 152122
#
# Makefile plive
# Sistema operativo Linux con compilatore gcc 4.7.3
#
plive: plive.o funzioni.o
	gcc -o plive plive.o funzioni.o -lncurses -lpthread
#
plive.o: plive.c
	gcc -c plive.c 
#
funzioni.o: funzioni.c
	gcc -c funzioni.c 
