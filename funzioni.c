/**
 *  Anno Accademico: 2012-2013
 *  Corso di Studio: Informatica
 *  Titolo del progetto: Modifica utility di base di GNU/Linux
 *  Autori: Zaffaroni Mirko n° 151818
 *  Bonetta Giovanni n° 152122
 **/

#include "funzioni.h"

void take_s(char* string, FILE* f) {        

    int i = 0;
    char l;
    l = getc(f);                                                 

    while ((l != '\n') && (l != EOF) && (isspace(l) == 0)) {     // salva in string un carattere alla volta finche non trova uno spazio vuoto o finisce il file di lettura
        string[i++] = l;
        l = getc(f);
    }
    string[i++] = '\0';                                          //mette il carattere di fine stringa                                   
}

bool is_valid_int(const char *str) {

    if (*str == '-')                       //se il numero è negativo ritorna FALSE
        return FALSE;

    if (!*str)                             //se il puntatore a str è nullo ritorna FALSE
        return FALSE;

    while (*str) {
        if (!isdigit(*str))                //se trova un carattere che nn è un numero ritorna false
            return FALSE;
        else
            str++;
    }

    return TRUE;                           //str è un intero positivo, ritorna TRUE
}

int num_proc(char *path) {

    int i = 0;
    DIR * d;
    struct dirent *dir;
    d = opendir(path);
    if (d == NULL){ 
            syslog(LOG_ERR, "non riesco ad aprire /proc/");           //exit se d è nullo
            exit(EXIT_FAILURE);
        }
    if (d != NULL) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir -> d_name) == 0 || strcmp("..", dir -> d_name) == 0) {
                continue;
            }
            if (!(is_valid_int(dir -> d_name)))        
                continue;
            i++;                        //se il nome della cartella è un numero aumenta il contatore
        }
        closedir(d);
    }
    return i;                    //ritorna il numero di cartelle che hanno per nome un numero (ossia le cartelle dei processi)
}

void nomi_proc(char *path, char** array_nomi_proc) {

    int i = 0;
    DIR * d;
    struct dirent *dir;
    d = opendir(path);
         if (d == NULL){ 
            syslog(LOG_ERR, "non riesco ad aprire /proc/");         //exit se d è nullo
            exit(EXIT_FAILURE);
        }
    if (d != NULL) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir -> d_name) == 0 || strcmp("..", dir -> d_name) == 0) {
                continue;
            }
            if (!(is_valid_int(dir -> d_name)))
                continue;
            strcpy(array_nomi_proc[i], dir -> d_name);            //salva nell'array di nomi dei processi il nome della cartella in cui sta leggendo
            i++;
        }
        closedir(d);
    }
}

char** alloca_matrice(int r, int c) { 

    char** array;
    int i;
    array = (char**) malloc(sizeof (char*)*r);         //alloca un array di #r puntatori a stringhe
    for (i = 0; i < r; i++) {
        array[i] = (char*) malloc(sizeof (char)*c);    //per ogni cella dell'arrey alloca lo spazio per una stringa grande c
    }
    return array;
}

process* alloca_proc(int n) {

    process* array;
    array = (process*) malloc(sizeof (process) * n);   //alloca un array di #n processi
    return array;
}

void calc_primo_tempo(int num_process, char** matrice, process* array_proc) { 

    FILE * f;
    FILE * s;

    int i;

    for (i = 0; i < num_process; i++) {
        
        char path[10];           
        char status[10];          
        strcpy(path, "/proc/");                            //salvo "/proc/" nella stringa path
        strcpy(status, "/stat");                           //salvo "/stat" nella stringa status
        char proc[100];          
        strcpy(proc, matrice[i]);                          //salvo l'id del processo nella stringa proc
        strcat(path, proc);
        strcat(path, status);                              //la stringa path contiene il path del file "stat" del processo 

        f = fopen(path, "r");                              //apre stat
        if (f == NULL){ 
            syslog(LOG_INFO, "non riesco a leggere %s", path);
            continue; 
        }
        s = fopen("/proc/stat", "r");                      //apre proc/stat
        if (f == NULL){ 
            syslog(LOG_ERR, "non riesco a leggere /proc/stat");
            exit(EXIT_FAILURE);
        }
        
        /* prendo i dati che mi servono dai file /proc/stat e /proc/<pid>/stat */
        int d;                                      
        char cpu[4];
        unsigned int u;
        long int cutime, cstime;
        unsigned long int lu;
        unsigned long int utime, stime;
        unsigned long int c1, c2, c3, c4, c5, c6, c7;
        fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld",
        &array_proc[i].pid, array_proc[i].name, &array_proc[i].state, &array_proc[i].ppid, &d, &d, &d, &d, &u, &lu, &lu, &lu, &lu, &utime, &stime, &cutime, &cstime);
        array_proc[i].timep = utime + stime + cutime + stime;
        fscanf(s, "%s %lu %lu %lu %lu %lu %lu %lu", cpu, &c1, &c2, &c3, &c4, &c5, &c6, &c7);       //salva i tempi totali della cpu nelle variabili c1...c7
        array_proc[i].tot_t = c1 + c2 + c3 + c4 + c5 + c6 + c7;
        fclose(f);
        fclose(s);
    }
}

