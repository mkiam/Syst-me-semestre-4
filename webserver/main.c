#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>




int main (void/*int argc,char ** argv*/)
{
initialiser_signaux();

  int socket_serveur = creer_serveur(8000);
  char buf[256];
  int socket_client;
  while(1){
    socket_client = accept (socket_serveur, NULL, NULL );
    if (socket_client == -1)
      {
	perror ( "accept" );
	/* traitement d’ erreur */
      }
pid_t pid;
    /* On peut maintenant dialoguer avec le client */
pid=fork();
if(pid==0){
    
    sleep(1);
    const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur \n" ;
FILE* fichier1 = NULL;

fichier1=fdopen(2, "w+");
FILE* fichier2= NULL;
fichier2=fdopen(socket_client, "w+");
fprintf(fichier1,"pawnee %s", message_bienvenue);
   while(fgets(buf,256,fichier2)!= NULL){
fprintf(fichier1,"pawnee %s",buf);
   
}
 exit(1);  
}else{
close(socket_client);
}
initialiser_signaux();
}
  return 0;
}

