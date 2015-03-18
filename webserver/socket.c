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
#include <sys/stat.h>
#include <fcntl.h>



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
	    if(oc1[0]=='/'){
	      request->url=malloc(oc2-oc1);
	      strncpy(request->url,oc1,oc2-oc1-1);
	      request->url[(oc2-oc1)-1] = '\0';
	      if(strncmp(oc2,"HTTP/1.", 7 )== 0) {
		request->major_version = 1;
		    
		/* version majeure ok */
		    
		if (oc2[7] != '0' && oc2[7] != '1') 
		  reqOk=0;
		else
		  request->minor_version = oc2[7]-'0';
	      }
	    }else 
	      reqOk = 1;
	  }
	      
      }
  } else{ 
    reqOk=0;
    request->method= HTTP_UNSUPPORTED;
  }

  return reqOk;


}

void skip_headers(FILE *client){
  char buf[256];
  while(fgets_or_exit(buf,256,client)){
    if (strcmp(buf,"\r\n")==0||strcmp(buf,"\n")==0)
      break;
  }
}

void send_status(FILE *client , int code , const char *reason_phrase){
  fprintf(client,"HTTP/1.1 %d %s\r\n",code,reason_phrase);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
  send_status(client, code, reason_phrase);
  fprintf(client, "Content-Length: %d\r\n", (int) strlen(message_body));
  fprintf(client, "\r\n");
  fprintf(client,"%s",message_body);
  fflush(client);
}


char *rewrite_url(char *url){
  int i;
  int taille = strlen(url);
 
  if(url[taille-1]== '/')
    strcat(url,"index.html"); 

  for(i=0;i<taille;++i){
    if(url[i] =='?')
      {
	url[i] = '\0';
	return url;
      }
  
  }
  return url;
}

int check_and_open(const char *url, const char *document_root){
  struct stat buf;
  int fd;
  char *tmp;
  if(strstr(url, "..")){
    errno = EACCES;
    return -1;
  }
  tmp=malloc(strlen(url)+strlen(document_root)+1);
  strcpy(tmp, document_root);
  strcat(tmp, url);
  printf("chemin: %s\n", tmp);

  if(stat(tmp,&buf)==-1){
    perror(tmp);
    return -1;
  }
  if(S_ISREG(buf.st_mode)){
    fd = open(tmp, O_RDONLY);
    if (fd == -1){
      
      perror("open");
      return -1;
    }
    return fd;
  }
  errno = EACCES;
  return -1;
}

int get_file_size(int fd){
  struct stat buf;

  if(fstat(fd, &buf) == -1){
    perror("fichier");
    return -1;
  }

  return buf.st_size;

}

int copy(int in, int out){
  int val;
  int t;
 
  while((t = read(in, &val, sizeof(val))) != 0){
    if(t == -1){
      perror("read");
      return -1;
    }

    if(write(out, &val, t) == -1){
      perror("write");
      return -1;
    }
 
  }
  return out;
}








