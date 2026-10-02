#include <stdio.h>
#include <stdlib.h>
#include "TAD_Data.h"

struct dataType{
	int nodeType;
	union{
		str string;
		struct{
			struct dataType* data;
			struct dataType* next;
		};
	};
};

/* FUNCIONES DE CREACION */
Tdata create_str_ast(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = STR;
	n->string = NULL;
	return n;
}	
Tdata create_str_value(str value){
	Tdata n = create_str_ast();
	set_str_value(n, value);
	return n;
}
void set_str_value(Tdata elem, str value){
	if(elem == NULL || elem->nodeType != STR) return;
	str copy = value == NULL ? NULL : load2(value);
	free_str(elem->string);
	elem->string = copy;
}
str get_str_value(Tdata elem){
	if(elem == NULL || elem->nodeType != STR) return NULL;
	return elem->string;
}
Tdata data_first(Tdata collection){
	return collection;
}
Tdata data_next(Tdata iterator){
	if(iterator == NULL || (iterator->nodeType != SET && iterator->nodeType != LIST)) return NULL;
	return iterator->next;
}
Tdata data_element(Tdata iterator){
	if(iterator == NULL || (iterator->nodeType != SET && iterator->nodeType != LIST)) return NULL;
	return iterator->data;
}
Tdata create_set(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = SET;
	n->data = NULL;
	n->next = NULL;
	return n;
}
Tdata create_list(){
	Tdata n = (Tdata)malloc(sizeof(struct dataType));
	n->nodeType = LIST;
	n->data = NULL;
	n->next = NULL;
	return n;
}
void addEndTdata(Tdata *cab, Tdata nuevo){
	if(*cab == NULL){
		*cab = nuevo;
	}
	else{
		Tdata ult = *cab;
		while(ult->next != NULL)
			ult = ult->next;
		ult->next = nuevo;
	}
}	
	
/* FUNCIONES AUXILIARES */
Tdata clone(Tdata n) {
	if (n == NULL) 
		return NULL;
	Tdata nuevo = NULL;
	if (n->nodeType == STR) {
		nuevo = create_str_ast();
		if (n->string) set_str_value(nuevo, n->string);
	} 
	else if (n->nodeType == SET || n->nodeType == LIST) {
		Tdata aux = n;
		Tdata head = NULL;
		Tdata tail = NULL;
		Tdata nodo_lista;
		while (aux != NULL) {
			if (n->nodeType == SET) {
				nodo_lista = create_set();
			} else {
				nodo_lista = create_list();
			}
			nodo_lista->data = clone(aux->data);
			nodo_lista->next = NULL;
			if (head == NULL) {
				head = nodo_lista;
				tail = nodo_lista;
			} else {
				tail->next = nodo_lista;
				tail = nodo_lista;
			}
			aux = aux->next;
		}
		return head;
	}
	return nuevo;
}
int equals(Tdata a, Tdata b) {
	if (a == b) 
		return 1;
	if (a == NULL || b == NULL) 
		return 0;
	if (a->nodeType != b->nodeType) 
		return 0;
	
	if (a->nodeType == STR) {
		return compararStr(a->string, b->string) == 0;
	}
	return 0;
}

