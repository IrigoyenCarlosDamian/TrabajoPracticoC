#ifndef LIST_PART_INCLUDED
#define LIST_PART_INCLUDED
#define CNT_COL_LIST_PART 3
   
typedef struct {   
    int ListaPartido_id;
	int Partido_id;
	char nombre_ListaPartido[MAX];
} t_ListaPartido;
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
    int (*findbykey)(void *self,int ListaPartido_id);
    //-- getters
    int (*getListaPartidoId)(void *self);
    int (*getPartidoId)(void *self);
    char *(*getNombreListaPartido)(void *self);    
    //-- setters
    void (*setPartidoId)(void *self,int);
    void (*setNombreListaPartido)(void *self,char *NombreListaPartido);
    
    //estructura estatica
    t_ListaPartido info;
	void *partido_obj;    
    //getter de Objetos Relacion
	void *(*getPartidoObj)(void *self);
}obj_ListaPartido;
// funcionalidad publica que se implementa en ListaPartido.c
extern obj_ListaPartido *ListaPartido_new ();
// meta data para acceder a las ListaPartidos
static t_column cols_ListaPartido[CNT_COL_LIST_PART]={ {"id",t_int,4,true,true},{"partido_id",t_int,4,false,false}, {"nom_lista",t_varchar,60,false,false}};
// plantilla para la ListaPartido.
static t_table table_ListaPartido={"lista_partido",CNT_COL_LIST_PART,0, cols_ListaPartido,NULL};
#endif
