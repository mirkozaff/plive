/**
 *  Anno Accademico: 2012-2013
 *  Corso di Studio: Informatica
 *  Titolo del progetto: Modifica utility di base di GNU/Linux
 *  Autori: Zaffaroni Mirko n° 151818
 *	Bonetta Giovanni n° 152122
 **/
 
#ifndef FUNZIONI_H
#define	FUNZIONI_H

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <syslog.h>

typedef struct {                //struttura che contiene i dati di ogni processo
    char name[100];
    int pid;
    int ppid;
    char state;
    unsigned long int timep;    // tempo cpu del processo
    unsigned long int tot_t;    //tempo cpu totale
    float cpu;                  //%cpu usata dal processo 
} process;

/* prende una parola da file e la salva nel parametro string */
void take_s(char* string, FILE* f);

/*
controlla se la stringa passata come argomento contiene un intero
TRUE se str contiene un intero
FALSE altrimenti
*/
bool is_valid_int(const char *str);

/*calcola e ritorna il numero di processi contenuti in /proc*/
int num_proc(char *path);

/*salva nel parametro array_nomi_proc gli ID dei processi */
void nomi_proc(char *path, char** array_nomi_proc);

/*alloca una matrice di stringhe e ne ritorna il puntatore*/
char** alloca_matrice(int r, int c);

/*alloca un array di strutture process e ne ritorna il puntatore*/
process* alloca_proc(int n);

/*
calcola i dati del sistema e dei processi al tempo zero e li salva nell'array di process array_proc
*/
void calc_primo_tempo(int num_process, char** matrice, process* array_proc);

/*
calcola i dati del sistema e dei processi al tempo_uno,
elabora i dati al tempo_zero e salva i risultati nell'array di process array_proc
*/
void calc_secondo_tempo(int num_process, char** matrice, process* array_proc);

/* 
definisce se un processo è <, >, = di un altro processo
ritorna:
-1 se il primo processo è da considerarsi > del secondo
 1 se il primo processo è da considerarsi < del secondo
 0 se i due processi sono da considerarsi uguali
*/
int cmp(const void *a, const void *b);

/* ordina l'array di processi in base alla %cpu*/
void sort_proc(int num_process, process* array_proc);

/*stampa a video i dati contenuti nell'array di processi array_proc*/
void stampa(int n, process* array_proc);

/* 
funzione chiamata alla creazione della thread
gestisce l'input da tastiera 
*/ 
void *foo(void *secondi);

#endif	/* FUNZIONI_H */