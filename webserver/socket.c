#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>





int creer_serveur(int port){
  int socket_serveur;
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

  if(socket_serveur == -1)
    {
      perror("socket_serveur");
    }

  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d ’ écoute */
  saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */

  int optval = 1;
  if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1)
    perror ( " Can not set SO_REUSEADDR option " );


  if (bind(socket_serveur,(struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    {
      perror ("bind socker_serveur");
      /* traitement de l ’ erreur */
    }

  if (listen(socket_serveur,10) == -1)
    {
      perror ("listen socket_serveur");
      /* traitement d’erreur */
    }

  return socket_serveur;
}

void traitement_signal(int sig)
{
  
  int status;
  printf ("Signal %d reçu \n" , sig );
  if(sig ==SIGCHLD){
    while(waitpid(-1,&status,WNOHANG)>0)
      ;


  }
}
void initialiser_signaux ( void )
{
struct sigaction sa ;
sa.sa_handler = traitement_signal ;
sigemptyset (&sa.sa_mask );
sa.sa_flags = SA_RESTART ;
if (sigaction(SIGCHLD, &sa , NULL ) == -1)
{
perror ("sigaction(SIGCHLD)" );
}
}
char *fgets_or_exit(char *buffer, int size, FILE *stream){
  
  char *f = fgets(buffer,size,stream);
  if(f== NULL)
    exit(1);
  return f;
 
}
int parse_http_request ( const char * request_line  /*,http_request * request */){
  int reqOk=1;
 if(strncmp(request_line,"GET", 3)==0) {
	char *oc1 = strchr(request_line,' '); /* oc1 pointe sur ' ' après GET */
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
		if(strncmp(oc1,"/ ", 2)==0){
		  if(strncmp(oc2,"HTTP/1.", 7)==0) {
		    /* version majeure ok */

		    if (oc2[7] != '0' && oc2[7] != '1') 
		      reqOk=0;
		  }
		}else 
		  reqOk=404;
	      }
	      
	  }
      } else 
	reqOk=0;
 return reqOk;

}




