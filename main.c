#include <stdio.h>
#include <stdlib.h>

#include "TAD_AF.h"
#include "TAD_Data.h"
#include "String.h"

void mostrarMenu() {
	printf("\n");
	printf("===============================================================\n");
	printf("                             MENU                              \n");
	printf("===============================================================\n");
	printf("  [a] AFD que acepta cadenas que comienzan con 'abb'\n");
	printf("  [b] AFD que acepta cadenas que terminan con 'ab'\n");
	printf("  [c] AFD que acepta cadenas que contienen 'aa' o 'bb'\n");
	printf("  [d] AFND que acepta cadenas que contienen 'aa' o 'bb'\n");
	printf("  [e] AFND que acepta cadenas que contienen '100', '1011', '111'\n");
	printf("  [f] AFD que acepta cadenas que contienen '100', '1011', '111'\n");
	printf("---------------------------------------------------------------\n");
	printf("  [s] Salir\n");
	printf("===============================================================\n");
	printf(">> Seleccione una opcion: ");
}
int main(){
	char opi;

	Automata A1 = createAF();
	Automata A2 = createAF();
	Automata A3 = createAF(); 
	Automata A4 = createAF(); 
	
	str nombreArchivo1 = load2("Automata1 - comienzan con 'abb'.txt"); 
	loadAutomataFromTXT(&A1, nombreArchivo1);
	
	str nombreArchivo2 = load2("Automata2 - terminan con 'ab'.txt"); 
	loadAutomataFromTXT(&A2, nombreArchivo2); 
	
	str nombreArchivo3 = load2("Automata3 - contiene 'aa' o 'bb'.txt");
	loadAutomataFromTXT(&A3, nombreArchivo3);
	printAutomataFormal(A3);
	
	str nombreArchivo4 = load2("Automata4 - contiene 100 1011 111.txt");
	loadAutomataFromTXT(&A4, nombreArchivo4);
	
	printf("\n----- CONVERSION AFND A AFD -----\n");
	printf("Conversion AFND (automata 3): contiene 'aa' o 'bb'\n");
	Automata AFD1 = conversionAFD(A3);
	printAutomataFormal(AFD1);
	
	//printf("Conversion AFND (automata 4): contiene 100 1011 111\n");
	Automata AFD2 = conversionAFD(A4);
	//printAutomataFormal(AFD3);
	
	do {
		mostrarMenu();
		scanf(" %c", &opi);
		
		switch(opi) {
		case 'a':
		case 'A': {
			printf("\nAutomata 1: cadenas que comienzan con 'abb'\n");
			
			str w1 = load2("abbaaaab");
			str w2 = load2("abaabb");
			
			printAutomataFormal(A1);
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(A1, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(A1, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		} 
		
		case 'b':
		case 'B': {
			printf("\nAutomata 2: cadenas que terminan con 'ab'\n");
		
			str w1 = load2("abbaaaab");
			str w2 = load2("abaabb");
			
			printAutomataFormal(A2);
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(A2, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(A2, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		}
		
		case 'c':
		case 'C': {
			printf("\nAutomata 3-AFD (resultado de conversion): cadenas que contienen 'aa' o 'bb'\n");
			printAutomataFormal(AFD1); 
			
			str w1 = load2("abababaa"); 
			str w2 = load2("abababa");
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(AFD1, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(AFD1, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		}
		
		case 'd':
		case 'D': {
			printf("\nAutomata 3: cadenas que contienen 'aa' o 'bb'\n");
			printAutomataFormal(A3); 
			
			str w1 = load2("abababaa"); 
			str w2 = load2("abababa");
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(A3, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(A3, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		}
		
		case 'e':
		case 'E': {
			printf("\nAutomata 4: cadenas que contienen '100', '1011' o '111'\n");
			
			str w1 = load2("00001000000");  
			str w2 = load2("000000101"); 
			
			printAutomataFormal(A4);
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(A4, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(A4, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		}
		case 'f':
		case 'F': {
			printf("\nAutomata 4-AFD (resultado de conversion): cadenas que contienen '100', '1011' o '111'\n");
			
			str w1 = load2("00001000000");  
			str w2 = load2("000000101"); 
			
			printAutomataFormal(AFD2);
			
			printf("cadena: "); print_string(w1);
			if(validarCadena(AFD2, w1)) printf(" aceptada\n");
			else printf(" rechazada\n");
			
			printf("cadena: "); print_string(w2);
			if(validarCadena(AFD2, w2)) printf(" aceptada\n");
			else printf(" rechazada\n");
			break;
		}
		case 's':
		case 'S':
			printf("\n Saliendo del programa... ¡Exitos con la presentacion!\n");
			break;
			
		default:
			printf("\n[ERROR] Opcion no valida. Por favor, ingresa una letra del menu.\n");
			break;
		}
		
	} while(opi != 's' && opi != 'S');
	
	return 0;
}
