#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "circuito.h"
//Inclusiones para resolver obtencion de objetos relacion
#include "../localidad/localidad.h"
#include "../seccion/seccion.h"

THIS(obj_Circuito)
void CopyDataCircuito (t_Circuito *dest,t_Circuito *orig)
{	
	dest->Circuito_id = orig->Circuito_id;
	strcpy( dest->letra ,orig->letra);
	dest->Seccion_id = orig->Seccion_id;
	strcpy( dest->nombre_Circuito,orig->nombre_Circuito);
	dest->Localidad_id = orig->Localidad_id;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillCircuitoFromDB(void **rw , int rowi,PGresult *res)
{
    t_Circuito *dsCircuito;
    dsCircuito = &(( (t_Circuito *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsCircuito->Circuito_id = atoi(PQgetvalue(res,rowi,0));
	strcpy( dsCircuito->letra ,rtrim(PQgetvalue(res,rowi,1),' '));
    dsCircuito->Seccion_id = atoi(PQgetvalue(res,rowi,2));
    strcpy( dsCircuito->nombre_Circuito ,rtrim(PQgetvalue(res,rowi,3),' '));
    dsCircuito->Localidad_id = atoi(PQgetvalue(res,rowi,4));
}
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Circuito
static void fillObjCircuito(obj_Circuito *p,  t_Circuito rwitm)
{  
	CopyDataCircuito (&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Circuito
static void fillRowsCircuito(void **list, int size,void *data)
{
     int i;
     obj_Circuito *d;
     // pedir memoria para el listado en memoria de Circuitos obj_Circuito
    *list = (obj_Circuito **)malloc(sizeof(obj_Circuito*)* size);
    for(i=0;i<size;++i)
    {
      d = Circuito_new();
      fillObjCircuito(d,((t_Circuito *)data)[i]);
      ((obj_Circuito **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Circuito(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Circuito.
 t->rows = malloc(sizeof(t_Circuito)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {     
       CopyDataCircuito (&(((t_Circuito *)t->rows)[i]), &(((t_Circuito *)data)[i]));	 
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Circuito(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Circuito),fillCircuitoFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_CircuitoImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Circuito), ((t_table*)((obj_Circuito*)self)->ds)->rows,fillCircuitoFromDB,fillRowsCircuito);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	//obj_Circuito *obj = this(self);
	return  ((obj_Circuito *)self)->isNewObj;
	//return obj;
}
//----------------------------------------------------
// implementacion de metodos para Circuito
static int find_CircuitoImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Circuito *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Circuito_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;
   size = exec_get_Circuito(sql,&data);   
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Circuito(obj->ds,data,size);   
   // setear datos a la instancia....
   if(size>0)
   	 fillObjCircuito(obj,((t_Circuito *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_CircuitoImpl(void *self)
{
   obj_Circuito *obj = this(self); 
   int newIdCircuito;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdCircuito);
   if(isNew_Obj)
     obj->info.Circuito_id = newIdCircuito;
   return retValue;
}
//----------------------------------------------------
static void toString_CircuitoImpl(void *self)
{
     obj_Circuito *self_o=this(self);
     obj_Circuito *sup;
     obj_Localidad *loc = self_o->getLocalidadObj(self_o);
     obj_Seccion *seccion=self_o->getSeccionObj(self_o);
     printf("Circuito_Id:%d|Nombre:%s|Seccion:%s|Localidad:%s\n",self_o->info.Circuito_id,self_o->info.nombre_Circuito,seccion->getNombreSeccion(seccion),loc->getNombreLocalidad(loc));
}
//----------------------------------------------------
//implementacion de getters
static int getCircuitoId_Impl(void *self)
{ 
  obj_Circuito *obj = this(self);
  return obj->info.Circuito_id;
}
//----------------------------------------------------
static int getLocalidadId_Impl(void *self)
{ 
  obj_Circuito *obj = this(self);
  return obj->info.Localidad_id;
}
//----------------------------------------------------
static int getSeccionId_Impl(void *self)
{ 
  obj_Circuito *obj = this(self);
  return obj->info.Seccion_id;
}
//----------------------------------------------------
static char *getNombreCircuito_Impl(void *self)
{
	obj_Circuito *obj = this(self);
	return obj->info.nombre_Circuito;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreCircuito_Impl(void *self,char *nombre_Circuito)
{ 
	obj_Circuito *obj = this(self);
	strcpy(obj->info.nombre_Circuito,nombre_Circuito);
}
//----------------------------------------------------
static void setLetraCircuito_Impl(void *self,char *letra_Circuito)
{ 
	obj_Circuito *obj = this(self);
	strcpy(obj->info.letra,letra_Circuito);
}
//----------------------------------------------------
static void setSeccionId_Impl(void *self,int secc_id)
{ 
	obj_Circuito *obj = this(self);
	obj->info.Seccion_id = secc_id;
}
//----------------------------------------------------
static void setLocalidadId_Impl(void *self,int loc_id)
{ 
	obj_Circuito *obj = this(self);
	obj->info.Localidad_id = loc_id;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Circuito *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Circuito_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.letra );
   if(pos==2)
     snprintf( field, MAX_WHERE_SQL,"%s", obj->info.Seccion_id);
   if(pos==3)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Circuito );
   if(pos==4)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Localidad_id );
   strcat(cad,field);   
}
//----------------------------------------------------
// Implementar relaciones con otras entidades.
static void *getLocalidadObj_Impl(void *self)
{
	obj_Circuito *obj = (obj_Circuito *)self;
	if(obj->localidad_obj==NULL)
	{
		obj->localidad_obj = Localidad_new();		
		((obj_Localidad *)obj->localidad_obj)->findbykey(obj->localidad_obj,obj->getLocalidadId(obj));
	}
	return obj->localidad_obj;
}
//----------------------------------------------------
static void *getSeccionObj_Impl(void *self)
{
	obj_Circuito *obj = (obj_Circuito *)self;
	if(obj->seccion_obj==NULL)
	{
		obj->seccion_obj = Seccion_new();		
		((obj_Seccion *)obj->seccion_obj)->findbykey(obj->seccion_obj,obj->getSeccionId(obj));
	}
	return obj->seccion_obj;
}
//----------------------------------------------------
static void destroyInternal_Impl(void *self)
{
	obj_Circuito *obj = (obj_Circuito *)self;
	// liberar referencias internas
	if(obj->localidad_obj!=NULL) 
	  free(obj->localidad_obj);
	if(obj->seccion_obj!=NULL)
	  free(obj->seccion_obj);
}
//----------------------------------------------------
static void *init_Circuito(void *self)
{
  obj_Circuito *obj = (obj_Circuito *)self;
  //setear valores default
  obj->info.Circuito_id=0;
  CLEAR(obj->info.nombre_Circuito,MAX);
  obj->ds  = &table_Circuito;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getCircuitoId  	  = getCircuitoId_Impl;
  obj->getNombreCircuito = getNombreCircuito_Impl;
  obj->getLocalidadId = getLocalidadId_Impl;
  obj->getSeccionId = getSeccionId_Impl;
  /// setters  
  obj->setNombreCircuito = setNombreCircuito_Impl;
  obj->setLetraCircuito = setLetraCircuito_Impl;  
  obj->setSeccionId = setSeccionId_Impl;
  obj->setLocalidadId = setLocalidadId_Impl;  
  
  // relaciones con otras entidades  
  obj->localidad_obj = NULL;//setear en NULL en el constructor.
  obj->seccion_obj = NULL;//setear en NULL en el constructor.
  obj->getLocalidadObj = getLocalidadObj_Impl;
  obj->getSeccionObj = getSeccionObj_Impl;
  // debo implementar funcion de destroyInternal
  //incializacion de la interfaz de la entidad
  obj->destroyInternal = destroyInternal_Impl;
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_CircuitoImpl;
  obj->findAll =   findAll_CircuitoImpl;
  obj->saveObj =   saveObj_CircuitoImpl; 
  obj->toString =   toString_CircuitoImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Circuito
obj_Circuito *Circuito_new()
{
  return (obj_Circuito *)init_obj(sizeof(obj_Circuito), init_Circuito);
}
//----------------------------------------------------