/* OPERACIONES SOBRE LIST */
void append_list(Tdata *list, Tdata e){
	Tdata nuevo = create_list();
	nuevo->data = clone(e);
	nuevo->next = NULL;
	
	addEndTdata(list, nuevo);
}
int length(Tdata cab ){
	if (cab == NULL ) {
		return 0;
	}
	else {
		return 1 + length(cab->next);
	}
}
Tdata copy_list(Tdata cab ){
	if(cab == NULL){
		return NULL;
	}
	else {
		Tdata aux ;
		aux = NULL;
		while (cab != NULL ){
			append_list(&aux , cab->data);
			cab = cab->next ;
		}
		return aux ;
	}
}
Tdata concat(Tdata list1 , Tdata list2 ){
	if (list1 == NULL && list2 == NULL){
		return NULL;
	}
	else {
		Tdata aux1 = NULL , aux2 = NULL , conca = NULL;
		aux1 = copy_list(list1);
		aux2 = copy_list(list2);
		if(list1 == NULL){
			return aux2;
		}
		else {
			if (list2 == NULL){
				return aux1;
			}
			else {
				conca = aux1;
				while (aux1->next != NULL){
					aux1 = aux1->next;
				}
				aux1->next = aux2 ;
				return conca;
			}
		}
	}
}
int seEncontro(Tdata dato1, Tdata dato2) {
	if (dato1->nodeType != dato2->nodeType) {
		return 0;
	}
	
	if (dato1->nodeType == STR) {
		if (compararStr(dato1->string, dato2->string)== 0) {
			return 1; 
		} else {
			return 0;
		}
	} 
	else if (dato1->nodeType == LIST || dato1->nodeType == SET) {
		
		while (dato1 != NULL && dato2 != NULL) {
			if (seEncontro(dato1->data, dato2->data) == 0) {
				return 0; 
			}
			dato1 = dato1->next;
			dato2 = dato2->next;
			
		}
		if (dato1 == NULL && dato2 == NULL) {
			return 1; 
		} else {
			return 0; 
		}
	}
	
	return 0; 
}
void search(Tdata cab , Tdata elemento){
	while (cab != NULL && seEncontro(cab->data,elemento)== 0 ){
		cab = cab->next;
		
	}
	if (cab == NULL){
		printf("No se encuentra en la lista ");
	}
	else {
		printf("Se encuentra en la lista ");
	}
}	
	
/* OPERACIONES SOBRE SET */
void append_set(Tdata *A, Tdata e){
	Tdata nuevo = create_set();
	nuevo->data = clone(e);
	nuevo->next = NULL;
	
	addEndTdata(A, nuevo);
}
int belongs(Tdata set, Tdata elem) {
	while (set != NULL) {
		if (equals(set->data, elem))
			return 1;
		set = set->next;
	}
	return 0;
}
void insert_set(Tdata* set, Tdata elem) {
	if (!belongs(*set, elem)) {
		append_set(set, elem);
	}
}
Tdata union_set(Tdata A, Tdata B) {
	Tdata resultado	= clone(A);
	
	if (B == NULL) 
		return resultado;
	
	Tdata aux = B;
	while (aux != NULL) {
		if (aux->data != NULL) {
			insert_set(&resultado, aux->data); 
		}
		aux = aux->next;
	}
	return resultado;
}
Tdata intersection_set(Tdata A, Tdata B) {
	Tdata resultado = NULL;
	Tdata auxA = A;
	while (auxA != NULL) {
		if (belongs(B, auxA->data)) {
			insert_set(&resultado, auxA->data);
		}
		auxA = auxA->next;
	}
	return resultado;
}
Tdata difference_set(Tdata A, Tdata B){
	Tdata resultado = NULL;
	
	while(A != NULL){
		
		if(!belongs(B, A->data))
			insert_set(&resultado, A->data);
		
		A = A->next;
	}
	
	return resultado;
}
int subset(Tdata A, Tdata B){
	while(A != NULL){
		
		if(!belongs(B, A->data))
			return 0;
		
		A = A->next;
	}
	
	return 1;
}
int equals_set(Tdata A, Tdata B){
	return subset(A,B) && subset(B,A);
}
void remove_set(Tdata* set, Tdata elem){
	Tdata act = *set;
	Tdata ant = NULL;
	
	while(act != NULL){
		if(equals(act->data, elem)){
			if(ant == NULL)
				*set = act->next;
			else
				ant->next = act->next;
			free(act);
			return;
		}
		ant = act;
		act = act->next;
	}
}
	
