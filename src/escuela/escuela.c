#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "escuela.h"
//Inclusiones para resolver obtencion de objetos relacion
#include "../localidad/localidad.h"
#include "../circuito/circuito.h"

THIS(obj_Escuela)

void CopyDataEscuela (t_Escuela *dest,t_Escuela *orig)
{	
	dest->Escuela_id = orig->Escuela_id;
	dest->Circuito_id = orig->Circuito_id;	
	strcpy( dest->nombre_Escuela,orig->nombre_Escuela);
	dest->Localidad_id = orig->Localidad_id;
	strcpy( dest->dir_Escuela,orig->dir_Escuela);
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillEscuelaFromDB(void **rw , int rowi,PGresult *res)
{
    t_Escuela *dsEscuela;
    dsEscuela = &(( (t_Escuela *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsEscuela->Escuela_id = atoi(PQgetvalue(res,rowi,0));
	dsEscuela->Circuito_id = atoi(PQgetvalue(res,rowi,1));
    strcpy( dsEscuela->nombre_Escuela ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsEscuela->dir_Escuela ,rtrim(PQgetvalue(res,rowi,3),' '));
    dsEscuela->Localidad_id = atoi(PQgetvalue(res,rowi,4));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Escuela
static void fillObjEscuela(obj_Escuela *p,  t_Escuela rwitm)
{  
	CopyDataEscuela(&(p->info),&rwitm);	
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Escuela
static void fillRowsEscuela(void **list, int size,void *data)
{
     int i;
     obj_Escuela *d;
     // pedir memoria para el listado en memoria de Escuelas obj_Escuela
    *list = (obj_Escuela **)malloc(sizeof(obj_Escuela*)* size);
    for(i=0;i<size;++i)
    {
      d = Escuela_new();
      fillObjEscuela(d,((t_Escuela *)data)[i]);
      ((obj_Escuela **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Escuela(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Escuela.
 t->rows = malloc(sizeof(t_Escuela)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataEscuela(&(((t_Escuela *)t->rows)[i]),&(((t_Escuela *)data)[i]));	  
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Escuela(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Escuela),fillEscuelaFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_EscuelaImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Escuela), ((t_table*)((obj_Escuela*)self)->ds)->rows,fillEscuelaFromDB,fillRowsEscuela);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Escuela *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Escuela
static int find_EscuelaImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Escuela *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Escuela_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Escuela(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Escuela(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjEscuela(obj,((t_Escuela *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_EscuelaImpl(void *self)
{
   obj_Escuela *obj = this(self); 
   int newIdEscuela;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdEscuela);
   if(isNew_Obj)
     obj->info.Escuela_id = newIdEscuela;
   return retValue;
}
//----------------------------------------------------
static void toString_EscuelaImpl(void *self)
{
     obj_Escuela *self_o=this(self);
     obj_Escuela *sup;
     obj_Localidad *loc=self_o->getLocalidadObj(self_o);
     obj_Circuito *circuito=self_o->getCircuitoObj(self_o);
     printf("Escuela_Id:%d|Nombre_Escuela:%s|Localidad:%s|Circuito:%s|Direccion:%s\n",self_o->info.Escuela_id,self_o->info.nombre_Escuela,loc->getNombreLocalidad(loc),circuito->getNombreCircuito(circuito),self_o->info.dir_Escuela);
     //printf("Escuela_id: %d  Nombre_Escuela:%s - direccion: %s - Localidad: %s\n",self_o->info.Escuela_id,self_o->info.nombre_Escuela,self_o->info.dir_Escuela,loc->getNombreLocalidad(loc));
}
//----------------------------------------------------
//implementacion de getters
static int getEscuelaId_Impl(void *self)
{ 
  obj_Escuela *obj = this(self);
  return obj->info.Escuela_id;
}
//----------------------------------------------------
static int getCircuitoId_Impl(void *self)
{ 
  obj_Escuela *obj = this(self);
  return obj->info.Circuito_id;
}
//----------------------------------------------------
static int getLocalidadId_Impl(void *self)
{ 
  obj_Escuela *obj = this(self);
  return obj->info.Localidad_id;
}
//----------------------------------------------------
static char *getNombreEscuela_Impl(void *self)
{
	obj_Escuela *obj = this(self);
	return obj->info.nombre_Escuela;	
}
//----------------------------------------------------
static char *getDireccionEscuela_Impl(void *self)
{
	obj_Escuela *obj = this(self);
	return obj->info.dir_Escuela;
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreEscuela_Impl(void *self,char *nombre_Escuela)
{ 
	obj_Escuela *obj = this(self);
	strcpy(obj->info.nombre_Escuela,nombre_Escuela);
}
//----------------------------------------------------
static void setDireccionEscuela_Impl(void *self,char *direccion_Escuela)
{ 
	obj_Escuela *obj = this(self);
	strcpy(obj->info.dir_Escuela,direccion_Escuela);
}
//----------------------------------------------------
static void setCircuitoId_Impl(void *self,int CircuitoId)
{ 
	obj_Escuela *obj = this(self);
	obj->info.Circuito_id = CircuitoId;
}
//----------------------------------------------------
static void setLocalidadId_Impl(void *self,int LocalidadId)
{ 
	obj_Escuela *obj = this(self);
	obj->info.Localidad_id = LocalidadId;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Escuela *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Escuela_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Circuito_id );
   if(pos==2)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Escuela );
   if(pos==3)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.dir_Escuela );
   if(pos==4)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.Localidad_id );
   strcat(cad,field);   
}
//----------------------------------------------------
// Implementar relaciones con otras entidades.
static void *getLocalidadObj_Impl(void *self)
{
	obj_Escuela *obj = (obj_Escuela *)self;
	if(obj->localidad_obj==NULL)
	{
		obj->localidad_obj = Localidad_new();		
		((obj_Localidad *)obj->localidad_obj)->findbykey(obj->localidad_obj,obj->getLocalidadId(obj));
	}
	return obj->localidad_obj;
}
//----------------------------------------------------
static void *getCircuitoObj_Impl(void *self)
{
	obj_Escuela *obj = (obj_Escuela *)self;
	if(obj->circuito_obj==NULL)
	{
		obj->circuito_obj = Circuito_new();		
		((obj_Circuito *)obj->circuito_obj)->findbykey(obj->circuito_obj,obj->getCircuitoId(obj));
	}
	return obj->circuito_obj;
}
//----------------------------------------------------
//implementacion interna de Liberacion de punteros internos.
static void destroyInternal_Impl(void *self)
{
	obj_Escuela *obj = (obj_Escuela *)self;
	// liberar referencias internas
	if(obj->localidad_obj!=NULL) 
	  free(obj->localidad_obj);
	if(obj->circuito_obj!=NULL)
	  free(obj->circuito_obj);
	  
}
//----------------------------------------------------
static void *init_Escuela(void *self)
{
  obj_Escuela *obj = (obj_Escuela *)self;   
  //setear valores default
  obj->info.Escuela_id=0;
  CLEAR(obj->info.nombre_Escuela,MAX);
  obj->ds  = &table_Escuela;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getEscuelaId  	  = getEscuelaId_Impl;
  obj->getCircuitoId  	  = getCircuitoId_Impl;
  obj->getLocalidadId  	  = getLocalidadId_Impl;
  obj->getNombreEscuela = getNombreEscuela_Impl;  
  obj->getDireccionEscuela = getDireccionEscuela_Impl;
  /// setters  
  obj->setCircuitoId  	  = setCircuitoId_Impl;
  obj->setLocalidadId  	  = setLocalidadId_Impl;
  obj->setDireccionEscuela = setDireccionEscuela_Impl;
  obj->setNombreEscuela = setNombreEscuela_Impl;  
  // relaciones con otras entidades  
  obj->localidad_obj = NULL;//setear en NULL en el constructor.
  obj->circuito_obj = NULL;//setear en NULL en el constructor.
  obj->getLocalidadObj = getLocalidadObj_Impl;
  obj->getCircuitoObj = getCircuitoObj_Impl;
  // debo implementar funcion de destroyInternal
  //incializacion de la interfaz de la entidad
  obj->destroyInternal = destroyInternal_Impl;
  /// verificar lo faltante......
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_EscuelaImpl;
  obj->findAll =   findAll_EscuelaImpl;
  obj->saveObj =   saveObj_EscuelaImpl; 
  obj->toString =   toString_EscuelaImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Escuela
obj_Escuela *Escuela_new()
{
  return (obj_Escuela *)init_obj(sizeof(obj_Escuela), init_Escuela);
}
//----------------------------------------------------
