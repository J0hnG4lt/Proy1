#include <stdlib.h>
#include <stdio.h>


/*

Autor: Georvic Tur 
Carnet: 12-11402

*/

struct nodos{

	char *nombre;
	struct nodos *siguiente;

};

typedef struct nodos nodo;


struct cabeza{

	nodo *primero;
	nodo *ultimo;
	
	char *nombre_x;
	char *nombre_y;

};

typedef struct cabeza lista;

struct lista_de_listas{

	lista *perfil;
	struct lista_de_listas *siguiente;

};

typedef struct lista_de_listas nodo_perfil;

struct facelooks{

	nodo_perfil *primero;
	nodo_perfil *ultimo;

};

typedef struct facelooks facelook;



void insertar_amigo(lista *, char *);
void insertar_perfil(facelook *, lista *);
void eliminar_lista(lista *);
void eliminar_facelook(facelook *);
void leer_archivo(facelook *, char *);
void imprimir_lista(lista *);
void imprimir_facelook(facelook *);
facelook *map(lista *);
int esta_en_lista(nodo *, lista *);
int lista_len(lista *);
lista *reduce(lista *, lista *);
lista *fusionar_lista(lista *, lista *);
lista *fusionar_facelook(facelook *, facelook *);
int facelook_len(facelook *);
lista *reduce_2(lista *);
void facelook_to_string(facelook *, char *);
void leer_archivo_pid(facelook *facelook_array[], int *, char *);
void leer_archivo_pid_reduce(facelook *, char *);
