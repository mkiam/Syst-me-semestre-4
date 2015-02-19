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


enum http_method {
HTTP_GET ,
HTTP_UNSUPPORTED ,
};

typedef struct
{
enum http_method method;
int major_version;
int minor_version;
char *url;
} http_request;


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
int parse_http_request ( const char * request_line  ,http_request * request ){
  int reqOk=1;

 if(strncmp(request_line,"GET", 3)==0) {

   request->method = HTTP_GET;
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

		request->url = oc1;

		if(strncmp(oc1,"/ ", 2) == 0){
		  if(strncmp(oc2,"HTTP/1.", 7 )== 0) {
		    request->major_version = 1;

		    /* version majeure ok */

		    if (oc2[7] != '0' && oc2[7] != '1') 
		      reqOk=0;
		    else
		      request->minor_version = oc2[7];
		  }
		}else 
		  reqOk = 404;
	      }
	      
	  }
 } else{ 
	reqOk=0;
	request->method= HTTP_UNSUPPORTED;
 }

 return reqOk;


}
/*
void skip_headers(FILE *client){
  int c1=0;
  char c2="\r\n";
  char c3="\n";
  while(c1!=(int)c2&&c1!=(int)c3)
    c1=fgetc(client);
  fp = fopen (client,"w+");
   fprintf(fp,"pawnee %s",  );

}

void send_status(FILE *client , int code , const char *reason_phrase){
  FILE* fichier2= NULL;
  char buf[256]
    fichier2 = fdopen(client, "w+");
     
  Récupérer la première ligne 
 parse_http_request(fgets_or_exit(buf,256,fichier2));
  fprintf(fichier2,"SP %s",code,"SP %s",reason_phrase);

}
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
  fichier2 = fdopen(client, "w+");
  send_status(client, code, reason_phrase,reason_phrase);
  fprintf(fichier2,"pawnee %s",message_body);

}*/




