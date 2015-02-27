#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include"socket.h"




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


			const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur \n" ;
			const char * mot; 
			const char * erreur="400 Bad request\r\n";
			const char * erreur404="404 Not Found\r\n";


			FILE* fichier2= NULL;
			http_request hr;

			fichier2=fdopen(socket_client, "w+");

			/* Récupérer la première ligne */
			int p = parse_http_request(fgets_or_exit(buf,256,fichier2),&hr);

			skip_headers(fichier2);

			if(p==0){
				mot="Bad Request";
				send_response(fichier2,400,mot,erreur);
			}
			else {
				char *newUrl=rewrite_url(hr.url);
				if(check_and_open(newUrl,(const char *)buf)!=-1){
					mot="Ok"; 
					send_response(fichier2,200,mot,message_bienvenue);
				}else{
					mot="Method Not Allow";
					send_response(fichier2,404,mot,erreur404);
				}
			}
		}
		exit(1);  
		close(socket_client);
		initialiser_signaux();
	}
	return 0;
}




