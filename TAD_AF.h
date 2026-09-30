#ifndef TAD_AF_H
#define TAD_AF_H

#include "TAD_Data.h"

typedef str State;
typedef str Symbol;

typedef struct transition{
	Symbol symbol;
	Tdata to;	
	struct transition* next;
} Transition;

typedef struct stateNode{
	State name;
	Transition* transitions;
	int isFinal;
	struct stateNode* next;
} StateNode;

typedef struct{
	StateNode* states;
	State q0;
	int deterministic;
} Automata;

Automata createAF();
void loadAutomata(Automata*, str);
void loadAutomataFromTXT(Automata*, str);
void printAutomata(Automata);
void printAutomataFormal(Automata);
Automata conversionAFD(Automata);
int validarCadena(Automata af, str);

#endif
