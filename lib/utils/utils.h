extern PGconn *conn;

char* rtrim(char* string, char junk);
char** fStrSplit(char *str, const char *delimiters);
char* fStrJoin(char **str, const char *delimiters, int sz_opt);
void disconnectdb(PGconn *);
PGconn * connectdb(char *servidor,char *port,char *base,char *usuario,char *password);
void inicializarArregloInt(int *arreglo, int cantidad, int valor);
char *itoa_simple(char *dest, int i);
int startswith(const char *pre, const char *str);
char * getFechaHora();
char * getFecha();

void prepararConsulta(char**, char*, char*);
void prepararConsultaString(char**, char*, char*);
void separarArgumentos(int, char*, char**, int, char***, int *);
void separarArgumentosProfesional(char **, int, int, int **);
void separarArgumentosTurno(char **, int, int, char ***);
void mostrarAyuda(char *);
