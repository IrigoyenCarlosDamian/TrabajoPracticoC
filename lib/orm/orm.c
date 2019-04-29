#include "../../config/config.h"
#include "../utils/utils.h"
#include "orm.h"
#include <string.h>
#include <malloc.h>
/*
funcion generica que recibe como argumento cadena sql puntero a array dinamico en memoria para alojar segun la funcion de
llenado pasada por parametro (puntero a funcion)
*/
int exec_get_fromDB(char *sql,void **rw, size_t t,void(*fcFill)(void **,int ri,PGresult *r)) {
    PGconn *conn;
    //Conexion con la base de datos 
	if ((conn = connectdb(SERVER,PORT,DB,USER,PASS)) == NULL){
        exit(-1);
    }
    int tuplas=0,i=0;
    PGresult *res; //Instancia para contener los datos de ejecucion de consulta
    res = PQexec(conn, sql);

    if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res)) {
        tuplas = PQntuples(res);
        if(tuplas!=0) {
            *rw = malloc(t * tuplas);
            for (i = 0; i<tuplas; i++) 
            {
             //el puntero a funcion recibido carga la info en la estructura
             fcFill(rw,i,res);
            }        }
        PQclear(res);
    }
    disconnectdb(conn);
    return tuplas;
}
//----------------------------------------------------
static char *selectSQL(t_object *o)
{
  t_table *tt = o->ds;
  int size=0,i=0;
  char *sql, str_where[MAX_WHERE_SQL],fields[MAX_SQL];

  sql = (char*)malloc(MAX_SQL*sizeof(char));
  //blanquear cadenas
  memset(sql, '\0',MAX_SQL);
  memset(fields, '\0',MAX_SQL);
  
  //ver si hay criterio para agregar 
  strcpy(sql,SQL_STR);
  //recorrer cada nombre de columna(armar listado de campos)
  for(i=0;i<tt->cant_columns;++i)
  {
     strcat(fields , ((t_column *)tt->columns)[i].nombre);
     strcat(fields,(i<tt->cant_columns-1)?",":"");
  }
  //reemplazar listado de campos y nombre de tabla.
  strcpy(sql ,(char*) str_replace(sql,TOK1,fields));  
  strcpy(sql ,(char*)str_replace(sql,TOK2,tt->nombre));
  return sql;
}
//----------------------------------------------------
/* interface generica para poder ejecuar fidall obj */
/*
crear consulta SQL de la Tabla para poder cargar una lista generica en memoria.
*/
int findAllImpl(
void *self,  // puntero generico al que aplica
void **list, // lista resultado donde se devuelve la informacion
char *criteria, // criterio aplicado //despues del where 
size_t t, 
void *data,
void(*fcFill)(void **,int ri, PGresult *r),
void(*fillRows)(void **list,int size,void *data)
) 
{                     
  int size=0,i=0;
  char sql[MAX_SQL], fields[MAX_SQL];
  t_table *tt = ((t_object*)self)->ds;  

  strcpy(sql,selectSQL((t_object*)self));
  WHERE_SQL(sql,criteria);  
  // conformar una cadena sql: SELECT [campos...] FROM [tabla] WHERE [...]    
  // cargar el puntero data con datos desde la base de datos.
  size = exec_get_fromDB(sql,&data,t,fcFill);
  if(size>0)
    fillRows(list,size, data);    
  return size;
}
//----------------------------------------------------
bool saveObjImpl(
void *self,  // puntero generico al que aplica
int *newId // En la insecion debe retornar el id nuevo.
)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL], where[MAX_WHERE_SQL],*sql;   
   t_object *obj = (t_object *) self;
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert
		sql = (char*)getInsertSQL(obj);
		res = PQexec(conn, sql);
		code = PQresultStatus(res);
		*newId = atoi(PQgetvalue(res,0,0));	
		PQclear(res);
		free(sql);
		obj->isNewObj=false;// marcar que esta guardado en la BD.
  }
  else
  {// update
      sql = (char*)getUpdateSQL(obj);
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);	  
	  free(sql);
  }
  if ( code != PGRES_COMMAND_OK)  {
        disconnectdb(conn);     
       return false;
  }
    else
    {
       disconnectdb(conn);
       return true;    
    }
}
//----------------------------------------------------
char *getFindByKeySQL(void *self)//, char *where )
{
  t_table *tt = ((t_object*)self)->ds;
  int size=0,i=0;
  char *sql,where[MAX_WHERE_SQL];  
  memset(where, '\0',MAX_WHERE_SQL);
  sql = (char *)malloc(sizeof(char)*MAX_SQL); //liberar despues de usar
  //blanquear cadena
  memset(sql, '\0',MAX_SQL);
  strcat(sql,selectSQL((t_object*)self));
  for(i=0;i<tt->cant_columns;++i)
  {
    if(((t_column *)tt->columns)[i].pkey)
    {
     snprintf(where, MAX_WHERE_SQL,"%s=", ((t_column *)tt->columns)[i].nombre);
     ((t_object*)self)->getValueByPos(self, where,i);
     }     
  }  
  WHERE_SQL(sql,where);
  return sql;
}
//----------------------------------------------------
char *getUpdateSQL(void *self)
{
  t_table *tt = ((t_object*)self)->ds;
  int size=0,i=0;
  char *sqlRet,sql[MAX_SQL],fields[MAX_WHERE_SQL],where[MAX_WHERE_SQL],field[MAX_WHERE_SQL],values[MAX_WHERE_SQL];
      
  //blanquear cadenas
  CLEAR(sql,MAX_SQL);
  CLEAR(where,MAX_WHERE_SQL);
  CLEAR(field,MAX_WHERE_SQL);
  CLEAR(fields,MAX_WHERE_SQL);
  CLEAR(values,MAX_WHERE_SQL);

  //ver si hay criterio para agregar 
  strcpy(sql,SQL_UPDATE);
  //cant de columnas clave y cant de columnas datos
  //recorrer cada nombre de columna(armar listado de campos y where)
  for(i=0;i<tt->cant_columns;++i)
  {
     snprintf((((t_column *)tt->columns)[i].pkey? where:field), MAX_WHERE_SQL,"%s=", ((t_column *)tt->columns)[i].nombre);
     ((t_object*)self)->getValueByPos(self, (((t_column *)tt->columns)[i].pkey? where:field),i);
     strcat((((t_column *)tt->columns)[i].pkey? where:field), (((t_column *)tt->columns)[i].pkey?" and ":","));
  }
  //reemplazar listado de campos y nombre de tabla.
  strcpy(sql ,(char*)str_replace(sql,TOK1,tt->nombre));
  strcpy(sql ,(char*)str_replace(sql,TOK2,field));
  strcpy(sql ,(char*)str_replace(sql,TOK3,where));
  strcpy(sql ,(char*)str_replace(sql,", where "," where "));
  strcpy(sql ,(char*)str_replace(sql," and ;",";"));
  sqlRet = (char*)malloc(strlen(sql)+1*sizeof(char*));
  strcpy(sqlRet,sql);
  return sqlRet;
}
//----------------------------------------------------
char *getInsertSQL(void *self)
{
  t_table *tt = ((t_object*)self)->ds;
  int size=0,i=0,isAutoNum=0;
  char *sqlRet,sql[MAX_SQL],fields[MAX_WHERE_SQL],where[MAX_WHERE_SQL],field[MAX_WHERE_SQL],values[MAX_WHERE_SQL],col_autonum[MAX];
  //blanquear cadenas
  CLEAR(sql,MAX_SQL);
  CLEAR(where,MAX_WHERE_SQL);
  CLEAR(field,MAX_WHERE_SQL);
  CLEAR(fields,MAX_WHERE_SQL);
  CLEAR(values,MAX_WHERE_SQL);
  CLEAR(col_autonum,MAX);
  
  //ver si hay criterio para agregar 
  strcpy(sql,SQL_INSERT);   
  //cant de columnas clave y cant de columnas datos
  //recorrer cada nombre de columna(armar listado de campos y where)
  for(i=0;i<tt->cant_columns;++i)
  {
     //si la columna es clave. generar consulta sql para obtener el siguiente id +1
     snprintf(field, MAX_WHERE_SQL,"%s", ((t_column *)tt->columns)[i].nombre);
     strcat(field, ",");
     strcat(fields,field);
     isAutoNum = isAutoNum || (((t_column *)tt->columns)[i].autonum);
     
     if(((t_column *)tt->columns)[i].pkey && ((t_column *)tt->columns)[i].autonum) //si es la columna clave primaria, debe cambiar el value del insert.
       {
         strcat(values,SQL_MAX_ID);
         strcpy(values ,(char*)str_replace(values,TOK1,((t_column *)tt->columns)[i].nombre));//reemplazar nombre de columna clave
         strcpy(values ,(char*)str_replace(values,TOK2,tt->nombre));//reemplazar nombre de tabla
         strcpy(col_autonum, ((t_column *)tt->columns)[i].nombre);
       }
     else
     ((t_object*)self)->getValueByPos(self, values,i);     
     strcat(values, ",");
  }
  strcpy(sql ,(char*)str_replace(sql,TOK1,tt->nombre));
  strcpy(sql ,(char*)str_replace(sql,TOK2,fields));
  strcpy(sql ,(char*)str_replace(sql,TOK3,values));
  strcpy(sql ,(char*)str_replace(sql,",)",")"));
  if(!isAutoNum)
    strcpy(sql ,(char*)str_replace(sql," RETURNING $4;",";"));
  else
    strcpy(sql ,(char*)str_replace(sql,TOK4,col_autonum));
  sqlRet = (char*)malloc(strlen(sql)+1*sizeof(char*));
  strcpy(sqlRet,sql);
  return sqlRet;
}
//----------------------------------------------------
void *init_obj(size_t t_obj,void*(*init)(void *o))
{
   	 // pedir memoria para el objeto gral
     void *obj = malloc(t_obj);/// obj mem	 
	 ((t_object*)obj)->destroyInternal=NULL;//poner puntero a funcion en NULL, la invocacion de init de cada pseudoobjeto debe crear su version de detroyInternal
	 return init(obj);
}
//----------------------------------------------------
void destroyObj(void *objFree)
{
	t_object *obj = (t_object *)objFree;
	if(obj->destroyInternal!=NULL)
		obj->destroyInternal(obj);
	//free((obj->ds)->columns);
	free(obj->ds);
	free(obj);
}
//----------------------------------------------------
void destroyObjList(void **list,int size)
{
	int i=0;
	for(i=0;i< size;++i)
	{
		destroyObj(list[i]);		
	}
	free(list);
}
//----------------------------------------------------
void listObj(void *objList, char *criteria, int freeObj, void(*lstFunc)(void *o))//sobreescribir funcion toString....
{
	t_object *obj = (t_object *)objList;
	int i=0,size=0;
	void *list;
	t_object *rw;
    size = obj->findAll(obj,&list,NULL);
	for(i=0;i<size;++i)
	{
		rw = ((t_object **)list)[i];
		if(lstFunc!=NULL)
			lstFunc(rw);
		else
  		    rw->toString(rw);
	}
	if(freeObj)
	{
		destroyObjList(list,size);
		destroyObj(objList);
	}
}
