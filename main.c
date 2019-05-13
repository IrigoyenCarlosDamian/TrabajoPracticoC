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

#define MAXARG 15
#define TOPE 7
#define MENOSELE "-l"


void categoria(){
	obj_Categoria *cat;
 	cat = Categoria_new();
 	//ca
	int i=0,size=0;
  	void *list;
 	void *itm;
	size=cat->findAll(cat,&list,NULL);
    for(i=0;i<size;++i)
  	{

    itm=((obj_Categoria **)list)[i];
    ((obj_Categoria*)itm)->toString(itm);
	}
  	// librerar memoria
  	destroyObjList(list,size);
  	//destroyObj(secc);
  	destroyObj(cat);
}



int main(int argc, char *argv[])
{
   char argumentos[][MAXARG]={"categoria","circuito","escuela","localidad","mesa","partido","seccion"};
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
							categoria();
							break;
						}
						case 1:{

							printf("ENTRASTE A CIRCUITO\n");
							break;
						}
						case 2:{
							printf("ENTRASTE A ESCUELA\n");
							break;
						}
						case 3:{
							printf("ENTRASTE A LOCALIDAD\n");
							break;
						}
						case 4:{
							printf("ENTRASTE A MESA\n");
							break;
						}
						case 5:{
							printf("ENTRASTE A PARTIDO\n");
							break;
						}
						case 6:{
							printf("ENTRASTE A SECCION\n");
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
  {
    /*itm = ((obj_Seccion **)list)[i];
    ((obj_Seccion *)itm)->toString(itm);*/

  }
  // librerar memoria
  //destroyObjList(list,size);
  //destroyObj(secc);
  system("PAUSE");
  return 0;
}