void calc_secondo_tempo(int num_process, char** matrice, process* array_proc) {  

    FILE * f;
    FILE * s;

    int i;
     
    for (i = 0; i < num_process; i++) {

        char path[10];                                                                          
        char status[10];                                                                         
        strcpy(path, "/proc/");                          //salvo "/proc/" nella stringa path
        strcpy(status, "/stat");                         //salvo "/stat" nella stringa status
        char proc[100];          
        strcpy(proc, matrice[i]);                        //salvo l'id del processo nella stringa proc
        strcat(path, proc);  
        strcat(path, status);                            //la stringa path contiene il path del file "stat" del processo

        f = fopen(path, "r");                            //apre stat 
        if (f == NULL){ 
            syslog(LOG_INFO, "non riesco a leggere %s", path);
            continue; 
        }              
        s = fopen("/proc/stat", "r");                    //apre proc/stat
        if (f == NULL){ 
            syslog(LOG_ERR, "non riesco a leggere /proc/stat");
            exit(EXIT_FAILURE);
        }
        
        /* prendo i dati che mi servono dai file /proc/stat e /proc/<pid>/stat */
        int d;
        char cpu[4];
        unsigned int u;
        long int cutime, cstime;
        unsigned long int lu;
        unsigned long int utime, stime;
        unsigned long int c1, c2, c3, c4, c5, c6, c7;    
        fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld",
        &array_proc[i].pid, array_proc[i].name, &array_proc[i].state, &array_proc[i].ppid, &d, &d, &d, &d, &u, &lu, &lu, &lu, &lu, &utime, &stime, &cutime, &cstime);
        array_proc[i].timep = (utime + stime + cutime + stime) - array_proc[i].timep;
        fscanf(s, "%s %lu %lu %lu %lu %lu %lu %lu", cpu, &c1, &c2, &c3, &c4, &c5, &c6, &c7);   //salva i tempi totali della cpu nelle variabili c1...c7 
        array_proc[i].tot_t = (c1 + c2 + c3 + c4 + c5 + c6 + c7) - array_proc[i].tot_t;       //salva in tot_t la differenza di tempo utilizzata dalla cpu
        array_proc[i].cpu =(float) (array_proc[i].timep * 100) / array_proc[i].tot_t;        //calcolo la %cpu

        fclose(f);
        fclose(s);
    }
}

void sort_proc(int num_process, process* array_proc){
  qsort(array_proc, num_process, sizeof(process), cmp);    //chiama la quicksort passando l'array di processi e il puntatore alla funzione cmp
}

int cmp(const void *a, const void *b){               //definisco il rapporto tra i processi a e b
  process primo = *(process *)a;
  process secondo = *(process *)b;                   
  if(primo.cpu > secondo.cpu) return -1;             //a > b
  if(primo.cpu < secondo.cpu) return 1;              //a < b
  return 0;                                          //a = b
}

void stampa(int n, process* array_proc){

  int i;

  printw("%s \n\n", "    PID  |    PPID |     %CPU    |          NAME     ");
  for(i=0; i<n; i++){
     {printw("%8d |%8d |%12f |%20s \n", array_proc[i].pid, array_proc[i].ppid, array_proc[i].cpu, array_proc[i].name);}
    }
}

void *foo(void *secondi){
    
    int* sec = (int*) secondi;
    char c;
    c = getch();
    while(c != 'q' && c != 'Q'){
        if(c=='1' || c=='2' ||c=='3' || c=='4' ||c=='5' || c=='6' ||c=='7' || c=='8' || c=='9'){     //se l'input è un numero setta la variabile secondi = c
            *sec = atoi(&c);
        }
        c = getch();
    }
    endwin();         //se l'input è 'q' o 'Q' chiude il programma
    exit(0);
}
