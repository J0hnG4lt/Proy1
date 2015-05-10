#include <stdlib.h>
#include <stdio.h>
#include <string.h>



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



void insertar_amigo(lista *mi_lista ,char *nombre_amigo){

	if (mi_lista->primero != NULL){
	
		mi_lista->ultimo->siguiente = (nodo *)malloc(sizeof(nodo));
		mi_lista->ultimo->siguiente->siguiente = NULL;
		mi_lista->ultimo->siguiente->nombre = nombre_amigo;
		mi_lista->ultimo = mi_lista->ultimo->siguiente;
		
	}
	else{
		
		if(mi_lista->nombre_x==NULL){
			mi_lista->nombre_x = nombre_amigo;
		}
		else{
			mi_lista->primero = (nodo *)malloc(sizeof(nodo));
			mi_lista->primero->nombre = nombre_amigo;
			mi_lista->primero->siguiente = NULL;
			mi_lista->ultimo = mi_lista->primero;
			mi_lista->ultimo->siguiente = NULL;
		}
	}
}

void insertar_perfil(facelook *base_datos, lista *nuevo_usuario){

	if (base_datos->primero != NULL){
	
		base_datos->ultimo->siguiente = (nodo_perfil *)malloc(sizeof(nodo_perfil));
		base_datos->ultimo->siguiente->perfil = nuevo_usuario;
		base_datos->ultimo->siguiente->siguiente = NULL;
		base_datos->ultimo = base_datos->ultimo->siguiente;
		base_datos->ultimo->siguiente = NULL;

	}
	else{
	
		base_datos->primero = (nodo_perfil *)malloc(sizeof(nodo_perfil));
		base_datos->primero->perfil = nuevo_usuario;
		base_datos->primero->siguiente = NULL;
		base_datos->ultimo = base_datos->primero;
		base_datos->ultimo->siguiente = NULL;
	
	}

}





void imprimir_lista(lista *mi_lista){

	nodo *nodo_act = mi_lista->primero;
	printf("%s ->",mi_lista->nombre_x);
	
	while(nodo_act!=NULL){
	
		printf(" %s", nodo_act->nombre);
		nodo_act = nodo_act->siguiente;
		
	}
	
	printf("\n");

}

void eliminar_lista(lista *mi_lista){

	nodo *nodo_act;
	nodo *nodo_sig = mi_lista->primero;
	
	while(nodo_sig != NULL){
		
		nodo_act = nodo_sig;
		nodo_sig = nodo_sig->siguiente;
		
		free(nodo_act->nombre);
		free(nodo_act);
	
	}

	free(mi_lista);

}

void eliminar_facelook(facelook *base_datos){

	nodo_perfil *nodo_act;
	nodo_perfil *nodo_sig = base_datos->primero;
	
	while(nodo_sig != NULL){
	
		nodo_act = nodo_sig;
		nodo_sig = nodo_sig->siguiente;
		
		eliminar_lista(nodo_act->perfil);
		free(nodo_act);
		
	
	}
	
	free(base_datos);

}




void leer_archivo(facelook *base_datos, char *nombre_archivo){


	FILE *archivo_n = fopen(nombre_archivo, "r");

	if (archivo_n == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}
	int lines=0;
	char carac;
	while((carac = fgetc(archivo_n))!=EOF){
	
		if (carac == '\n'){lines++;}
	
	}
	
	fclose(archivo_n);
	
	
	FILE *archivo = fopen(nombre_archivo, "r");
	
	if (archivo == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}

	
	base_datos->primero = NULL;
	base_datos->ultimo = NULL;
	
	lista *usuario_nuevo;
	char *nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
	char *nombre_amigo_nuevo;
	char *linea = (char *)malloc(sizeof(char)*100);
	char *input_tok;

	int num_lineas_act=0;


	while(fscanf(archivo,"%[^\n]%*c",linea) != EOF){

		
		if(lines == num_lineas_act){break;}
		num_lineas_act++;

		input_tok = strtok(linea, " ");
		strcpy(nombre_usuario_nuevo,input_tok);
		
		strtok(NULL," ");
		
		usuario_nuevo = (lista *)malloc(sizeof(lista));
		usuario_nuevo->primero = NULL;
		usuario_nuevo->ultimo = NULL;
		usuario_nuevo->nombre_x = NULL;
		usuario_nuevo->nombre_y = NULL;
		
		insertar_amigo(usuario_nuevo,nombre_usuario_nuevo);
		
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);

		while((input_tok = strtok(NULL, " ")) != NULL){
		
			strcpy(nombre_amigo_nuevo,input_tok);
			insertar_amigo(usuario_nuevo,nombre_amigo_nuevo);
			nombre_amigo_nuevo =  (char *)malloc(sizeof(char)*100);
			
		}
		
		free(nombre_amigo_nuevo);
		nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
		insertar_perfil(base_datos,usuario_nuevo);
		
		
	}
	
	free(nombre_usuario_nuevo);
	fclose(archivo);

}

void imprimir_facelook(facelook *base_datos){

	nodo_perfil *nodo_act = base_datos->primero;
	
	while(nodo_act != NULL){
	
		imprimir_lista(nodo_act->perfil);
		nodo_act = nodo_act->siguiente;
	}

}





