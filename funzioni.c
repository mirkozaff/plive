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

    while ((l != '\n') && (l != EOF) && (isspace(l) == 0)) {
        string[i++] = l;
        l = getc(f);
    }
    string[i++] = '\0';
}

bool is_valid_int(const char *str) {
    // Gestisce i numeri negativi
    if (*str == '-')
        str++;

    // Gestisce le stringhe vuote
    if (!*str)
        return FALSE;

    //Controllo per i char non-digit nel resto della stringa
    while (*str) {
        if (!isdigit(*str))
            return FALSE;
        else
            str++;
    }

    return TRUE;
}

int num_proc(char *path) {

    int i = 0;
    DIR * d;
    struct dirent *dir;
    d = opendir(path);
    if (d == NULL){ 
            syslog(LOG_ERR, "non riesco ad aprire /proc/");
            exit(EXIT_FAILURE);
        }
    if (d != NULL) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir -> d_name) == 0 || strcmp("..", dir -> d_name) == 0) {
                continue;
            }
            if (!(is_valid_int(dir -> d_name)))
                continue;
            i++;
        }
        closedir(d);
    }
    return i;
}

void nomi_proc(char *path, char** array_nomi_proc) {

    int i = 0;
    DIR * d;
    struct dirent *dir;
    d = opendir(path);
         if (d == NULL){ 
            syslog(LOG_ERR, "non riesco ad aprire /proc/");
            exit(EXIT_FAILURE);
        }
    if (d != NULL) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir -> d_name) == 0 || strcmp("..", dir -> d_name) == 0) {
                continue;
            }
            if (!(is_valid_int(dir -> d_name)))
                continue;
            strcpy(array_nomi_proc[i], dir -> d_name);
            i++;
        }
        closedir(d);
    }
}

char** alloca_matrice(int r, int c) { //alloco la matrice che conterrà i nomi delle cartelle dei processi

    char** array;
    int i;
    array = (char**) malloc(sizeof (char*)*r);
    for (i = 0; i < r; i++) {
        array[i] = (char*) malloc(sizeof (char)*c);
    }
    return array;
}

process* alloca_proc(int n) {

    process* array;
    array = (process*) malloc(sizeof (process) * n);
    return array;
}

void calc_primo_tempo(int num_process, char** matrice, process* array_proc) { //PARTI DA QUI. SETTA TUTTE LE STRUTTURE PROCESS CON I LORO DATI. 

    FILE * f;
    FILE * s;

    int i;

    for (i = 0; i < num_process; i++) {
        
        char path[10];           //contiene "/proc/"
        char status[10];         //contiene "/stat" 
        strcpy(path, "/proc/");   
        strcpy(status, "/stat");
        char proc[100];          //contiene il pid del processo
        strcpy(proc, matrice[i]);
        strcat(path, proc);
        strcat(path, status);     //path contiene il path del file stat del processo

        f = fopen(path, "r");            //apre stat
        if (f == NULL){ 
            syslog(LOG_INFO, "non riesco a leggere %s", path);
            continue; 
        }
        s = fopen("/proc/stat", "r");
        if (f == NULL){ 
            syslog(LOG_ERR, "non riesco a leggere /proc/stat");
            exit(EXIT_FAILURE);
        }

        int d;                                      
        char cpu[4];
        unsigned int u;
        unsigned long int lu;
        unsigned long int utime, stime;
        unsigned long int c1, c2, c3, c4, c5, c6, c7;
        fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu", &array_proc[i].pid, array_proc[i].name, &array_proc[i].state, &array_proc[i].ppid, &d, &d, &d, &d, &u, &lu, &lu, &lu, &lu, &utime, &stime);
        array_proc[i].timep = utime + stime;
        fscanf(s, "%s %lu %lu %lu %lu %lu %lu %lu", cpu, &c1, &c2, &c3, &c4, &c5, &c6, &c7);
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

        char path[10];           //contiene "/proc/"
        char status[10];         //contiene "/stat" 
        strcpy(path, "/proc/");   
        strcpy(status, "/stat");
        char proc[100];          //contiene il pid del processo
        strcpy(proc, matrice[i]);
        strcat(path, proc);
        strcat(path, status);     //path contiene il path del file stat del processo

        f = fopen(path, "r");                  //apre stat 
        if (f == NULL){ 
            syslog(LOG_INFO, "non riesco a leggere %s", path);
            continue; 
        }              //se il file non c'è passo al processo sucessivo
        s = fopen("/proc/stat", "r");          //apro proc/stat
        if (f == NULL){ 
            syslog(LOG_ERR, "non riesco a leggere /proc/stat");
            exit(EXIT_FAILURE);
        }

        int d;
        char cpu[4];
        unsigned int u;
        unsigned long int lu;
        unsigned long int utime, stime;
        unsigned long int c1, c2, c3, c4, c5, c6, c7;
        fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu", &array_proc[i].pid, array_proc[i].name, &array_proc[i].state, &array_proc[i].ppid, &d, &d, &d, &d, &u, &lu, &lu, &lu, &lu, &utime, &stime);;
        array_proc[i].timep = (utime + stime) - array_proc[i].timep;
        fscanf(s, "%s %lu %lu %lu %lu %lu %lu %lu", cpu, &c1, &c2, &c3, &c4, &c5, &c6, &c7);
        array_proc[i].tot_t = (c1 + c2 + c3 + c4 + c5 + c6 + c7) - array_proc[i].tot_t;
        array_proc[i].cpu =(float) (array_proc[i].timep * 100) / array_proc[i].tot_t;

        fclose(f);
        fclose(s);
    }
}

void sort_proc(int num_process, process* array_proc){
  qsort(array_proc, num_process, sizeof(process), cmp); 
}

int cmp(const void *a, const void *b){
  process primo = *(process *)a;
  process secondo = *(process *)b;
  if(primo.cpu > secondo.cpu) return -1;
  if(primo.cpu < secondo.cpu) return 1;
  return 0;
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

        if(c=='1' || c=='2' ||c=='3' || c=='4' ||c=='5' || c=='6' ||c=='7' || c=='8' || c=='9'){
        *sec = atoi(&c);
        }

   c = getch();
   }
   endwin();    
   exit(0);
}
