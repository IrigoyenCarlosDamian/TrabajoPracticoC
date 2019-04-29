#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "lista_partido.h"
//Inclusiones para resolver obtencion de objetos relacion
#include "../partido/partido.h"

THIS(obj_ListaPartido)

void CopyDataListaPartido (t_ListaPartido *dest,t_ListaPartido *orig)
{	
	dest->ListaPartido_id = orig->ListaPartido_id;	
	dest->Partido_id = orig->Partido_id;		
	strcpy( dest->nombre_ListaPartido,orig->nombre_ListaPartido);	
}

//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillListaPartidoFromDB(void **rw , int rowi,PGresult *res)
{
    t_ListaPartido *dsListaPartido;
    dsListaPartido = &(( (t_ListaPartido *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsListaPartido->ListaPartido_id = atoi(PQgetvalue(res,rowi,0));
	dsListaPartido->Partido_id = atoi(PQgetvalue(res,rowi,1));
    strcpy( dsListaPartido->nombre_ListaPartido ,rtrim(PQgetvalue(res,rowi,2),' '));    
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_ListaPartido
static void fillObjListaPartido(obj_ListaPartido *p,  t_ListaPartido rwitm)
{  
	CopyDataListaPartido(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_ListaPartido
static void fillRowsListaPartido(void **list, int size,void *data)
{
     int i;
     obj_ListaPartido *d;
     // pedir memoria para el listado en memoria de ListaPartidos obj_ListaPartido
    *list = (obj_ListaPartido **)malloc(sizeof(obj_ListaPartido*)* size);
    for(i=0;i<size;++i)
    {
      d = ListaPartido_new();
      fillObjListaPartido(d,((t_ListaPartido *)data)[i]);
      ((obj_ListaPartido **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_ListaPartido(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_ListaPartido.
 t->rows = malloc(sizeof(t_ListaPartido)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataListaPartido(&(((t_ListaPartido *)t->rows)[i]),&(((t_ListaPartido *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_ListaPartido(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_ListaPartido),fillListaPartidoFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_ListaPartidoImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_ListaPartido), ((t_table*)((obj_ListaPartido*)self)->ds)->rows,fillListaPartidoFromDB,fillRowsListaPartido);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_ListaPartido *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para ListaPartido
static int find_ListaPartidoImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_ListaPartido *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.ListaPartido_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_ListaPartido(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_ListaPartido(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjListaPartido(obj,((t_ListaPartido *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_ListaPartidoImpl(void *self)
{
   obj_ListaPartido *obj = this(self); 
   int newIdListaPartido;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdListaPartido);
   if(isNew_Obj)
     obj->info.ListaPartido_id = newIdListaPartido;
   return retValue;
}
//----------------------------------------------------
static void toString_ListaPartidoImpl(void *self)
{
     obj_ListaPartido *self_o=this(self);
     obj_ListaPartido *sup; 
     obj_Partido *part = self_o->getPartidoObj(self_o);
     printf("ListaPartido_id: %d  ListaPartido:%s - NroPartido:%d \n",self_o->info.ListaPartido_id,self_o->info.nombre_ListaPartido,part->getNroPartido(part));
}
//----------------------------------------------------
//implementacion de getters
static int getListaPartidoId_Impl(void *self)
{ 
  obj_ListaPartido *obj = this(self);
  return obj->info.ListaPartido_id;
}
//----------------------------------------------------
static int getPartidoId_Impl(void *self)
{ 
  obj_ListaPartido *obj = this(self);
  return obj->info.Partido_id;
}
//----------------------------------------------------
static char *getNombreListaPartido_Impl(void *self)
{
	obj_ListaPartido *obj = this(self);
	return obj->info.nombre_ListaPartido;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreListaPartido_Impl(void *self,char *nombre_ListaPartido)
{ 
	obj_ListaPartido *obj = this(self);
	strcpy(obj->info.nombre_ListaPartido,nombre_ListaPartido);
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_ListaPartido *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.ListaPartido_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_ListaPartido );
   strcat(cad,field);   
}
//----------------------------------------------------
// Implementar relaciones con otras entidades.
static void *getPartidoObj_Impl(void *self)
{
	obj_ListaPartido *obj = (obj_ListaPartido *)self;
	if(obj->partido_obj==NULL)
	{
		obj->partido_obj = Partido_new();		
		((obj_Partido *)obj->partido_obj)->findbykey(obj->partido_obj,obj->getPartidoId(obj));
	}
	return obj->partido_obj;
}
//----------------------------------------------------
static void destroyInternal_Impl(void *self)
{
	obj_ListaPartido *obj = (obj_ListaPartido *)self;
	// liberar referencias internas
	if(obj->partido_obj!=NULL) 
	  free(obj->partido_obj);	
}
//----------------------------------------------------
static void *init_ListaPartido(void *self)
{
  obj_ListaPartido *obj = (obj_ListaPartido *)self;   
  //setear valores default
  obj->info.ListaPartido_id=0;
  CLEAR(obj->info.nombre_ListaPartido,MAX);
  obj->ds  = &table_ListaPartido;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getListaPartidoId  	  = getListaPartidoId_Impl;
  obj->getPartidoId  	  = getPartidoId_Impl;
  obj->getNombreListaPartido = getNombreListaPartido_Impl;  
  /// verificar lo faltante......
  /// setters  
  obj->setNombreListaPartido = setNombreListaPartido_Impl;  
  // relaciones con otras entidades  
  obj->partido_obj = NULL;//setear en NULL en el constructor.
  obj->getPartidoObj = getPartidoObj_Impl;  
  // debo implementar funcion de destroyInternal
  //incializacion de la interfaz de la entidad
  obj->destroyInternal = destroyInternal_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_ListaPartidoImpl;
  obj->findAll =   findAll_ListaPartidoImpl;
  obj->saveObj =   saveObj_ListaPartidoImpl; 
  obj->toString =   toString_ListaPartidoImpl;
  return obj;
}
//----------------------------------------------------
//constructor de ListaPartido
obj_ListaPartido *ListaPartido_new()
{
  return (obj_ListaPartido *)init_obj(sizeof(obj_ListaPartido), init_ListaPartido);
}
//----------------------------------------------------