/* FUNCIONES DE CONVERSION */
Tdata strToList(Tdata s){
	if (s == NULL || s->nodeType != STR || s->string == NULL)
		return NULL;
	
	Tdata lista = NULL;
	
	str cad = s->string;
	int i = 0;
	
	while (cad[i] != '\0') {
		char aux[2];
		aux[0] = cad[i];
		aux[1] = '\0';
		
		Tdata nuevo = create_str_ast();
		set_str_value(nuevo, aux);
		
		append_list(&lista, nuevo);
		i++;
	}
	return lista;
}
Tdata listToStr(Tdata L){
	if (L == NULL) return NULL;
	
	int len = 0;
	Tdata aux = L;
	
	while (aux != NULL) {
		if (aux->data != NULL && aux->data->nodeType == STR) {
			len += longitudStr(aux->data->string);
		}
		aux = aux->next;
	}
	
	char *buffer = (char*)malloc(len + 1);
	buffer[0] = '\0';
	
	aux = L;
	while (aux != NULL) {
		if (aux->data != NULL && aux->data->nodeType == STR) {
			concatenarStr(buffer, aux->data->string);
		}
		aux = aux->next;
	}
	
	Tdata resultado = create_str_ast();
	resultado->string = buffer;
	
	return resultado;
}
Tdata listToSet(Tdata L){
	Tdata S = NULL;
	
	while(L != NULL)
	{
		insert_set(&S, L->data);
		L = L->next;
	}
	
	return S;
}
Tdata split(Tdata texto, Tdata sep){
	Tdata lista = NULL;
	str actual = load2(texto->string);
		
	str siguiente;
	while((siguiente = afterToken(actual, sep->string)) != NULL)
	{
		Tdata nodo = create_str_ast();
		nodo->string = beforeToken(actual, sep->string);
		
		append_list(&lista, nodo);
		
		free_str(actual);
		actual = siguiente;
	}
	
	Tdata ultimo = create_str_ast();
	set_str_value(ultimo, actual);
	
	append_list(&lista, ultimo);
	free_str(actual);
	
	return lista;
}
	
/* FUNCIONES DE MUESTRA */
void printStr(Tdata x){
	if(x != NULL && x->nodeType == STR)
		print_string(x->string);
}
void printList(Tdata L){
	if(L == NULL){
		printf("[]");
		return;
	}
	
	printf("[ ");
	
	Tdata aux = L;
	
	while(aux != NULL){
		
		printData(aux->data);
		
		if(aux->next != NULL)
			printf(", ");
		
		aux = aux->next;
	}
	
	printf(" ]");
}
void printSet(Tdata A){
	if(A == NULL){
		printf("{}");
		return;
	}
	
	printf("{ ");
	
	Tdata aux = A;
	
	while(aux != NULL){
		
		printData(aux->data);
		
		if(aux->next != NULL)
			printf(", ");
		
		aux = aux->next;
	}
	
	printf(" }");
}
void printData(Tdata x){
	if(x == NULL) return;
	
	switch(x->nodeType){
		
	case STR:
		printStr(x);
		break;
		
	case SET:
		printSet(x);
		break;
		
	case LIST:
		printList(x);
		break;
	}
}
Tdata setToStr(Tdata S){
	if (S == NULL) return NULL;
	
	int len = 0;
	int contcomas = 0;
	Tdata aux = S;
	
	while (aux != NULL) {
		if (aux->data != NULL && aux->data->nodeType == STR) {
			len += longitudStr(aux->data->string);
			contcomas = contcomas + 1;
		}
		aux = aux->next;
	}
	char *buffer;
	if ( contcomas == 1 ){
		buffer = (char*)malloc(len + 1);
		buffer[0] = '\0';
	}
	else {
		buffer = (char*)malloc(len + 1 + contcomas);
		buffer[0] = '\0';
	}
	aux = S;
	if (aux->data != NULL && aux->data->nodeType == STR) {
		concatenarStr(buffer, aux->data->string);
	}
	aux = aux->next; 
	while (aux != NULL) {
		concatenarStr(buffer, ",");
		
		if (aux->data != NULL && aux->data->nodeType == STR) {
			concatenarStr(buffer, aux->data->string);
		}
		aux = aux->next; 
	}
	Tdata resultado = create_str_ast();
	resultado->string = buffer;
	
	return resultado;
}
