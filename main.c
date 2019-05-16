#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "config/config.h"
#include "lib/orm/orm.h"
#include "lib/utils/utils.h"

#include "src/categoria/categoria.h"
#include "src/circuito/circuito.h"
#include "src/seccion/seccion.h"
#include "src/localidad/localidad.h"
#include "src/escuela/escuela.h"
#include "src/partido/partido.h"
#include "src/lista_partido/lista_partido.h"
#include "src/lista_partido_localidad/lista_partido_localidad.h"
#include "src/mesa/mesa.h"
#include "src/telegrama_acta/telegrama_acta.h"
#include "src/telegrama_acta_votos/telegrama_acta_votos.h"

#define MAXARG 30
#define TOPE 9
#define MENOSELE "-l"

/********************/
int OrdAlfabetico(const void *a, const void *b) {
	
	obj_Escuela *a1;
    obj_Escuela *b1;
    a1 =  *(obj_Escuela **)a;
    b1 =  *(obj_Escuela **)b;
    return (b1->info.Escuela_id-a1->info.Escuela_id);
    //return (strcmp(a1->info.nombre_Escuela,b1->info.nombre_Escuela)); 
}
/*********************************/
void lista_partido_localidad(){
	obj_ListaPartidoLocalidad *listaPartidoLocalidad;
 	listaPartidoLocalidad =ListaPartidoLocalidad_new ();
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=listaPartidoLocalidad->findAll(listaPartidoLocalidad,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_ListaPartidoLocalidad**)list)[i];
    	((obj_ListaPartidoLocalidad*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(listaPartidoLocalidad);	
}
/*********************************/
void lista_partido(){
	obj_ListaPartido*listaPartido;
 	listaPartido =ListaPartido_new();
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=listaPartido->findAll(listaPartido,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_ListaPartido**)list)[i];
    	((obj_ListaPartido*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(listaPartido);
}
/********************************/
void seccion(){
	obj_Seccion *seccion;
 	seccion =Seccion_new();
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=seccion->findAll(seccion,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_Seccion**)list)[i];
    	((obj_Seccion*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(seccion);	
}

/*******************************/
void partido(){
	obj_Partido *partido;
 	partido =Partido_new();
 //	circuito.info.Circuito_id;
 	//ca
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=partido->findAll(partido,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_Partido**)list)[i];
    	((obj_Partido*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(partido);
}
/*****************************/
void mesa(){
    obj_Mesa *mesa;
 	mesa =Mesa_new();
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=mesa->findAll(mesa,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_Mesa**)list)[i];
    	((obj_Mesa*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(mesa);
	
}
/************************/
void localidad(){
	obj_Localidad *localidad;
	localidad=Localidad_new();
	int i=0,size=0;
	void *list;
	void *itm;
	size=localidad->findAll(localidad,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_Localidad **)list)[i];
    	((obj_Localidad*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(esc);
  	destroyObj(localidad);
}

/***********************/
void escuela(){
	obj_Escuela *escuela;
	escuela=Escuela_new();
	int i=0,size=0;
	void *list;
	void *itm;
	size=escuela->findAll(escuela,&list,NULL);
	printf("Antes de Ordenar\n");
	for(i=0;i<size;++i){
   		itm=((obj_Circuito **)list)[i];
    	((obj_Circuito*)itm)->toString(itm);
	}
	qsort((obj_Escuela**)list,size,sizeof(obj_Escuela*),OrdAlfabetico);
	printf("despues de ordenar\n");
	for(i=0;i<size;++i){
    	itm=((obj_Circuito **)list)[i];
    	((obj_Circuito*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(esc);
  	destroyObj(escuela);
}

/******************/
void circuito(){
	obj_Circuito *circuito;
 	circuito = Circuito_new();
 //	circuito.info.Circuito_id;
 	//ca
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=circuito->findAll(circuito,&list,NULL);
    for(i=0;i<size;++i){
    	itm=((obj_Circuito **)list)[i];
    	((obj_Circuito*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(circuito);
}
/***************************************/
void categoria(){
	obj_Categoria *categoria;
 	categoria=Categoria_new();
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=categoria->findAll(categoria,&list,NULL);
	for(i=0;i<size;++i){
    	itm=((obj_Categoria**)list)[i];
    	((obj_Categoria*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(categoria);
	
}

/***************************************/
int main(int argc, char *argv[]){
   	 int i=0,size=0;
  	 void *list;
 	 void *itm;
     char argumentos[][MAXARG]={
	 "CATEGORIA",
	 "CIRCUITO",
	 "ESCUELA",
	 "LOCALIDAD",
	 "MESA",
	 "PARTIDO",
	 "SECCION",
	 "LISTAPARTIDO",
	 "LISTAPARTIDOLOCALIDAD"};
   	if(argc>=2 && argc<=4){
		int bandera=0;
  		int i=0;
  		if(argc ==3 && !strcmp("-l",argv[1])){
  			for(i;i<TOPE;i++){
				if(!strcmp(argumentos[i],argv[2])){
					bandera=1;
					printf("HAS SELECCIONADO LA OPCION %s\n",argumentos[i],i);
					system("pause");
					switch (i){
						case 0:{
							printf("ENTRASTE A CATEGORIA\n");
							categoria();
							break;
						}
						case 1:{
							printf("ENTRASTE A CIRCUITO\n");
							circuito();
							break;
						}
						case 2:{
							printf("ENTRASTE A ESCUELA\n");
							escuela();
							break;
						}
						case 3:{
							printf("ENTRASTE A LOCALIDAD\n");
							localidad();
							break;
						}
						case 4:{
							printf("ENTRASTE A MESA\n");
							mesa();
							break;
						}
						case 5:{
							printf("ENTRASTE A PARTIDO\n");
							partido();
							break;
						}
						case 6:{
							printf("ENTRASTE A SECCION\n");
							seccion();
							break;
						}
						case 7:{
							printf("ENTRASTE A LISTA PARTIDO\n");
							lista_partido();
							break;
						}
						case 8:{
							printf("ENTRASTE A LISTA PARTIDO POR LOCALIDAD\n");
							lista_partido_localidad();
							break;
						}
					}
				}
			}
			if (bandera==0){
				int i=0;
				printf("Ingrese Alguno De Los Siguientes  argumentos:\n");
				for(i;i<TOPE;i++){
					printf("%s\n",argumentos[i],i);
				}
			}
		}
   }
   system("PAUSE");
   return 0;
}




  //obj_Seccion *secc;
 // int result;
//  int i=0,size=0;
  //void *list;
 // void *itm;
  //printf("%s \n",argv[1]);
  	/*
  //ejemplo de busqueda/ actualizacion
  cat = Categoria_new();
  cat->findbykey(cat,2);
  cat->setNombreCategoria(cat,"Presidente Comunal");
  cat->saveObj(cat);
  destroyObj(cat);
  */
  // ejemplo de listado de items

	//secc = Seccion_new();
  //size= secc->findAll(secc,&list,NULL);


  //for(i=0;i<size;++i)
  //{
    /*itm = ((obj_Seccion **)list)[i];
    ((obj_Seccion *)itm)->toString(itm);*/

  //}
  // librerar memoria
  //destroyObjList(list,size);
  //destroyObj(secc);
  


