#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "telegrama_acta_votos.h"

THIS(obj_ActaTelegramaVotos)

void CopyDataActaTelegramaVotos (t_ActaTelegramaVotos *dest,t_ActaTelegramaVotos *orig)
{	
	dest->ActaTelegramaVotos_id = orig->ActaTelegramaVotos_id;	
	dest->ActaTelegrama_id = orig->ActaTelegrama_id;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillActaTelegramaVotosFromDB(void **rw , int rowi,PGresult *res)
{
    t_ActaTelegramaVotos *dsActaTelegramaVotos;
    dsActaTelegramaVotos = &(( (t_ActaTelegramaVotos *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsActaTelegramaVotos->ActaTelegramaVotos_id = atoi(PQgetvalue(res,rowi,0));
	dsActaTelegramaVotos->ActaTelegrama_id = atoi(PQgetvalue(res,rowi,1));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_ActaTelegramaVotos
static void fillObjActaTelegramaVotos(obj_ActaTelegramaVotos *p,  t_ActaTelegramaVotos rwitm)
{  
	CopyDataActaTelegramaVotos(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_ActaTelegramaVotos
static void fillRowsActaTelegramaVotos(void **list, int size,void *data)
{
     int i;
     obj_ActaTelegramaVotos *d;
     // pedir memoria para el listado en memoria de ActaTelegramaVotoss obj_ActaTelegramaVotos
    *list = (obj_ActaTelegramaVotos **)malloc(sizeof(obj_ActaTelegramaVotos*)* size);
    for(i=0;i<size;++i)
    {
      d = ActaTelegramaVotos_new();
      fillObjActaTelegramaVotos(d,((t_ActaTelegramaVotos *)data)[i]);
      ((obj_ActaTelegramaVotos **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_ActaTelegramaVotos(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_ActaTelegramaVotos.
 t->rows = malloc(sizeof(t_ActaTelegramaVotos)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataActaTelegramaVotos(&(((t_ActaTelegramaVotos *)t->rows)[i]),&(((t_ActaTelegramaVotos *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_ActaTelegramaVotos(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_ActaTelegramaVotos),fillActaTelegramaVotosFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_ActaTelegramaVotosImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_ActaTelegramaVotos), ((t_table*)((obj_ActaTelegramaVotos*)self)->ds)->rows,fillActaTelegramaVotosFromDB,fillRowsActaTelegramaVotos);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_ActaTelegramaVotos *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para ActaTelegramaVotos
static int find_ActaTelegramaVotosImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_ActaTelegramaVotos *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.ActaTelegramaVotos_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_ActaTelegramaVotos(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_ActaTelegramaVotos(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjActaTelegramaVotos(obj,((t_ActaTelegramaVotos *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_ActaTelegramaVotosImpl(void *self)
{
   obj_ActaTelegramaVotos *obj = this(self); 
   int newIdActaTelegramaVotos;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdActaTelegramaVotos);
   if(isNew_Obj)
     obj->info.ActaTelegramaVotos_id = newIdActaTelegramaVotos;
   return retValue;
}
//----------------------------------------------------
static void toString_ActaTelegramaVotosImpl(void *self)
{
     obj_ActaTelegramaVotos *self_o=this(self);
     obj_ActaTelegramaVotos *sup;     
     printf("ActaTelegramaVotos_id: %d  \n",self_o->info.ActaTelegramaVotos_id);
}
//----------------------------------------------------
//implementacion de getters
static int getActaTelegramaVotosId_Impl(void *self)
{ 
  obj_ActaTelegramaVotos *obj = this(self);
  return obj->info.ActaTelegramaVotos_id;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_ActaTelegramaVotos *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.ActaTelegramaVotos_id );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_ActaTelegramaVotos(void *self)
{
  obj_ActaTelegramaVotos *obj = (obj_ActaTelegramaVotos *)self;   
  //setear valores default
  obj->info.ActaTelegramaVotos_id=0;
  
  obj->ds  = &table_ActaTelegramaVotos;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getActaTelegramaVotosId  	  = getActaTelegramaVotosId_Impl;
 
  /// verificar lo faltante......
  /// setters  
 
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_ActaTelegramaVotosImpl;
  obj->findAll =   findAll_ActaTelegramaVotosImpl;
  obj->saveObj =   saveObj_ActaTelegramaVotosImpl; 
  obj->toString =   toString_ActaTelegramaVotosImpl;
  return obj;
}
//----------------------------------------------------
//constructor de ActaTelegramaVotos
obj_ActaTelegramaVotos *ActaTelegramaVotos_new()
{
  return (obj_ActaTelegramaVotos *)init_obj(sizeof(obj_ActaTelegramaVotos), init_ActaTelegramaVotos);
}
//----------------------------------------------------
