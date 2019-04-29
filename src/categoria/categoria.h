#ifndef CAT_INCLUDED
#define CAT_INCLUDED
#define CNT_COL_CAT 2
   
typedef struct {   
    int Categoria_id;
    char nombre_Categoria[MAX];
} t_Categoria;
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
    int (*findbykey)(void *self,int Categoria_id);
    //-- getters
    int (*getCategoriaId)(void *self);
    char *(*getNombreCategoria)(void *self);
    //-- setters
    void (*setNombreCategoria)(void *self,char *NombreCategoria);
    //estructura estatica
    t_Categoria info;    
}obj_Categoria;
// funcionalidad publica que se implementa en Categoria.c
extern obj_Categoria *Categoria_new ();
// meta data para acceder a las Categorias
static t_column cols_Categoria[CNT_COL_CAT]={ {"id",t_int,4,true,true}, {"nom_categoria",t_varchar,30,false,false}};
// plantilla para la Categoria.
static t_table table_Categoria={"categ_eleccion",CNT_COL_CAT,0, cols_Categoria,NULL};
#endif
