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

typedef struct {
    char name[100];
    int pid;
    int ppid;
    char state;
    unsigned long int timep; // tempo cpu del processo
    unsigned long int tot_t; //tempo cpu totale
    float cpu; //%cpu usata dal processo
} process;

void take_s(char* string, FILE* f);
bool is_valid_int(const char *str);
int num_proc(char *path);
void nomi_proc(char *path, char** array_nomi_proc);
char** alloca_matrice(int r, int c);
process* alloca_proc(int n);
void calc_primo_tempo(int num_process, char** matrice, process* array_proc);
void calc_secondo_tempo(int num_process, char** matrice, process* array_proc);
int cmp(const void *a, const void *b);
void sort_proc(int num_process, process* array_proc);
void stampa(int n, process* array_proc);
void *foo(void *secondi);

#endif	/* FUNZIONI_H */