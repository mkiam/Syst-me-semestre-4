
#ifndef __SOCKET_H__
#define __SOCKET_H__

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

/** Crée une socket serveur qui écoute sur toute les interfaces IPv4
de la machine sur le port passé en paramètre . La socket retournée
doit pouvoir être utilisée directement par un appel à accept .
La fonction retourne -1 en cas d ’ erreur ou le descripteur de la
socket créée . */

int creer_serveur (int);
void initialiser_signaux(void);
void traitement_signal(int);
char *fgets_or_exit(char *, int, FILE *);
int parse_http_request ( const char *,http_request * );
void skip_headers(FILE *);
void send_status(FILE *, int , const char *);
void send_response(FILE *, int , const char *, const char *);
#endif
