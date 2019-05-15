#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "Mesa.h"
//Inclusiones para resolver obtencion de objetos relacion
#include "../escuela/escuela.h"


THIS(obj_Mesa)

void CopyDataMesa (t_Mesa *dest,t_Mesa *orig)
{	
	dest->Mesa_id = orig->Mesa_id;	
	dest->nro_Mesa = orig->nro_Mesa;		
	dest->Circuito_id = orig->Circuito_id;
	dest->Escuela_id = orig->Escuela_id;
	dest->cant_electores = orig->cant_electores;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillMesaFromDB(void **rw , int rowi,PGresult *res)
{
    t_Mesa *dsMesa;
    dsMesa = &(( (t_Mesa *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsMesa->Mesa_id = atoi(PQgetvalue(res,rowi,0));
	dsMesa->nro_Mesa = atoi(PQgetvalue(res,rowi,1));
	dsMesa->Circuito_id = atoi(PQgetvalue(res,rowi,2));
	dsMesa->Escuela_id = atoi(PQgetvalue(res,rowi,3));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Mesa
static void fillObjMesa(obj_Mesa *p,  t_Mesa rwitm)
{  
	CopyDataMesa(&(p->info),&rwitm);
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_Mesa
static void fillRowsMesa(void **list, int size,void *data)
{
     int i;
     obj_Mesa *d;
     // pedir memoria para el listado en memoria de Mesas obj_Mesa
    *list = (obj_Mesa **)malloc(sizeof(obj_Mesa*)* size);
    for(i=0;i<size;++i)
    {
      d = Mesa_new();
      fillObjMesa(d,((t_Mesa *)data)[i]);
      ((obj_Mesa **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Mesa(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Mesa.
 t->rows = malloc(sizeof(t_Mesa)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
	   CopyDataMesa(&(((t_Mesa *)t->rows)[i]), &(((t_Mesa *)data)[i]));
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Mesa(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Mesa),fillMesaFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_MesaImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Mesa), ((t_table*)((obj_Mesa*)self)->ds)->rows,fillMesaFromDB,fillRowsMesa);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Mesa *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Mesa
static int find_MesaImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Mesa *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Mesa_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Mesa(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Mesa(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjMesa(obj,((t_Mesa *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_MesaImpl(void *self)
{
   obj_Mesa *obj = this(self); 
   int newIdMesa;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdMesa);
   if(isNew_Obj)
     obj->info.Mesa_id = newIdMesa;
   return retValue;
}
//----------------------------------------------------
static void toString_MesaImpl(void *self)
{
   	 obj_Mesa *self_o=this(self);
   	 //obj_Escuela *escuela =self_o->getEscuelaObj(self_o);
   	 obj_Escuela *sup;
   	 obj_Escuela *escuela=self_o->getEscuelaObj(self_o);// se invoca asi 
     printf("Mesa_id: %d  nro Mesa:%d  Escuela: %s \n ",self_o->info.Mesa_id,self_o->info.nro_Mesa,escuela->info.nombre_Escuela);
}
//----------------------------------------------------
//implementacion de getters
static int getMesaId_Impl(void *self)
{ 
  obj_Mesa *obj = this(self);
  return obj->info.Mesa_id;
}
//----------------------------------------------------
static int getNroMesa_Impl(void *self)
{ 
  obj_Mesa *obj = this(self);
  return obj->info.nro_Mesa;
}
//----------------------------------------------------
static int getCircuitoId_Impl(void *self)
{
	obj_Mesa *obj = this(self);
	return obj->info.Circuito_id;	
}
//----------------------------------------------------
static int getEscuelaId_Impl(void *self)
{
	obj_Mesa *obj = this(self);
	return obj->info.Escuela_id;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNroMesa_Impl(void *self,int Nro_Mesa)
{ 
	obj_Mesa *obj = this(self);
	obj->info.nro_Mesa = Nro_Mesa;
}
//----------------------------------------------------
static void setCircuitoId_Impl(void *self,int CircuitoId)
{ 
	obj_Mesa *obj = this(self);
	obj->info.Circuito_id = CircuitoId;
}
//----------------------------------------------------
static void setEscuelaId_Impl(void *self,int EscuelaId)
{ 
	obj_Mesa *obj = this(self);
	obj->info.Escuela_id = EscuelaId;
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Mesa *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Mesa_id );
	if(pos==1)
		snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Escuela_id);
/*   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Mesa );*/
   strcat(cad,field);   
}


///--- Implementar relaciones con otras entidades

///////////////////////////////////
static void *getEscuelaObj_Impl(void *self){//este es el metodo que busca la clave foranea 
	
	obj_Mesa *obj = this(self); //casteo al tipo de dato del .c 
    obj_Escuela *esc; // castelo al tipo de dato buscado 
    esc = Escuela_new(); //"nueva instacia"
    esc->findbykey(esc,obj->info.Escuela_id); // forma de busqueda 
    return esc;
	
}


//----------------------------------------------------
static void *init_Mesa(void *self)
{
  obj_Mesa *obj = (obj_Mesa *)self;   
  //setear valores default
  obj->info.Mesa_id=0;
  obj->info.nro_Mesa=0;
  obj->ds  = &table_Mesa;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getMesaId  	  = getMesaId_Impl;
  obj->getNroMesa = getNroMesa_Impl;  
  obj->getCircuitoId = getCircuitoId_Impl;
  obj->getEscuelaId = getEscuelaId_Impl;
  obj->getEscuelaObj = getEscuelaObj_Impl;
  /// setters  
  obj->setNroMesa = setNroMesa_Impl;
  obj->setCircuitoId = setCircuitoId_Impl;
  obj->setEscuelaId = setEscuelaId_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_MesaImpl;
  obj->findAll =   findAll_MesaImpl;
  obj->saveObj =   saveObj_MesaImpl; 
  obj->toString =   toString_MesaImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Mesa
obj_Mesa *Mesa_new()
{
  return (obj_Mesa *)init_obj(sizeof(obj_Mesa), init_Mesa);
}
//----------------------------------------------------
