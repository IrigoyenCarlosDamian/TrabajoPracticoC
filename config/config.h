#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 

#ifndef __DEFOPTS__
#define __DEFOPTS__
typedef enum { false=0,true=1} bool;

#define MAX 80
#define MAXFECHA 17
#define MAX1 30
#define MAX2 80
#define MAX3 90
#define MAX4 2
#define MAX5 150
#define MAX30 30
#define MAX50 50
#define MAX10 10
#define MAX12 12
#define MAXOBS 200
#define MAX_SQL 800
#define MAX_WHERE_SQL 600
#define NULL_VALUE -100
#define SQL_STR "select $1 from $2"
#define SQL_INSERT "insert into $1 ($2) values ($3) RETURNING $4;"
#define SQL_UPDATE "update $1 set $2 where $3;"
#define SQL_MAX_ID "coalesce((select max($1)+1 from $2),1)"
#define WHERE_SQL(sql,c)   if(c!=NULL){\
     strcat(sql," where ");\
     strcat(sql, c );\
  }
#define THIS(clase) static clase *this(void *self){\
   return (clase*)self;\
}
#define CLEAR(t,max) memset(t, '\0',max)
#define TOK1 "$1"
#define TOK2 "$2"
#define TOK3 "$3"
#define TOK4 "$4"
#define SERVER "localhost"
#define PORT "5432"
#define DB "elector"
#define USER "postgres"
#define PASS "mastervb"
void *LocalidadObj_static; 
#endif
