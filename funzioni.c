#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "funzioni.h"



void printdir(char *path){
    
    DIR * d;
	struct dirent *dir;
	d = opendir(path);
	if (d != NULL)  {
		while ((dir = readdir(d)) != NULL) {
			if(strcmp(".", dir -> d_name) == 0 || strcmp("..", dir -> d_name) == 0){
				continue;
			}
			if(!(is_valid_int(dir -> d_name)))
				continue;
			printf("%s\n", dir -> d_name);
		}

    closedir(d);
 	}
}


boolean is_valid_int(const char *str)
{
   // Gestisce i numeri negativi
   if (*str == '-')
      str++;

   // Gestisce le stringhe vuote
   if (!*str)
      return FALSE;

   //Controllo per i char non-digit nel resto della stringa
   while (*str)
   {
      if (!isdigit(*str))
         return FALSE;
      else
         str++;
   }

   return TRUE;
}