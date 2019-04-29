#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "lista_partido_localidad.h"

THIS(obj_ListaPartidoLocalidad)

void CopyDataListaPartidoLocalidad (t_ListaPartidoLocalidad *dest,t_ListaPartidoLocalidad *orig)
{	
	dest->ListaPartidoLocalidad_id = orig->ListaPartidoLocalidad_id;	
	dest->ListaPartido_id = orig->ListaPartido_id;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillListaPartidoLocalidadFromDB(void **rw , int rowi,PGresult *res)
{
    t_ListaPartidoLocalidad *dsListaPartidoLocalidad;
    dsListaPartidoLocalidad = &(( (t_ListaPartidoLocalidad *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsListaPartidoLocalidad->ListaPartidoLocalidad_id = atoi(PQgetvalue(res,rowi,0));
	dsListaPartidoLocalidad->ListaPartido_id = atoi(PQgetvalue(res,rowi,1));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_ListaPartidoLocalidad
static void fillObjListaPartidoLocalidad(obj_ListaPartidoLocalidad *p,  t_ListaPartidoLocalidad rwitm)
{  
	CopyDataListaPartidoLocalidad(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_ListaPartidoLocalidad
static void fillRowsListaPartidoLocalidad(void **list, int size,void *data)
{
     int i;
     obj_ListaPartidoLocalidad *d;
     // pedir memoria para el listado en memoria de ListaPartidoLocalidads obj_ListaPartidoLocalidad
    *list = (obj_ListaPartidoLocalidad **)malloc(sizeof(obj_ListaPartidoLocalidad*)* size);
    for(i=0;i<size;++i)
    {
      d = ListaPartidoLocalidad_new();
      fillObjListaPartidoLocalidad(d,((t_ListaPartidoLocalidad *)data)[i]);
      ((obj_ListaPartidoLocalidad **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_ListaPartidoLocalidad(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_ListaPartidoLocalidad.
 t->rows = malloc(sizeof(t_ListaPartidoLocalidad)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataListaPartidoLocalidad(&(((t_ListaPartidoLocalidad *)t->rows)[i]),&(((t_ListaPartidoLocalidad *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_ListaPartidoLocalidad(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_ListaPartidoLocalidad),fillListaPartidoLocalidadFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_ListaPartidoLocalidadImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_ListaPartidoLocalidad), ((t_table*)((obj_ListaPartidoLocalidad*)self)->ds)->rows,fillListaPartidoLocalidadFromDB,fillRowsListaPartidoLocalidad);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_ListaPartidoLocalidad *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para ListaPartidoLocalidad
static int find_ListaPartidoLocalidadImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_ListaPartidoLocalidad *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.ListaPartidoLocalidad_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_ListaPartidoLocalidad(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_ListaPartidoLocalidad(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjListaPartidoLocalidad(obj,((t_ListaPartidoLocalidad *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_ListaPartidoLocalidadImpl(void *self)
{
   obj_ListaPartidoLocalidad *obj = this(self); 
   int newIdListaPartidoLocalidad;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdListaPartidoLocalidad);
   if(isNew_Obj)
     obj->info.ListaPartidoLocalidad_id = newIdListaPartidoLocalidad;
   return retValue;
}
//----------------------------------------------------
static void toString_ListaPartidoLocalidadImpl(void *self)
{
     obj_ListaPartidoLocalidad *self_o=this(self);
     obj_ListaPartidoLocalidad *sup;     
     printf("ListaPartidoLocalidad_id: %d  \n",self_o->info.ListaPartidoLocalidad_id);
}
//----------------------------------------------------
//implementacion de getters
static int getListaPartidoLocalidadId_Impl(void *self)
{ 
  obj_ListaPartidoLocalidad *obj = this(self);
  return obj->info.ListaPartidoLocalidad_id;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_ListaPartidoLocalidad *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.ListaPartidoLocalidad_id );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_ListaPartidoLocalidad(void *self)
{
  obj_ListaPartidoLocalidad *obj = (obj_ListaPartidoLocalidad *)self;   
  //setear valores default
  obj->info.ListaPartidoLocalidad_id=0;
  
  obj->ds  = &table_ListaPartidoLocalidad;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getListaPartidoLocalidadId  	  = getListaPartidoLocalidadId_Impl;
 
  /// verificar lo faltante......
  /// setters  
 
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_ListaPartidoLocalidadImpl;
  obj->findAll =   findAll_ListaPartidoLocalidadImpl;
  obj->saveObj =   saveObj_ListaPartidoLocalidadImpl; 
  obj->toString =   toString_ListaPartidoLocalidadImpl;
  return obj;
}
//----------------------------------------------------
//constructor de ListaPartidoLocalidad
obj_ListaPartidoLocalidad *ListaPartidoLocalidad_new()
{
  return (obj_ListaPartidoLocalidad *)init_obj(sizeof(obj_ListaPartidoLocalidad), init_ListaPartidoLocalidad);
}
//----------------------------------------------------
