#ifndef TELEG_ACTA_VOT_INCLUDED
#define TELEG_ACTA_VOT_INCLUDED
#define CNT_COL_TELEG_ACTA_VOT 4 
   
typedef struct {   
    int ActaTelegramaVotos_id;
	int ActaTelegrama_id;		
	int ListaPartidoLocalidad_id;
	int votos;
} t_ActaTelegramaVotos;
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
    int (*findbykey)(void *self,int ActaTelegramaVotos_id);
    //-- getters
    int (*getActaTelegramaVotosId)(void *self);
    int (*getMesaId)(void *self);
    
    //-- setters
    void (*setListaMesaId)(void *self,int);    
    
    //estructura estatica
    t_ActaTelegramaVotos info;    
}obj_ActaTelegramaVotos;
// funcionalidad publica que se implementa en ActaTelegramaVotos.c
extern obj_ActaTelegramaVotos *ActaTelegramaVotos_new ();
// meta data para acceder a las ActaTelegramaVotos
static t_column cols_ActaTelegramaVotos[CNT_COL_TELEG_ACTA_VOT]={ {"id",t_int,4,true,true},{"teleg_id",t_int,4,false,false},{"lista_partido_loc_id",t_int,4,false,false}, {"votos",t_int,4,false,false}};
// plantilla para la ActaTelegramaVotos.
static t_table table_ActaTelegramaVotos={"telegrama_acta_votos",CNT_COL_TELEG_ACTA_VOT,0, cols_ActaTelegramaVotos,NULL};
#endif
