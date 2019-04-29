#ifndef CIRC_INCLUDED
#define CIRC_INCLUDED
#define CNT_COL_CIRC 5
#define MAXLET 2
   
typedef struct {   
    int Circuito_id;
	char letra[MAXLET];
	int Seccion_id;
    char nombre_Circuito[MAX];
	int Localidad_id;
} t_Circuito;
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
    int (*findbykey)(void *self,int Circuito_id);
    //-- getters
    int (*getCircuitoId)(void *self);
    char *(*getLetraCircuito)(void *self);
    int (*getSeccionId)(void *self);
    char *(*getNombreCircuito)(void *self);
    int (*getLocalidadId)(void *self);
    //-- setters
    void (*setLetraCircuito)(void *self,char *Letra);
    void (*setNombreCircuito)(void *self,char *NombreCircuito);
    void (*setSeccionId)(void *self,int idSeccion);
    void (*setLocalidadId)(void *self,int idLocalidad);
    //estructura estatica
    t_Circuito info;
    void *localidad_obj;
    void *seccion_obj;
    //getter de Objetos Relacion
	void *(*getLocalidadObj)(void *self);
	void *(*getSeccionObj)(void *self);
}obj_Circuito;
// funcionalidad publica que se implementa en Circuito.c
extern obj_Circuito *Circuito_new ();
// meta data para acceder a las Circuitos
static t_column cols_Circuito[CNT_COL_CIRC]={ {"id",t_int,4,true,true}, {"letra",t_varchar,1,false,false},{"secc_id",t_int,4,false,false}, {"nombre",t_varchar,50,false,false},{"localidad_id",t_int,4,false,false}};
// plantilla para la Circuito.
static t_table table_Circuito={"circuito",CNT_COL_CIRC,0, cols_Circuito,NULL};
#endif
