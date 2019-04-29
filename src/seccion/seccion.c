#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "seccion.h"

THIS(obj_Seccion)

void CopyDataSeccion (t_Seccion *dest,t_Seccion *orig)
{	
	dest->Seccion_id = orig->Seccion_id;
	strcpy( dest->nombre_Seccion,orig->nombre_Seccion);	
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillSeccionFromDB(void **rw , int rowi,PGresult *res)
{
    t_Seccion *dsSeccion;
    dsSeccion = &(( (t_Seccion *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsSeccion->Seccion_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsSeccion->nombre_Seccion ,rtrim(PQgetvalue(res,rowi,1),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Seccion
static void fillObjSeccion(obj_Seccion *p,  t_Seccion rwitm)
{  
	CopyDataSeccion(&(p->info),&rwitm);    
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Seccion
static void fillRowsSeccion(void **list, int size,void *data)
{
     int i;
     obj_Seccion *d;
     // pedir memoria para el listado en memoria de Seccions obj_Seccion
    *list = (obj_Seccion **)malloc(sizeof(obj_Seccion*)* size);
    for(i=0;i<size;++i)
    {
      d = Seccion_new();
      fillObjSeccion(d,((t_Seccion *)data)[i]);
      ((obj_Seccion **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Seccion(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Seccion.
 t->rows = malloc(sizeof(t_Seccion)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	  CopyDataSeccion(&(((t_Seccion *)t->rows)[i]),&(((t_Seccion *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Seccion(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Seccion),fillSeccionFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_SeccionImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Seccion), ((t_table*)((obj_Seccion*)self)->ds)->rows,fillSeccionFromDB,fillRowsSeccion);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Seccion *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Seccion
static int find_SeccionImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Seccion *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Seccion_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Seccion(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Seccion(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjSeccion(obj,((t_Seccion *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_SeccionImpl(void *self)
{
   obj_Seccion *obj = this(self); 
   int newIdSeccion;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdSeccion);
   if(isNew_Obj)
     obj->info.Seccion_id = newIdSeccion;
   return retValue;
}
//----------------------------------------------------
static void toString_SeccionImpl(void *self)
{
     obj_Seccion *self_o=this(self);
     obj_Seccion *sup;     
     printf("Seccion_id: %d  Seccion:%s \n",self_o->info.Seccion_id,self_o->info.nombre_Seccion);
}
//----------------------------------------------------
//implementacion de getters
static int getSeccionId_Impl(void *self)
{ 
  obj_Seccion *obj = this(self);
  return obj->info.Seccion_id;
}
//----------------------------------------------------
static char *getNombreSeccion_Impl(void *self)
{
	obj_Seccion *obj = this(self);
	return obj->info.nombre_Seccion;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreSeccion_Impl(void *self,char *nombre_Seccion)
{ 
	obj_Seccion *obj = this(self);
	strcpy(obj->info.nombre_Seccion,nombre_Seccion);
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Seccion *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Seccion_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Seccion );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_Seccion(void *self)
{
  obj_Seccion *obj = (obj_Seccion *)self;   
  //setear valores default
  obj->info.Seccion_id=0;
  CLEAR(obj->info.nombre_Seccion,MAX);
  obj->ds  = &table_Seccion;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getSeccionId  	  = getSeccionId_Impl;
  obj->getNombreSeccion = getNombreSeccion_Impl;  
  /// setters  
  obj->setNombreSeccion = setNombreSeccion_Impl;  
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_SeccionImpl;
  obj->findAll =   findAll_SeccionImpl;
  obj->saveObj =   saveObj_SeccionImpl; 
  obj->toString =   toString_SeccionImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Seccion
obj_Seccion *Seccion_new()
{
  return (obj_Seccion *)init_obj(sizeof(obj_Seccion), init_Seccion);
}
//----------------------------------------------------
