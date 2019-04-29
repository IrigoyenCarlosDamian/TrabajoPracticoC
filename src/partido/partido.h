#ifndef PART_INCLUDED
#define PART_INCLUDED
#define CNT_COL_PART 4
   
typedef struct {   
    int Partido_id;
	int nro_Partido;
	char nombre_Partido[MAX];	
	char color[MAX];		
} t_Partido;
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
    int (*findbykey)(void *self,int Partido_id);
    //-- getters
    int (*getPartidoId)(void *self);
    int (*getNroPartido)(void *self);
    char *(*getNombrePartido)(void *self);
    char *(*getColorPartido)(void *self);
    //-- setters
    void (*setNroPartido)(void *self,int);
    void (*setNombrePartido)(void *self,char *NombrePartido);
    void (*setColorPartido)(void *self,char *ColorsPartido);
    //estructura estatica
    t_Partido info;    
}obj_Partido;
// funcionalidad publica que se implementa en Partido.c
extern obj_Partido *Partido_new ();
// meta data para acceder a las Partidos
static t_column cols_Partido[CNT_COL_PART]={ {"id",t_int,4,true,true},{"nro_partido",t_int,4,false,false}, {"nom_partido",t_varchar,60,false,false}, {"color",t_varchar,60,false,false}};
// plantilla para la Partido.
static t_table table_Partido={"partido",CNT_COL_PART,0, cols_Partido,NULL};
#endif
