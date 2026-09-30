#include <stdio.h>
#include <stdlib.h>
#include "TAD_AF.h"

// ----- FUNCION CREACION DE AF -----
Automata createAF(){
	Automata A;
	A.states = NULL;
	A.q0 = NULL;
	A.deterministic = -1;
	
	return A;
}
	
// ----- MODULOS AUXILIARES DE CREACION -----
StateNode* findState(StateNode *states, str nombre){
	while(states != NULL){
		if(compararStr(states->name, nombre) == 0)
			return states;
		states = states->next;
	}
	return NULL;
}
int symbolBelongs(Tdata alphabet, Symbol symbol){
	Tdata iter = data_first(alphabet);
	while(iter != NULL){
		if(compararStr(get_str_value(data_element(iter)), symbol) == 0)
			return 1;
		iter = data_next(iter);
	}
	return 0;
}
void addTransition(Transition **head, Transition *nuevo){
	if(*head == NULL){
		*head = nuevo;
	} else {
		Transition *aux = *head;
		while(aux->next != NULL) aux = aux->next;
		aux->next = nuevo;
	}
}
Tdata createDestinationSet(str texto, Automata *af){
	if(compararStr(texto,"{}")==0) return NULL;
	// ejemplo "{q0,q1}"
	str sinLlave = afterToken(texto,"{"); // sinLlave = "q0,q1}"
	str contenido = beforeToken(sinLlave,"}"); // contenido = "q0,q1"
	Tdata aux = create_str_value(contenido);
	Tdata coma = create_str_value(",");
	Tdata lista = split(aux,coma); // lista = ["q0", "q1"]
	Tdata resultado = NULL; // resultado = { }
	Tdata iter = data_first(lista);
	while(iter != NULL){
		Tdata estado = data_element(iter);
		if(findState(af->states, get_str_value(estado)) != NULL){
			insert_set(&resultado, estado); // inserto cada dato de la lista en el conjunto, no permite repetidos
		} else {
			printf("ERROR: estado: "); print_string(get_str_value(estado)); printf(" fuera del conjunto de estados\n");
		}
		iter = data_next(iter);
	}
	free(sinLlave);
	
	return resultado; // resultado = {"q0", "q1"}
}
Transition* createTransition(str symbol, Tdata destinos){
	Transition *t;
	t = (Transition*)malloc(sizeof(Transition));
	t->symbol = load2(symbol);
	t->to = destinos;
	t->next = NULL;
	return t;
}
void loadOneTransition(Automata *af, Tdata alphabet, str texto){
	str origen;
	str simbolo;
	str resto;
	str destinosTxt;
	
	// ejemplo texto = "q0-a-{q0,q1}"
	origen = beforeToken(texto,"-"); // origen = "q0" 
	resto = afterToken(texto,"-"); // resto = "a-{q0,q1}"
	simbolo = beforeToken(resto,"-"); // simbolo = "a"
	destinosTxt = afterToken(resto,"-"); // destionsTxt = "{q0,q1}"
	
	StateNode *estadoOrigen;
	estadoOrigen = findState(af->states, origen);  // busco por 'origen' el Nodo Estado al que le quiero insertar las transiciones
	
	if(estadoOrigen == NULL){
		printf("ERROR: estado "); print_string(origen); printf(" inexistente\n");
		return;
	}
	if(!symbolBelongs(alphabet, simbolo)){
		printf("ERROR: simbolo: "); print_string(simbolo); printf(" fuera del alfabeto\n");
		return;
	}
	Tdata destinos;
	destinos = createDestinationSet(destinosTxt, af); // de "{q0,q1}" obtengo {"q0","q1"}
	
	if(destinos == NULL && !af->deterministic) return; // AFND:{} => no guardar
	
	Transition *nueva;
	nueva = createTransition(simbolo,destinos);
	addTransition(&(estadoOrigen->transitions), nueva);
}
void loadTransitions(Automata *af, Tdata alphabet, Tdata transitions){
	Tdata coma = create_str_value(",");
	Tdata listaAlfabeto = split(data_element(alphabet), coma); // ["a,b,c"] ->["a","b","c"]
	
	Tdata barra = create_str_value("|");
	
	Tdata lista = split(data_element(transitions), barra); 
	// ["q0-a-{q0,q1}","q0-b-{q0}","q0-c-{q0}","q1-a-{}","q1-b-{q2}","q1-c-{}","q2-a-{q3}","q2-b-{}","q2-c-{}","q3-a-{q3}","q3-b-{q3}","q3-c-{q3}"]
	
	Tdata iter = data_first(lista);
	while(iter != NULL){ 
		loadOneTransition(af,listaAlfabeto,get_str_value(data_element(iter))); 
		/* ejemplo la primera vez evalua (af, ["a","b","c"], "q0-a-{q0,q1}")
		la segunda vez evalua (af, ["a","b","c"], "q0-b-{q0}") ... */
		iter = data_next(iter);
	}
}
void loadStates(Automata *af, Tdata states){
	Tdata coma = create_str_value(",");
	Tdata listaEstados = split(data_element(states), coma); // ["q0,q1,q2,q3"] -> ["q0","q1","q2","q3"]
	
	// crear Nodos Estados vacios
	StateNode *ultimo = NULL;
	Tdata iter = data_first(listaEstados);
	while(iter != NULL){
		str nombreEstado = get_str_value(data_element(iter));
		// Si el estado no existe todavia, lo agrego
		if(findState(af->states, nombreEstado) == NULL){
			StateNode *nuevo = malloc(sizeof(StateNode));
			nuevo->name = load2(nombreEstado);
			nuevo->transitions = NULL;
			nuevo->isFinal = 0;
			nuevo->next = NULL;
			
			if(af->states == NULL){
				af->states = nuevo;
			}else{
				ultimo->next = nuevo;
			}
			
			ultimo = nuevo;
		}
		iter = data_next(iter);
	}
}
void loadFinalStates(Automata *af, Tdata finals){
	Tdata coma = create_str_value(",");
	
	Tdata listaFinales = split(data_element(finals), coma); // ["q3"]
	
	Tdata iter = data_first(listaFinales);
	while(iter != NULL){
		str nombreFinal = get_str_value(data_element(iter));
		StateNode *estado = af->states;
		int encontrado = 0;
		
		while(estado != NULL){
			if(compararStr(estado->name, nombreFinal) == 0){
				estado->isFinal = 1;
				encontrado = 1;
				break;
			}
			estado = estado->next;
		}
		if(!encontrado){
			printf("ERROR: estado final ");
			print_string(nombreFinal);
			printf(" inexistente\n");
		}
		
		iter = data_next(iter);
	}
}
// ----- FUNCIONES DE CARGA DE AUTOMATA -----
void loadAutomataFields(Automata *af,
						Tdata deterministic,
						Tdata states,
						Tdata alphabet,
						Tdata transitions,
						Tdata initial,
						Tdata finals){
	
	af->deterministic = atoi(get_str_value(data_element(deterministic)));
	
	// cargar estados
	loadStates(af, states);
	
	// estado inicial 
	str nombreInicial = get_str_value(data_element(initial));
	if(findState(af->states, nombreInicial) == NULL){
		printf("ERROR: estado inicial ");
		printData(data_element(initial));
		printf(" inexistente\n");
	}else{
		af->q0 = load2(nombreInicial);
	}
	
	// estados finales
	loadFinalStates(af, finals);
	
	// transiciones
	loadTransitions(af, alphabet, transitions);
}
void loadAutomata(Automata *af, str original){
	Tdata cadena = create_str_value(original);
	
	Tdata token = create_str_value(";");
	
	Tdata lista = split(cadena,token);
	/*	[
	"0",
	"q0,q1,q2,q3",
	"a,b,c",
	"q0-a-{q0,q1}|q0-b-{q0}|q0-c-{q0}|q1-a-{}|q1-b-{q2}|q1-c-{}|q2-a-{q3}|q2-b-{}|q2-c-{}|q3-a-{q3}|q3-b-{q3}|q3-c-{q3}"
	"q0"
	"q3"
	] */
	
	Tdata estados = data_next(data_first(lista)); // ["q0,q1,q2,q3"]
	Tdata alfabeto = data_next(estados); // ["a,b,c"]
	Tdata transiciones = data_next(alfabeto);
	Tdata inicial = data_next(transiciones);
	Tdata finales = data_next(inicial);
	
	loadAutomataFields(
					   af,
					   lista,
					   estados,
					   alfabeto,
					   transiciones,
					   inicial,
					   finales
					   );
}
void loadAutomataFromTXT(Automata *af, str nombreArchivo){
	FILE *f = fopen(nombreArchivo, "r");
	
	if(f == NULL){
		printf("ERROR: no se pudo abrir el archivo\n");
		return;
	}
	
	char buffer[5000];
	Tdata campos[6];
	for(int i = 0; i < 6; i++){
		if(fgets(buffer, sizeof(buffer), f) == NULL){
			printf("ERROR: archivo incompleto\n");
			fclose(f);
			return;
		}
		buffer[posicionPrimerCaracter(buffer, "\n")] = '\0';
		campos[i] = NULL;
		append_list(&campos[i], create_str_value(buffer));
	}
	fclose(f);
	loadAutomataFields(
					   af,
					   campos[0],
					   campos[1],
					   campos[2],
					   campos[3],
					   campos[4],
					   campos[5]
					   );
}

