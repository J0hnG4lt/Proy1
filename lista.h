#include <stdlib.h>
#include <stdio.h>

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
