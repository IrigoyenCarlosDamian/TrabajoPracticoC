#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "partido.h"

THIS(obj_Partido)

void CopyDataPartido (t_Partido *dest,t_Partido *orig)
{	
	dest->Partido_id = orig->Partido_id;	
	dest->nro_Partido = orig->nro_Partido;		
	strcpy( dest->nombre_Partido,orig->nombre_Partido);	
	strcpy( dest->color,orig->color);	
}

//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillPartidoFromDB(void **rw , int rowi,PGresult *res)
{
    t_Partido *dsPartido;
    dsPartido = &(( (t_Partido *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsPartido->Partido_id = atoi(PQgetvalue(res,rowi,0));
	dsPartido->nro_Partido = atoi(PQgetvalue(res,rowi,1));
    strcpy( dsPartido->nombre_Partido ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsPartido->color ,rtrim(PQgetvalue(res,rowi,3),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Partido
static void fillObjPartido(obj_Partido *p,  t_Partido rwitm)
{  
	CopyDataPartido(&(p->info),&rwitm);
	p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Partido
static void fillRowsPartido(void **list, int size,void *data)
{
     int i;
     obj_Partido *d;
     // pedir memoria para el listado en memoria de Partidos obj_Partido
    *list = (obj_Partido **)malloc(sizeof(obj_Partido*)* size);
    for(i=0;i<size;++i)
    {
      d = Partido_new();
      fillObjPartido(d,((t_Partido *)data)[i]);
      ((obj_Partido **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Partido(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Partido.
 t->rows = malloc(sizeof(t_Partido)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataPartido(&(((t_Partido *)t->rows)[i]),&(((t_Partido *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Partido(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Partido),fillPartidoFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_PartidoImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Partido), ((t_table*)((obj_Partido*)self)->ds)->rows,fillPartidoFromDB,fillRowsPartido);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Partido *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Partido
static int find_PartidoImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Partido *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Partido_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Partido(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Partido(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjPartido(obj,((t_Partido *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_PartidoImpl(void *self)
{
   obj_Partido *obj = this(self); 
   int newIdPartido;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdPartido);
   if(isNew_Obj)
     obj->info.Partido_id = newIdPartido;
   return retValue;
}
//----------------------------------------------------
static void toString_PartidoImpl(void *self)
{
     obj_Partido *self_o=this(self);
     obj_Partido *sup;     
     printf("Partido_id: %d  Partido:%s \n",self_o->info.nro_Partido,self_o->info.nombre_Partido);
}
//----------------------------------------------------
//implementacion de getters
static int getPartidoId_Impl(void *self)
{ 
  obj_Partido *obj = this(self);
  return obj->info.Partido_id;
}
//----------------------------------------------------
static int getNroPartido_Impl(void *self)
{ 
  obj_Partido *obj = this(self);
  return obj->info.nro_Partido;
}
//----------------------------------------------------
static char *getNombrePartido_Impl(void *self)
{
	obj_Partido *obj = this(self);
	return obj->info.nombre_Partido;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombrePartido_Impl(void *self,char *nombre_Partido)
{ 
	obj_Partido *obj = this(self);
	strcpy(obj->info.nombre_Partido,nombre_Partido);
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Partido *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Partido_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.nro_Partido );
   if(pos==2)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Partido );
   if(pos==3)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.color );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_Partido(void *self)
{
  obj_Partido *obj = (obj_Partido *)self;   
  //setear valores default
  obj->info.Partido_id=0;
  CLEAR(obj->info.nombre_Partido,MAX);
  obj->ds  = &table_Partido;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getPartidoId  	  = getPartidoId_Impl;
  obj->getNroPartido  	  = getNroPartido_Impl;  
  obj->getNombrePartido = getNombrePartido_Impl;  
  /// verificar lo faltante......
  /// setters  
  obj->setNombrePartido = setNombrePartido_Impl;  
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_PartidoImpl;
  obj->findAll =   findAll_PartidoImpl;
  obj->saveObj =   saveObj_PartidoImpl; 
  obj->toString =   toString_PartidoImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Partido
obj_Partido *Partido_new()
{
  return (obj_Partido *)init_obj(sizeof(obj_Partido), init_Partido);
}
//----------------------------------------------------