// ----- MODULOS PARA RECUPERAR Q,F,qo,Sigma -----
Tdata Rec_sigma(Automata af){
	Tdata Sigma = NULL;
	StateNode *auxEstado = af.states; 
	
	while (auxEstado != NULL) {
		Transition *auxTrans = auxEstado->transitions;
		while (auxTrans != NULL) {
			Tdata simbolo = create_str_value(auxTrans->symbol);
			
			insert_set(&Sigma, simbolo);
			auxTrans = auxTrans->next;
		}
		auxEstado = auxEstado->next; 
	}
	return Sigma;
}
Tdata Rec_q0(Automata af){
	return create_str_value(af.q0);
}
Tdata Rec_Q(Automata af) {
	Tdata Q = NULL;
	StateNode *aux = af.states;
	
	while (aux != NULL) {
		Tdata nombre = create_str_value(aux->name);
		
		insert_set(&Q, nombre); // insert_set ya clono el contenido internamente
		aux = aux->next;
	}
	return Q;
}
Tdata Rec_F(Automata af){
	Tdata F = NULL;
	StateNode *aux = af.states;
	
	while (aux != NULL) {
		if (aux->isFinal) {
			Tdata nombre = create_str_value(aux->name);
			insert_set(&F, nombre);
		}
		aux = aux->next;
	}
	return F;
}

