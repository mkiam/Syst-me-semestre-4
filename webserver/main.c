#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>




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
    write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));
    int nb=read(socket_client,buf, 256);
    while(nb>0){
      write(socket_client,buf,nb);
      nb=read(socket_client,buf, 256);
    }
exit(1);
   
}else{
close(socket_client);
}
}
  return 0;
}

