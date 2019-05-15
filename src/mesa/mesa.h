#ifndef Mesa_INCLUDED
#define Mesa_INCLUDED
#define CNT_COL_Mesa 5
   
typedef struct {   
	int Mesa_id;
	int nro_Mesa;
	int Circuito_id;
	int Escuela_id;
	int cant_electores;
} t_Mesa;
//----------------------------------------------------
typedef struct {
	t_table *ds;
	int  (*findAll)(void *self, void **list,char *criteria); 
	void (*toString)(void *self);
	bool (*getIsNewObj)(void *self);
	bool (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
	void (*getValueByPos)(void *self,char *cad, int pos);
	bool isNewObj;
	//// FIN CORRESPONDENCIA COMUN CON t_object
	int (*findbykey)(void *self,int Mesa_id);
	//-- getters
	int (*getMesaId)(void *self);
	int (*getNroMesa)(void *self);
	int (*getCircuitoId)(void *self);
	int (*getEscuelaId)(void *self);
	//-- setters
	void (*setNroMesa)(void *self,int);
	void (*setCircuitoId)(void *self,int);
	void (*setEscuelaId)(void *self,int);
	void (*setCantElectores)(void *self,int);
	//--relaciones
	void *(*getEscuelaObj)(void *self);
	void *(*getCircuitoObj)(void *self);
	//--variables para poder dar soporte a la relacion
	void *circuito_obj;
	void *escuela_obj;
	//estructura estatica
	t_Mesa info;    
	
}obj_Mesa;
// funcionalidad publica que se implementa en Mesa.c
extern obj_Mesa *Mesa_new ();
// meta data para acceder a las Mesas
static t_column cols_Mesa[CNT_COL_Mesa]={ {"id",t_int,4,true,true},{"nro_mesa",t_int,4,false,false},{"circ_id",t_int,4,false,false},{"esc_id",t_int,4,false,false},{"cant_electores",t_int,4,false,false}};
// plantilla para la Mesa.
static t_table table_Mesa={"mesa",CNT_COL_Mesa,0, cols_Mesa,NULL};
#endif
