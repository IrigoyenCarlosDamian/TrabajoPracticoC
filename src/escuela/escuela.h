#ifndef ESC_INCLUDED
#define ESC_INCLUDED
#define CNT_COL_ESC 5
   
typedef struct {   
    int Escuela_id;
	int Circuito_id;
	char nombre_Escuela[MAX];	
	char dir_Escuela[MAX];	
	int Localidad_id;
} t_Escuela;
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
    int (*findbykey)(void *self,int Escuela_id);
    //-- getters
    int (*getEscuelaId)(void *self);
    int (*getCircuitoId)(void *self);
    int (*getLocalidadId)(void *self);
    char *(*getNombreEscuela)(void *self);
    char *(*getDireccionEscuela)(void *self);
    //-- setters
    void (*setCircuitoId)(void *self,int CircuitoId);
    void (*setLocalidadId)(void *self,int LocalidadId);
    void (*setNombreEscuela)(void *self,char *NombreEscuela);
    void (*setDireccionEscuela)(void *self,char *Direccion);
    //estructura estatica
    t_Escuela info;
    void *localidad_obj;
    void *circuito_obj;
    //getter de Objetos Relacion
	void *(*getLocalidadObj)(void *self);
	void *(*getCircuitoObj)(void *self);
}obj_Escuela;
// funcionalidad publica que se implementa en Escuela.c
extern obj_Escuela *Escuela_new ();
// meta data para acceder a las Escuelas
static t_column cols_Escuela[CNT_COL_ESC]={ {"id",t_int,4,true,true},{"circ_id",t_int,4,false,false}, {"nom_escuela",t_varchar,60,false,false}, {"direccion",t_varchar,80,false,false},{"localidad_id",t_int,4,false,false}};
// plantilla para la Escuela.
static t_table table_Escuela={"escuela",CNT_COL_ESC,0, cols_Escuela,NULL};
#endif
