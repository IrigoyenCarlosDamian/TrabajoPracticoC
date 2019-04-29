#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "telegrama_acta.h"
//Inclusiones para resolver obtencion de objetos relacion
#include "../mesa/mesa.h"

THIS(obj_ActaTelegrama)

void CopyDataActaTelegrama (t_ActaTelegrama *dest,t_ActaTelegrama *orig)
{	
	dest->ActaTelegrama_id = orig->ActaTelegrama_id;	
	dest->Mesa_id = orig->Mesa_id;
	strcpy( dest->fecha,orig->fecha);	
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillActaTelegramaFromDB(void **rw , int rowi,PGresult *res)
{
    t_ActaTelegrama *dsActaTelegrama;
    dsActaTelegrama = &(( (t_ActaTelegrama *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsActaTelegrama->ActaTelegrama_id = atoi(PQgetvalue(res,rowi,0));
	dsActaTelegrama->Mesa_id = atoi(PQgetvalue(res,rowi,1));
	strcpy(dsActaTelegrama->fecha , PQgetvalue(res,rowi,2));
	dsActaTelegrama->cargado = atoi(PQgetvalue(res,rowi,3));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_ActaTelegrama
static void fillObjActaTelegrama(obj_ActaTelegrama *p,  t_ActaTelegrama rwitm)
{  
	CopyDataActaTelegrama(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_ActaTelegrama
static void fillRowsActaTelegrama(void **list, int size,void *data)
{
     int i;
     obj_ActaTelegrama *d;
     // pedir memoria para el listado en memoria de ActaTelegramas obj_ActaTelegrama
    *list = (obj_ActaTelegrama **)malloc(sizeof(obj_ActaTelegrama*)* size);
    for(i=0;i<size;++i)
    {
      d = ActaTelegrama_new();
      fillObjActaTelegrama(d,((t_ActaTelegrama *)data)[i]);
      ((obj_ActaTelegrama **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_ActaTelegrama(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_ActaTelegrama.
 t->rows = malloc(sizeof(t_ActaTelegrama)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataActaTelegrama(&(((t_ActaTelegrama *)t->rows)[i]),&(((t_ActaTelegrama *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_ActaTelegrama(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_ActaTelegrama),fillActaTelegramaFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_ActaTelegramaImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_ActaTelegrama), ((t_table*)((obj_ActaTelegrama*)self)->ds)->rows,fillActaTelegramaFromDB,fillRowsActaTelegrama);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_ActaTelegrama *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para ActaTelegrama
static int find_ActaTelegramaImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_ActaTelegrama *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.ActaTelegrama_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_ActaTelegrama(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_ActaTelegrama(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjActaTelegrama(obj,((t_ActaTelegrama *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_ActaTelegramaImpl(void *self)
{
   obj_ActaTelegrama *obj = this(self); 
   int newIdActaTelegrama;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdActaTelegrama);
   if(isNew_Obj)
     obj->info.ActaTelegrama_id = newIdActaTelegrama;
   return retValue;
}
//----------------------------------------------------
static void toString_ActaTelegramaImpl(void *self)
{
     obj_ActaTelegrama *self_o=this(self);
     obj_ActaTelegrama *sup;     
     printf("ActaTelegrama_id: %d  \n",self_o->info.ActaTelegrama_id);
}
//----------------------------------------------------
//implementacion de getters
static int getActaTelegramaId_Impl(void *self)
{ 
  obj_ActaTelegrama *obj = this(self);
  return obj->info.ActaTelegrama_id;
}
//----------------------------------------------------
//implementacion de setters
static void setMesaId_Impl(void *self,int mesa_id)
{ 
	obj_ActaTelegrama *obj = this(self);
	obj->info.Mesa_id = mesa_id;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_ActaTelegrama *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.ActaTelegrama_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Mesa_id );
   if(pos==2)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.fecha );
   if(pos==3)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.cargado );
   strcat(cad,field);   
}
//----------------------------------------------------
static void destroyInternal_Impl(void *self)
{
	obj_ActaTelegrama *obj = (obj_ActaTelegrama *)self;
	// liberar referencias internas
	if(obj->mesa_obj!=NULL) 
	  free(obj->mesa_obj);	
}
//----------------------------------------------------
static void *init_ActaTelegrama(void *self)
{
  obj_ActaTelegrama *obj = (obj_ActaTelegrama *)self;   
  //setear valores default
  obj->info.ActaTelegrama_id=0;
  
  obj->ds  = &table_ActaTelegrama;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getActaTelegramaId  	  = getActaTelegramaId_Impl;
 // relaciones con otras entidades  
  obj->mesa_obj = NULL;//setear en NULL en el constructor.
  /// verificar lo faltante......
  /// setters  
  obj->setMesaId  	  = setMesaId_Impl;
 // debo implementar funcion de destroyInternal
  //incializacion de la interfaz de la entidad
  obj->destroyInternal = destroyInternal_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_ActaTelegramaImpl;
  obj->findAll =   findAll_ActaTelegramaImpl;
  obj->saveObj =   saveObj_ActaTelegramaImpl; 
  obj->toString =   toString_ActaTelegramaImpl;
  return obj;
}
//----------------------------------------------------
//constructor de ActaTelegrama
obj_ActaTelegrama *ActaTelegrama_new()
{
  return (obj_ActaTelegrama *)init_obj(sizeof(obj_ActaTelegrama), init_ActaTelegrama);
}
//----------------------------------------------------
