#ifndef LIST_PART_LOC_INCLUDED
#define LIST_PART_LOC_INCLUDED
#define CNT_COL_LIST_PART_LOC 4 
   
typedef struct {   
    int ListaPartidoLocalidad_id;
	int ListaPartido_id;
	int Categoria_id;
	int Localidad_id;
} t_ListaPartidoLocalidad;
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
    int (*findbykey)(void *self,int ListaPartidoLocalidad_id);
    //-- getters
    int (*getListaPartidoLocalidadId)(void *self);
    int (*getListaPartidoId)(void *self);
    int(*getCategoriaId)(void *self);
    int (*getLocalidadId)(void *self);
    //-- setters
    void (*setListaPartidoId)(void *self,int);
    void (*setCategoriaId)(void *self,int);
    void (*setLocalidadId)(void *self,int);
    //--Relaciones
    void *(*getListaPartidoObj)(void *self);
	void *(*getCategoriaObj)(void *self);
    void *(*getLocalidadObj)(void *self);

    //--Variables internas para dar soporte a la relacion
    void *listaPartido_obj;
    void *categoria_obj;
    void *localidad_obj;
    //geter objet relacion
 
    //estructura estatica
    t_ListaPartidoLocalidad info;    
}obj_ListaPartidoLocalidad;
// funcionalidad publica que se implementa en ListaPartidoLocalidad.c
extern obj_ListaPartidoLocalidad *ListaPartidoLocalidad_new ();
// meta data para acceder a las ListaPartidoLocalidad
static t_column cols_ListaPartidoLocalidad[CNT_COL_LIST_PART_LOC]={ {"id",t_int,4,true,true},{"lista_partido_id",t_int,4,false,false}, {"categoria_id",t_int,4,false,false},{"localidad_id",t_int,4,false,false},};
// plantilla para la ListaPartidoLocalidad.
static t_table table_ListaPartidoLocalidad={"lista_partido_localidad",CNT_COL_LIST_PART_LOC,0, cols_ListaPartidoLocalidad,NULL};
#endif
