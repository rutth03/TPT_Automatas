#include "String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

str load2(const char* s){
	str r = (str)malloc(strlen(s)+1);
	strcpy(r, s);
	return r;
}
void print_string(str s){
	printf("%s", s);
}
str toLowerCopy(str s){
	str r = (str)malloc(strlen(s) + 1);
	int i = 0;
	
	while (s[i] != '\0'){
		if (s[i] >= 'A' && s[i] <= 'Z'){
			r[i] = s[i] + 32;
		} else {
			r[i] = s[i];
		}
		i++;
	}
	
	r[i] = '\0';
	return r;
}
int compararStr(str a, str b){
	while(*a != '\0' && *b != '\0'){
		if(*a != *b) return *a - *b;
		a++;
		b++;
	}
	return *a - *b;
}
int longitudStr(str s){
	return strlen(s);
}
void concatenarStr(str dest, str src){
	strcat(dest, src);
}
str beforeToken(str texto, str token){
	char* pos = strstr(texto, token);
	
	if(pos == NULL) return NULL;
	
	int len = pos - texto;
	
	str resultado = malloc(len + 1);
	
	strncpy(resultado, texto, len);
	resultado[len] = '\0';
	
	return resultado;
}
str afterToken(str texto, str token){
	char* pos = strstr(texto, token);
	
	if(pos == NULL)
		return NULL;
	
	pos += strlen(token);
	
	return load2(pos);
}
int posicionPrimerCaracter(str texto, str caracteres){
	int i = 0;
	while(texto[i] != '\0'){
		int j = 0;
		while(caracteres[j] != '\0'){
			if(texto[i] == caracteres[j]){
				return i;
			}
			j++;
		}
		i++;
	}
	return i; 
}
