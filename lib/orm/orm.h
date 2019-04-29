typedef enum {t_int=1,t_varchar,t_bool,t_float,t_date} tipo_dato;
//----------------------------------------------------
//Formato de una columna: nombre, tipo, es primary key? tamaño para varchar
typedef struct{
  char nombre[MAX];
  tipo_dato tipo;
  size_t size;
  bool pkey;
  bool autonum;
}t_column;
//----------------------------------------------------
typedef struct {
	char nombre[MAX]; // nombre de la tabla
	int cant_columns; //cantidad de columnas
    int cant_rows; // cantidad de filas
    void *columns; //coleccion de columnas
    void *rows; //coleccion de filas
}t_table;
//----------------------------------------------------
// Estructura de pseudoobjeto genérico: Object 
typedef struct 
{
 t_table *ds;
 int  (*findAll)(void *self, void **list,char *criteria);
 void (*toString)(void *self);
 bool (*getIsNewObj)(void *self);
 int  (*saveObj)(void *self);
 void (*destroyInternal)(void *self);
 void (*getValueByPos)(void *self,char *cad, int pos);
 bool isNewObj;
 int (*findbykey)(void *self,int);
}t_object;
//----------------------------------------------------
// IMPLEMENTACION PARA DAR SOPORTE A COMPORTAMIENTO GENERICO - ABSTRACT.
// Implementacion en  "orm.c"
int exec_get_fromDB(char *,void **, size_t, void(*)(void **, int, PGresult *));
char *getUpdateSQL(void *self);
char *getInsertSQL(void *self);
char *getFindByKeySQL(void *self);
int findAllImpl(void *,void **, char *, size_t, void *,void(*)(void **,int, PGresult *),void(*)(void **, int, void *));
bool saveObjImpl(void *,int*);
void *init_obj(size_t, void*(*)(void *));
void listObj(void *objList, char *criteria, int freeObj,void(*lstFunc)(void *o));
void destroyObj(void *);
void destroyObjList(void **,int);
// Implementacion en "utils.c"
char *getFecha();
char *getFechaHora();

