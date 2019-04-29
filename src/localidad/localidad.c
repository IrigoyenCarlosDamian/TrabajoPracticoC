#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "localidad.h"

THIS(obj_Localidad)

void CopyDataLocalidad (t_Localidad *dest,t_Localidad *orig)
{	
	dest->Localidad_id = orig->Localidad_id;	
	strcpy( dest->nombre_Localidad,orig->nombre_Localidad);	
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillLocalidadFromDB(void **rw , int rowi,PGresult *res)
{
    t_Localidad *dsLocalidad;
    dsLocalidad = &(( (t_Localidad *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsLocalidad->Localidad_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsLocalidad->nombre_Localidad ,rtrim(PQgetvalue(res,rowi,1),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Localidad
static void fillObjLocalidad(obj_Localidad *p,  t_Localidad rwitm)
{  
	CopyDataLocalidad(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Localidad
static void fillRowsLocalidad(void **list, int size,void *data)
{
     int i;
     obj_Localidad *d;
     // pedir memoria para el listado en memoria de Localidads obj_Localidad
    *list = (obj_Localidad **)malloc(sizeof(obj_Localidad*)* size);
    for(i=0;i<size;++i)
    {
      d = Localidad_new();
      fillObjLocalidad(d,((t_Localidad *)data)[i]);
      ((obj_Localidad **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Localidad(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Localidad.
 t->rows = malloc(sizeof(t_Localidad)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
		CopyDataLocalidad(&(((t_Localidad *)t->rows)[i]),&(((t_Localidad *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Localidad(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Localidad),fillLocalidadFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_LocalidadImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Localidad), ((t_table*)((obj_Localidad*)self)->ds)->rows,fillLocalidadFromDB,fillRowsLocalidad);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Localidad *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Localidad
static int find_LocalidadImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Localidad *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Localidad_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Localidad(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Localidad(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjLocalidad(obj,((t_Localidad *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_LocalidadImpl(void *self)
{
   obj_Localidad *obj = this(self); 
   int newIdLocalidad;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdLocalidad);
   if(isNew_Obj)
     obj->info.Localidad_id = newIdLocalidad;
   return retValue;
}
//----------------------------------------------------
static void toString_LocalidadImpl(void *self)
{
     obj_Localidad *self_o=this(self);
     obj_Localidad *sup;     
     printf("Localidad_id: %d  Localidad:%s \n",self_o->info.Localidad_id,self_o->info.nombre_Localidad);
}
//----------------------------------------------------
//implementacion de getters
static int getLocalidadId_Impl(void *self)
{ 
  obj_Localidad *obj = this(self);
  return obj->info.Localidad_id;
}
//----------------------------------------------------
static char *getNombreLocalidad_Impl(void *self)
{
	obj_Localidad *obj = this(self);
	return obj->info.nombre_Localidad;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreLocalidad_Impl(void *self,char *nombre_Localidad)
{ 
	obj_Localidad *obj = this(self);
	strcpy(obj->info.nombre_Localidad,nombre_Localidad);
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Localidad *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Localidad_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Localidad );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_Localidad(void *self)
{
  obj_Localidad *obj = (obj_Localidad *)self;   
  //setear valores default
  obj->info.Localidad_id=0;
  CLEAR(obj->info.nombre_Localidad,MAX);
  obj->ds  = &table_Localidad;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getLocalidadId  	  = getLocalidadId_Impl;
  obj->getNombreLocalidad = getNombreLocalidad_Impl;  
  /// setters  
  obj->setNombreLocalidad = setNombreLocalidad_Impl;  
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_LocalidadImpl;
  obj->findAll =   findAll_LocalidadImpl;
  obj->saveObj =   saveObj_LocalidadImpl; 
  obj->toString =   toString_LocalidadImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Localidad
obj_Localidad *Localidad_new()
{
  return (obj_Localidad *)init_obj(sizeof(obj_Localidad), init_Localidad);
}
//----------------------------------------------------
