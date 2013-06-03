/**
 *  Anno Accademico: 2012-2013
 *  Corso di Studio: Informatica
 *  Titolo del progetto: Modifica utility di base di GNU/Linux
 *  Autori: Zaffaroni Mirko n° 151818
 *  Bonetta Giovanni n° 152122
 **/
 
#include "funzioni.h"

int main(int argc, char *argv[]) {

    openlog(argv[0], LOG_CONS || LOG_PID, LOG_LOCAL0);     //apro il log

    WINDOW * mainwin;            //finestra dove stamperò i risultati della plive
    pthread_t worker_thread;     
    int i;
    int n = 10;
    char c;
    int secondi = 1;
    int flag;
    char* n_flag = 0;
    int tmpn;
    int x, y;


    while ((flag = getopt(argc, argv, "n:")) != -1){                     //richiamo la getopt
        switch (flag) {
            case 'n':                                              
                n_flag = optarg;
                if (is_valid_int(n_flag) == TRUE) n = atoi(n_flag);     //se l'argomento di -n è un intero n = intero
                else {
                    fprintf(stderr, "-n not valid argument.\n");
                    syslog(LOG_ERR, "Errore di input nel programma");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Errore di input nel programma.\n");
                syslog(LOG_ERR, "Errore di input nel programma");
                exit(EXIT_FAILURE);
        }
    }


    if ((mainwin = initscr()) == NULL) {                     //se ho un errore nell'inizializzare la finestra esco
        fprintf(stderr, "Error initialising ncurses.\n");
        syslog(LOG_ERR, "Errore nell'inizializzare ncurses");
        exit(EXIT_FAILURE);
    }

    noecho();                    // disabilito l'echo dei pulsanti
    keypad(mainwin, TRUE);       // Enable the keypad for non-char keys 


    if (pthread_create(&worker_thread, NULL,      //creo la pthread
            *foo, &secondi)) {
        fprintf(stderr, "Error while creating thread\n");
        syslog(LOG_ERR, "Errore nel creare la thread");
        exit(EXIT_FAILURE);
    }

    while (c != 'q') {

        int npstampa;

        getmaxyx(mainwin, y, x);            //setto x = massima ampiezza del terminale, y = massima profondità del terminale
        if (n > y - 3) npstampa = y - 3;
        else {
            npstampa = n;
        }

        int num_process = num_proc("/proc/");           //vedo quanti processi ci sono in /proc
        process* array_proc = alloca_proc(num_process); //alloco un array di strutture processi
        if (array_proc == NULL){ 
            syslog(LOG_ERR, "non riesco ad inizializzare l'array di processi");
            exit(EXIT_FAILURE);
        }

        char** matrice = alloca_matrice(num_process, 10); //alloco la matrice dei nomi delle cartelle dei processi
        if (matrice == NULL){ 
            syslog(LOG_ERR, "non riesco ad inizializzare la matrice dei processi");
            exit(EXIT_FAILURE);
        }

        nomi_proc("/proc/", matrice);                         //metto nella matrice i nomi dei processi presenti in /proc

        calc_primo_tempo(num_process, matrice, array_proc);   //setto per ogni processo i propri dati
        sleep(secondi);
        calc_secondo_tempo(num_process, matrice, array_proc); //aggiorno i dati di ogni processo. mi serve per settare la %cpu 

        sort_proc(num_process, array_proc);                   //sorto l'array dei processi in base alla cpu

        clear();   //pulisco lo schermo
        stampa(npstampa, array_proc);   //stampo i dati di ogni processo
        refresh();                      

    }

    endwin();   //chiudo la finestra
    syslog(LOG_INFO, "Programma eseguito con successo");
    closelog();    //chiudo il log
    return 0;
}