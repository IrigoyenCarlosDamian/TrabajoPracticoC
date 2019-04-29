#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "categoria.h"

THIS(obj_Categoria)

void CopyDataCategoria (t_Categoria *dest,t_Categoria *orig)
{	
	dest->Categoria_id = orig->Categoria_id;
	strcpy( dest->nombre_Categoria,orig->nombre_Categoria);	
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
static void fillCategoriaFromDB(void **rw , int rowi,PGresult *res)
{
    t_Categoria *dsCategoria = &(( (t_Categoria *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD    
	dsCategoria->Categoria_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsCategoria->nombre_Categoria ,rtrim(PQgetvalue(res,rowi,1),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_Categoria
static void fillObjCategoria(obj_Categoria *p,  t_Categoria rwitm)
{  
      CopyDataCategoria(&(p->info),&rwitm);
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------

//Copiar en arreglo dinamico de tipo obj_Categoria
static void fillRowsCategoria(void **list, int size,void *data)
{
     int i;
     obj_Categoria *d;
     // pedir memoria para el listado en memoria de Categorias obj_Categoria
    *list = (obj_Categoria **)malloc(sizeof(obj_Categoria*)* size);
    for(i=0;i<size;++i)
    {
      d = Categoria_new();
      fillObjCategoria(d,((t_Categoria *)data)[i]);
      ((obj_Categoria **)*list)[i] = d;
    }
 }
 
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset:t_table
static void fill_dataset_Categoria(t_table *t,void *data, int sz)
{
 int i;
 //pedir memoria para contener listado de registros en formato dataset_Categoria.
 t->rows = malloc(sizeof(t_Categoria)* sz);
 t->cant_rows=sz;//cantidad de filas
     for(i=0;i<sz;++i)
     {
      CopyDataCategoria(&(((t_Categoria *)t->rows)[i]),&(((t_Categoria *)data)[i]));      
     }
}
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
static int exec_get_Categoria(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_Categoria),fillCategoriaFromDB);
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
static int findAll_CategoriaImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, sizeof(t_Categoria), ((t_table*)((obj_Categoria*)self)->ds)->rows,fillCategoriaFromDB,fillRowsCategoria);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_Categoria *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para Categoria
static int find_CategoriaImpl(void *self, int k) // se debe pasar en orden de aparicion de las columnas claves 
{
   int size=0; void *data;  
   char *sql;
   obj_Categoria *obj = this(self);
   //obtener cadena sql (select * from table where ...)las columnas claves estan igualadas a datos.   
   obj->info.Categoria_id=k;//setear dato clave
   sql = (char*)getFindByKeySQL((t_object*)self);
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((t_table*)obj->ds)->rows;   
   size = exec_get_Categoria(sql,&data);
   //liberar cadena sql
   free(sql);
   // completar 
   fill_dataset_Categoria(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjCategoria(obj,((t_Categoria *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
static bool saveObj_CategoriaImpl(void *self)
{
   obj_Categoria *obj = this(self); 
   int newIdCategoria;
   bool isNew_Obj = obj->getIsNewObj(self);
   bool retValue = saveObjImpl(self,&newIdCategoria);
   if(isNew_Obj)
     obj->info.Categoria_id = newIdCategoria;
   return retValue;
}
//----------------------------------------------------
static void toString_CategoriaImpl(void *self)
{
     obj_Categoria *self_o=this(self);
     obj_Categoria *sup;     
     printf("Categoria_id: %d  Categoria:%s \n",self_o->info.Categoria_id,self_o->info.nombre_Categoria);
}
//----------------------------------------------------
//implementacion de getters
static int getCategoriaId_Impl(void *self)
{ 
  obj_Categoria *obj = this(self);
  return obj->info.Categoria_id;
}
//----------------------------------------------------
static char *getNombreCategoria_Impl(void *self)
{
	obj_Categoria *obj = this(self);
	return obj->info.nombre_Categoria;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setNombreCategoria_Impl(void *self,char *nombre_Categoria)
{ 
	obj_Categoria *obj = this(self);
	strcpy(obj->info.nombre_Categoria,nombre_Categoria);
}
//----------------------------------------------------
static void getValueByPosImpl(void *self,char * cad, int pos)
{ 
   char field[MAX_WHERE_SQL];
   obj_Categoria *obj = this(self);
   t_table *tt=obj->ds;
   if(pos==0)
     snprintf( field, MAX_WHERE_SQL,"%d", obj->info.Categoria_id );
   if(pos==1)
     snprintf( field, MAX_WHERE_SQL,"'%s'", obj->info.nombre_Categoria );
   strcat(cad,field);   
}
//----------------------------------------------------
static void *init_Categoria(void *self)
{
  obj_Categoria *obj = (obj_Categoria *)self;   
  //setear valores default
  obj->info.Categoria_id=0;
  CLEAR(obj->info.nombre_Categoria,MAX);
  obj->ds  = &table_Categoria;  
  obj->isNewObj = true;//marcar como objeto nuevo, si se crea nueva instancia
  obj->getValueByPos = getValueByPosImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getCategoriaId  	  = getCategoriaId_Impl;
  obj->getNombreCategoria = getNombreCategoria_Impl;  
  /// setters  
  obj->setNombreCategoria = setNombreCategoria_Impl;  
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_CategoriaImpl;
  obj->findAll =   findAll_CategoriaImpl;
  obj->saveObj =   saveObj_CategoriaImpl; 
  obj->toString =   toString_CategoriaImpl;
  return obj;
}
//----------------------------------------------------
//constructor de Categoria
obj_Categoria *Categoria_new()
{
  return (obj_Categoria *)init_obj(sizeof(obj_Categoria), init_Categoria);
}
//----------------------------------------------------
