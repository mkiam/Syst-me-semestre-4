#include <stdio.h>
#include <string.h>
#include "socket.h"

int main (void/*int argc,char ** argv*/)
{
  int serveur=creer_serveur(8000);
  
  while(1){
    accept(serveur, NULL, NULL);
   
	}
 return 0;
}