// ----- FUNCIONES DE MUESTRA -----
void printDestinations(Tdata destinos){
	if(destinos == NULL){
		printf("{}");
		return;
	}
	printData(destinos); 
}
void printTransition(Automata af, Transition *t){
	// AFD: d(q,a) = p 
	if(af.deterministic){
		if(t->to == NULL){
			printf("{}");
		}else{
			printData(data_element(data_first(t->to)));
		}
	}
	// AFND: d(q,a) = {p1,p2,...}
	else{
		printDestinations(t->to);
	}
}
void printAutomata(Automata af){
	StateNode *aux;
	
	printf("=================================\n");
	if(af.deterministic)
		printf("AFD\ndeterministic = %d\n", af.deterministic);
	else
		printf("AFND\ndeterministic = %d\n", af.deterministic);
	
	printf("Estado inicial: ");
	print_string(af.q0);
	printf("\n");
	
	printf("Estados:\n");
	aux = af.states;
	while(aux != NULL){
		printf(" - "); print_string(aux->name);
		if(aux->isFinal) printf(" (estado de aceptacion)");
		printf("\n");
		
		Transition *t = aux->transitions;
		while(t != NULL){
			printf("      ");
			print_string(t->symbol);
			printf(" -> ");
			printDestinations(t->to);
			printf("\n");
			t = t->next;
		}
		aux = aux->next;
	}
	printf("=================================\n");
}

void printAutomataFormal(Automata af){
	printf("=================================\n");
	
	if(af.deterministic)
		printf("AFD\n");
	else
		printf("AFND\n");
	
	printf("\nA = (Q, Sigma, delta, q0, F)\n\n");
	
	printf("Q = ");
	printData(Rec_Q(af));
	printf("\n");
	
	printf("Sigma = ");
	printData(Rec_sigma(af));
	printf("\n");
	
	printf("q0 = ");
	printData(Rec_q0(af));
	printf("\n");
	
	printf("F = ");
	printData(Rec_F(af));
	printf("\n\n");
	
	printf("delta:\n");
	
	StateNode *estado = af.states;
	
	while(estado != NULL){
		Transition *t = estado->transitions;
		while(t != NULL){
			printf("d(");
			print_string(estado->name);
			printf(",");
			print_string(t->symbol);
			printf(") = ");
			printTransition(af, t);
			printf("\n");
			t = t->next;
		}
		estado = estado->next;
	}
	printf("=================================\n");
}
	
