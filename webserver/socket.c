#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


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

void initialiser_signaux(void){
  if (signal(SIGPIPE , SIG_IGN) == SIG_ERR )
    {
  perror ("signal");
    }

}
