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
      const char * erreur="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
      const char * erreur404="HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 17\r\n\r\n404 Not Found\r\n";


      FILE* fichier2= NULL;
      http_request hr;
      
      fichier2=fdopen(socket_client, "w+");

      /* Récupérer la première ligne */
      int p = parse_http_request(fgets_or_exit(buf,256,fichier2),&hr);

skip_headers(fichier2);

      if(p==0)
	send_status(fichier2,400, erreur);
      else if(p==404)
	send_status(fichier2,404, erreur404);
      else     
	fprintf(fichier2,"pawnee %s", message_bienvenue);
      /* répondre au client */
      exit(1);  
    }else{
      close(socket_client);
    }
    initialiser_signaux();
  }
  return 0;
}

