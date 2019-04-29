#ifndef SECC_INCLUDED
#define SECC_INCLUDED
#define CNT_COL_SECC 2
#define MAXLET 2
   
typedef struct {   
    int Seccion_id;
	char nombre_Seccion[MAX];	
} t_Seccion;
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
    int (*findbykey)(void *self,int Seccion_id);
    //-- getters
    int (*getSeccionId)(void *self);
    char *(*getNombreSeccion)(void *self);
    //-- setters
    void (*setNombreSeccion)(void *self,char *NombreSeccion);
    //estructura estatica
    t_Seccion info;    
}obj_Seccion;
// funcionalidad publica que se implementa en Seccion.c
extern obj_Seccion *Seccion_new ();
// meta data para acceder a las Seccions
static t_column cols_Seccion[CNT_COL_SECC]={ {"id",t_int,4,true,true}, {"nombre",t_varchar,50,false,false}};
// plantilla para la Seccion.
static t_table table_Seccion={"seccion",CNT_COL_SECC,0, cols_Seccion,NULL};
#endif
