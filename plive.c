#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "funzioni.h"

int main(int argc, char *argv[]) {
    
    char dir[40];
    
    printf("Dimmi la cartella che vuoi vedere: ");
    scanf("%s", dir);
    
    //stampa solo cartelle che hanno il nome in numeri es: "524"
    printdir(dir);


	return 0;
}