// ----- FUNCION DE CONVERSION AFND A AFD -----
StateNode *agregarEstado(StateNode **estados, str nombre_estado){
	StateNode *nuevo = (StateNode*)malloc(sizeof(StateNode));
	
	nuevo->name = load2(nombre_estado);
	nuevo->transitions = NULL;
	nuevo->isFinal = 0;
	nuevo->next = NULL;
	
	if(*estados == NULL){
		*estados = nuevo;
	}else{
		StateNode *aux = *estados;
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = nuevo;
	}
	return nuevo;
}
StateNode *findSubsetState(StateNode *states, Tdata subconjunto){
	Tdata coma = create_str_value(",");
	
	while(states != NULL){
		Tdata txt = create_str_value(states->name);
		Tdata lista = split(txt, coma);
		Tdata conjunto = listToSet(lista);
		if(equals_set(conjunto, subconjunto))
			return states;
		states = states->next;
	}
	return NULL;
}
str obtenerNombreNuevo(StateNode *states, str viejo){
	StateNode *aux = states;
	int i = 0;
	
	while(aux != NULL){
		if(compararStr(aux->name, viejo) == 0){
			char buffer[20];
			sprintf(buffer, "p%d", i);
			return load2(buffer);
		}
		aux = aux->next;
		i++;
	}
	return NULL;
}
Automata renombrarEstados(Automata AFD){
	Automata nuevo = createAF();
	nuevo.deterministic = AFD.deterministic;
	
	// PRIMER RECORRIDO: Crear p0, p1, p2, ... y copiar finales
	StateNode *viejo = AFD.states;
	int i = 0;
	
	while(viejo != NULL){
		char buffer[20];
		sprintf(buffer, "p%d", i);
		StateNode *nuevoEstado = agregarEstado(&(nuevo.states), load2(buffer));
		nuevoEstado->isFinal = viejo->isFinal;
		viejo = viejo->next;
		i++;
	}
	
	// Estado inicial 
	nuevo.q0 = obtenerNombreNuevo(AFD.states, AFD.q0);
	
	// SEGUNDO RECORRIDO: Copiar transiciones 
	viejo = AFD.states;
	
	while(viejo != NULL){
		// Obtener el estado origen renombrado 
		str origenNuevoNombre = obtenerNombreNuevo(AFD.states, viejo->name);
		StateNode *origenNuevo = findState(nuevo.states, origenNuevoNombre);
		Transition *t = viejo->transitions;
		
		while(t != NULL){
			// Buscar cu�l es el estado destino viejo usando el conjunto almacenado en t->to 
			StateNode *destinoViejo = findSubsetState(AFD.states, t->to);
			
			if(destinoViejo != NULL){
				// Obtener su nuevo nombre 
				str destinoNuevoNombre = obtenerNombreNuevo(AFD.states, destinoViejo->name);
				
				// Crear {pX} 
				Tdata destino = NULL;
				Tdata dato = create_str_value(destinoNuevoNombre);
				insert_set(&destino, dato);
				Transition *nueva = createTransition(t->symbol, destino);
				addTransition(&(origenNuevo->transitions), nueva);
			}
			t = t->next;
		}
		viejo = viejo->next;
	}
	return nuevo;
}
int contieneFinal(Tdata subconjunto, Automata AFND){
	StateNode *q = AFND.states;
	
	while(q != NULL){
		if(q->isFinal){
			
			Tdata nombre = create_str_value(q->name);
			
			if(belongs(subconjunto,nombre))
				return 1;
		}
		q = q->next;
	}
	return 0;
}
Automata conversionAFD(Automata AFND){
	// 1.verifica que el automata ingresado sea NO determinista
	if(AFND.deterministic){
		printf("ERROR: el automata ya es determinista\n");
		return AFND;
	}
	
	// 2.crea automata vacio y lo define como determinista
	Automata AFD = createAF();
	AFD.deterministic = 1;
	
	// estado inicial del afd = estado inicial del afnd
	AFD.q0 = load2(AFND.q0);
	
	// recupero el alfabeto original
	Tdata alfabeto = Rec_sigma(AFND);
	
	// lista de estado pendientes: empieza con q0
	StateNode *estadoActual = agregarEstado(&(AFD.states), AFD.q0);
	
	while(estadoActual != NULL){ // mientras haya estados pendientes, tomo un estado del afd
		
		// convertir nombre del estado pendiente actual a subconjunto
		Tdata txt = create_str_value(estadoActual->name);
		
		Tdata coma = create_str_value(",");
		
		Tdata listaEstados = split(txt, coma);
		Tdata conjuntoEstados = listToSet(listaEstados);
		
		// recorremos cada simbolo del alfabeto
		Tdata letra = data_first(alfabeto);
		
		while(letra != NULL){
			Symbol simbolo = get_str_value(data_element(letra));
			Tdata destinosUnidos = NULL;
			
			// recorrer subconjunto
			Tdata auxEstado = data_first(conjuntoEstados);
			
			while(auxEstado != NULL){
				str nombreQ = get_str_value(data_element(auxEstado));
				StateNode *q = findState(AFND.states, nombreQ);
				
				if(q != NULL){
					Transition *tr = q->transitions;
					while(tr != NULL){
						if(compararStr(tr->symbol, simbolo) == 0){
							destinosUnidos = union_set(destinosUnidos, tr->to);
						}
						tr = tr->next;
					}
				}
				auxEstado = data_next(auxEstado);
			}
			
			if(destinosUnidos != NULL){
				StateNode *existente = findSubsetState(AFD.states, destinosUnidos);
				if(existente == NULL){ // si aun no existe como estado del AFD, lo agrego
					Tdata nombre = setToStr(destinosUnidos);
					existente = agregarEstado(&(AFD.states), get_str_value(nombre));
					existente->isFinal = contieneFinal(destinosUnidos, AFND);
				}
				// si ya existe como estado del AFD, solo agrego la transicion correspondiente
				Transition *t = createTransition(simbolo, clone(destinosUnidos));
				addTransition(&(estadoActual->transitions), t);
			}
			
			letra = data_next(letra);
		}
		estadoActual = estadoActual->next;
	}
	printAutomata(AFD);
	return renombrarEstados(AFD);
}
// ----- FUNCION AUXILIAR DE VALIDACION -----
// Dado un conjunto de estados actuales y un s�mbolo, devuelve el conjunto de todos los estados destinos posibles
Tdata transicionConjunto(Automata af, Tdata estadosActuales, str simbolo) {
	Tdata proximosEstados = NULL; // Conjunto vac�o {}
	
	Tdata auxEstado = data_first(estadosActuales);
	while (auxEstado != NULL) {
		// Buscamos el nodo del estado en la estructura del aut�mata
		StateNode *encontrado = findState(af.states, get_str_value(data_element(auxEstado)));
		if (encontrado != NULL) {
			Transition *t = encontrado->transitions;
			// Recorremos las transiciones del estado buscando el s�mbolo
			while (t != NULL) {
				if (compararStr(t->symbol, simbolo) == 0) {
					// t->to contiene un conjunto de estados destinos (Tdata tipo SET)
					Tdata auxDestino = data_first(t->to);
					while (auxDestino != NULL) {
						// Insertamos en el nuevo conjunto (insert_set evita duplicados)
						insert_set(&proximosEstados, data_element(auxDestino));
						auxDestino = data_next(auxDestino);
					}
				}
				t = t->next;
			}
		}
		auxEstado = data_next(auxEstado);
	}
	return proximosEstados;
}

