#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>  /* Inclusion de la libreria de acceso a funcionalidad para manipular objetos y conectarse a PostgreSQL */
#include <time.h>
#include <malloc.h>

#include "utils.h"
#include "../../config/config.h"


#define MAXARG 30
#define TOPE 9
#define MENOSELE "-l"


//#define MAXFECHA 17
 //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
char* rtrim(char* string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}
//----------------------------------------------------
char** fStrSplit(char *str, const char *delimiters)
{
    char * token; 
    char **tokenArray;
    int count=0;
    token = (char *)strtok(str, delimiters); // Get the first token
    tokenArray = (char**)malloc(1 * sizeof(char*));
    tokenArray[0] = NULL;
    if (!token) {       
        return tokenArray;  
    } 
    while (token != NULL) { // While valid tokens are returned
        tokenArray[count] = (char*)strdup(token);
        //printf ("%s", tokenArray[count]);
        count++;
        tokenArray = (char **)realloc(tokenArray, sizeof(char *) * (count + 1));
        token = (char *)strtok(NULL, delimiters); // Get the next token
    }
    tokenArray[count] = NULL;  /* Terminate the array */
    return tokenArray;
}
//----------------------------------------------------
char* fStrJoin(char **str, const char *delimiters, int sz_opt)
{
    char *joinedStr;
    int i = 1, size=0;
    
    if(sz_opt!=0 ) 
    {
      size = sz_opt;
    }
    else
      size = strlen(str[0])+1;
    joinedStr = (char *)realloc(NULL, size);
    strcpy(joinedStr, str[0]);
    if (str[0] == NULL){
        return joinedStr;
    }
    while (i< size ){
        joinedStr = (char*)realloc(joinedStr, strlen(joinedStr) + strlen(str[i]) + strlen(delimiters) + 1);
        strcat(joinedStr, delimiters);
        strcat(joinedStr, str[i]);
        i++;
    }
    return joinedStr;
}
//----------------------------------------------------
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
//----------------------------------------------------
PGconn * connectdb(char *servidor,char *port,char *base,char *usuario,char *password)
{
   PGconn *conn;
   /*Realizar la conexion al servidor*/
   conn = PQsetdbLogin(servidor, port, NULL,NULL, base,usuario,password);
   /*Consultar estado de la operacion de conexion*/

   if (PQstatus(conn) != CONNECTION_OK)
      fprintf(stderr, "La conexion a la Base de Datos ha fallado: %s", PQerrorMessage(conn) );               
   
   return conn;
 }
//----------------------------------------------------
void disconnectdb(PGconn * conn)
{   
   /*Cerrar conexion y liberar recurso de conexion con el servidor*/
   PQfinish(conn);
}
//----------------------------------------------------
char *getFecha()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
     return ffecha;
}
//----------------------------------------------------
char *getFechaHora()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d %02d:%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min);
     return ffecha;
}
//----------------------------------------------------
void inicializarArregloInt(int *arreglo, int cantidad, int valor){
    int i;
    for(i = 0; i< cantidad; i++){
        arreglo[i] = valor;
    }
}
//----------------------------------------------------

static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}
//----------------------------------------------------

char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}
//----------------------------------------------------
 
