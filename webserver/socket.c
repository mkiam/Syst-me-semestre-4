#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

int creer_serveur(int port){
int socket_serveur;
socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

if(socket_serveur == -1)
  {
    perror("socket_serveur");
  }

struct sockaddr_in saddr ;
saddr.sin_family = AF_INET; /* Socket ipv4 */
saddr.sin_port = htons(8000); /* Port d ’ écoute */
saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */

if (bind(socket_serveur,(struct sockaddr*) &saddr, sizeof(saddr)) == -1)
{
perror ("bind socker_serveur");
/* traitement de l ’ erreur */
}

if (listen(socket_serveur,10) == -1)
{
perror ("listen socket_serveur");
/* traitement d ’ erreur */
}

int socket_client;
socket_client = accept (socket_serveur, NULL, NULL);

if (socket_client == -1)
{
perror ( " accept " );
/* traitement d ’ erreur */
}
/* On peut maintenant dialoguer avec le client */

const char * message_bienvenue = "Bonjour,bienvenue sur mon serveur \n" ;
write (socket_client , message_bienvenue , strlen(message_bienvenue));

return 1;
}