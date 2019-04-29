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

int main(int argc, char *argv[])
{	
  obj_Categoria *cat
  obj_Seccion *secc;
  int result;
  int i=0,size=0;
  void *list;
  	/*
  //ejemplo de busqueda/ actualizacion 
  cat = Categoria_new();
  cat->findbykey(cat,2);
  cat->setNombreCategoria(cat,"Presidente Comunal");
  cat->saveObj(cat);
  destroyObj(cat);
  */
  // ejemplo de listado de items
  /*  secc = Seccion_new();
  size= secc->findAll(secc,&list,NULL);
  for(i=0;i<size;++i)
  {
    itm = ((obj_Seccion **)list)[i];    
    ((obj_Seccion *)itm)->toString(itm);
  }
  // librerar memoria
  destroyObjList(list,size); 
  destroyObj(secc);
*/
  system("PAUSE");
  return 0;
}
