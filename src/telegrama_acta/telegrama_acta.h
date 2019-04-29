#ifndef TELEG_ACTA_INCLUDED
#define TELEG_ACTA_INCLUDED
#define CNT_COL_TELEG_ACTA 4 
   
typedef struct {   
    int ActaTelegrama_id;
	int Mesa_id;
	char fecha[MAXFECHA];
	bool cargado;
} t_ActaTelegrama;
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
    int (*findbykey)(void *self,int ActaTelegrama_id);
    //-- getters
    int (*getActaTelegramaId)(void *self);
    int (*getMesaId)(void *self);    
    //-- setters    
	void (*setMesaId)(void *self,int);
	void (*setFecha)(void *self,char *);
	void (*setCargado)(void *self,bool);
    //estructura estatica
    t_ActaTelegrama info;
	void *mesa_obj;
	//getter de Objetos Relacion
	void *(*getMesaObj)(void *self);
}obj_ActaTelegrama;
// funcionalidad publica que se implementa en ActaTelegrama.c
extern obj_ActaTelegrama *ActaTelegrama_new ();
// meta data para acceder a las ActaTelegrama
static t_column cols_ActaTelegrama[CNT_COL_TELEG_ACTA]={ {"id",t_int,4,true,true},{"nro_mesa",t_int,4,false,false}, {"fecha",t_varchar,10,false,false},{"cargado",t_int,4,false,false},};
// plantilla para la ActaTelegrama.
static t_table table_ActaTelegrama={"telegrama_acta",CNT_COL_TELEG_ACTA,0, cols_ActaTelegrama,NULL};
#endif
