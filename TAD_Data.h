#ifndef TAD_DATA_H
#define TAD_DATA_H

#include "String.h"

#define STR 1
#define SET 2
#define LIST 3

typedef struct dataType* Tdata;

/* FUNCIONES DE CREACION */
Tdata create_str_ast();
Tdata create_str_value(str value);
Tdata create_list();
Tdata create_set();
void set_str_value(Tdata elem, str value);
str get_str_value(Tdata elem);

/* ITERACION Y EXTRACCION ABSTRACTA */
Tdata data_first(Tdata collection);
Tdata data_next(Tdata iterator);
Tdata data_element(Tdata iterator);

/* AUXILIARES */
Tdata clone(Tdata);
int equals(Tdata, Tdata);
void printData(Tdata);
Tdata split(Tdata, Tdata);

/* OPERACIONES SOBRE LIST */
void append_list(Tdata*, Tdata);
int length(Tdata);
Tdata copy_list(Tdata);
Tdata concat(Tdata, Tdata);
void search(Tdata, Tdata);

/* OPERACIONES SOBRE SET */
void append_set(Tdata*, Tdata );
int belongs(Tdata, Tdata);
void insert_set(Tdata*, Tdata);	
Tdata union_set(Tdata, Tdata);
Tdata intersection_set(Tdata, Tdata);
Tdata difference_set(Tdata, Tdata);
int subset(Tdata, Tdata);
int equals_set(Tdata, Tdata);
void remove_set(Tdata*, Tdata);

/* FUNCIONES DE CONVERSION */
Tdata listToStr(Tdata);
Tdata strToList(Tdata);
Tdata listToSet(Tdata);
Tdata setToStr(Tdata);

#endif
