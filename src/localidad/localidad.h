#ifndef LOC_INCLUDED
#define LOC_INCLUDED
#define CNT_COL_LOC 2
#define MAXLET 2
   
typedef struct {   
    int Localidad_id;
	char nombre_Localidad[MAX];	
} t_Localidad;
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
    int (*findbykey)(void *self,int Localidad_id);
    //-- getters
    int (*getLocalidadId)(void *self);
    char *(*getNombreLocalidad)(void *self);
    //-- setters
    void (*setNombreLocalidad)(void *self,char *NombreLocalidad);
    //estructura estatica
    t_Localidad info;    
}obj_Localidad;
// funcionalidad publica que se implementa en Localidad.c
extern obj_Localidad *Localidad_new ();
// meta data para acceder a las Localidads
static t_column cols_Localidad[CNT_COL_LOC]={ {"id",t_int,4,true,true}, {"nom_localidad",t_varchar,50,false,false}};
// plantilla para la Localidad.
static t_table table_Localidad={"localidad",CNT_COL_LOC,0, cols_Localidad,NULL};
#endif
