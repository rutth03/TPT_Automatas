#ifndef STRING_H
#define STRING_H

typedef char* str;

str load2(const char*);
void print_string(str );
int compararStr(str, str);
int longitudStr(str);
void concatenarStr(str, str);
str beforeToken(str, str);
str afterToken(str, str);
int posicionPrimerCaracter(str, str);
#endif