// ----- FUNCION PRINCIPAL DE VALIDACION -----
int validarCadena(Automata af, str cadena) {
	// 1. Inicializar el conjunto de estados actuales con el estado inicial q0
	Tdata estadosActuales = NULL;
	Tdata aux;
	Tdata q0_data = create_str_value(af.q0);
	insert_set(&estadosActuales, q0_data);
	
	// 2. Procesar la cadena car�cter por car�cter
	int i = 0;
	while (cadena[i] != '\0') {
		// Convertimos el car�cter actual en un str (string de de longitud 1)
		char charStr[2];
		charStr[0] = cadena[i];
		charStr[1] = '\0';
		
		// Obtenemos el siguiente conjunto de estados
		Tdata proximos = transicionConjunto(af, estadosActuales, charStr);
		
		// Liberar el conjunto anterior 
		// por simplicidad aqu� pisamos la referencia de control)
		estadosActuales = proximos;
		
		// Si nos quedamos sin estados posibles a mitad de camino, la cadena se rechaza
		if (estadosActuales == NULL) {
			return 0; 
		}
		i++;
	}
	aux=data_first(estadosActuales);
	// 3. Verificar si alguno de los estados finales alcanzados es de aceptaci�n (isFinal)
	while (aux != NULL) {
		StateNode *est = findState(af.states, get_str_value(data_element(aux)));
		if (est != NULL && est->isFinal) {
			return 1; // Cadena aceptada
		}
		aux = data_next(aux);
	}
	
	return 0; // Ning�n estado alcanzado era final
}
