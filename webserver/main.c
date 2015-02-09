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

      int reqOk=1;
      FILE* fichier2= NULL;
      fichier2=fdopen(socket_client, "w+");
     
      /* Récupérer la première ligne */
      fgets(buf,256,fichier2);

      /* vérifier la première ligne */


      if(strncmp(buf,"GET", 3)==0) {
	char *oc1 = strchr(buf,' '); /* oc1 pointe sur ' ' après GET */
	if( oc1 == NULL || oc1[1] == ' ')
	  {
	    reqOk=0;
	  }
	else
	  {
	    char *oc2 = strchr(oc1 + 1, ' '); /* oc2 pointe sur ' ' après l'url */
	    if (oc2 == NULL || oc2[1] == ' ') 
	      {
		reqOk=0;
	      }
	    else
	      {
		oc2++;
		oc1++;
		/* buf pointe sur GET */
		/* oc1 pointe sur url */
		/* oc2 pointe sur HTTP/.... */
		if(strncmp(oc2,"HTTP/1.", 7)==0) {
		  /* version majeure ok */
		  if (oc2[7] != '0' && oc2[7] != '1') 
		    reqOk=0;
		}
	      }
	  }
      } else 
	reqOk=0;
	

      


      while(fgets(buf,256,fichier2)!= NULL){
	if (strcmp(buf,"\r\n")==0||strcmp(buf,"\n")==0)
	  break;
      }

      if(reqOk==0)
	fprintf(fichier2,"pawnee %s", erreur);
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

