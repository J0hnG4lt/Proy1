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


struct pares{

	lista *x;
	lista *y;

};

typedef struct pares par;



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
	if (mi_lista->nombre_y == NULL){
		printf("%s ->",mi_lista->nombre_x);
	}
	else{
		printf("%s %s ->",mi_lista->nombre_x,mi_lista->nombre_y);
	}
	
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
	
	if (mi_lista->nombre_x != NULL){
	
	free(mi_lista->nombre_x);
	}
	
	if (mi_lista->nombre_y != NULL){
	
	free(mi_lista->nombre_y);
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


facelook *map(lista *mi_lista){

	facelook *base_datos_nueva = (facelook *)malloc(sizeof(facelook));
	base_datos_nueva->primero = NULL;
	base_datos_nueva->ultimo = NULL;
	
	
	nodo *nodo_act;
	
	lista *nueva_lista;
	
	nodo_act = mi_lista->primero;
	nodo *nodo_act_amigo;
	char *nombre_nodo_act_amigo;
	
	while(nodo_act != NULL){
	
		nueva_lista = (lista *)malloc(sizeof(lista));
		nueva_lista->primero = NULL;
		nueva_lista->ultimo = NULL;
		
		nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
		strcpy(nueva_lista->nombre_x,mi_lista->nombre_x);
		
		nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
		strcpy(nueva_lista->nombre_y,nodo_act->nombre);
		
		
		nodo_act_amigo = mi_lista->primero;
		
		
		
		while(nodo_act_amigo != NULL){
		
			nombre_nodo_act_amigo = (char *)malloc(sizeof(char)*100);
			strcpy(nombre_nodo_act_amigo,nodo_act_amigo->nombre);
			
			insertar_amigo(nueva_lista, nombre_nodo_act_amigo);
			
			nodo_act_amigo = nodo_act_amigo->siguiente;
			
		}
		
		
		insertar_perfil(base_datos_nueva, nueva_lista);
		
		nodo_act = nodo_act->siguiente;
	}
	
	return base_datos_nueva;

}

int esta_en_lista(nodo *mi_nodo, lista *mi_lista){

	
	if (strcmp(mi_nodo->nombre, mi_lista->nombre_x) == 0){
		return 1;
	}
	
	if (strcmp(mi_nodo->nombre, mi_lista->nombre_y) == 0){
		return 1;
	}
	
	nodo *nodo_act = mi_lista->primero;
	
	while(nodo_act != NULL){
	
		if (strcmp(mi_nodo->nombre, nodo_act->nombre) == 0){
			return 1;
		}
		
		nodo_act = nodo_act->siguiente;
	}
	
	return 0;

}

int lista_len(lista *mi_lista){

	int len = 0;
	nodo *nodo_act = mi_lista->primero;
	
	while(nodo_act != NULL){
	
		len++;
		nodo_act = nodo_act->siguiente;
	
	}
	
	return len;

}


lista *reduce(lista *lista_a, lista *lista_b){

	lista *nueva_lista = (lista *)malloc(sizeof(lista));
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	
	nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
	nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
	
	strcpy(nueva_lista->nombre_x,lista_a->nombre_x);
	strcpy(nueva_lista->nombre_y,lista_a->nombre_y);
	
	//lista *lista_corta;
	nodo *nodo_act;
	
	char *nombre_act;
	
	if (lista_len(lista_a) <= lista_len(lista_b)){
	
		//lista_corta = lista_a;
		nodo_act = lista_b->primero;
	}
	else{
		//lista_corta = lista_b;
		nodo_act = lista_a->primero;
	}
	
	while(nodo_act != NULL){
	
		if (!esta_en_lista(nodo_act, nueva_lista)){
		
			nombre_act = (char *)malloc(sizeof(char)*100);
			strcpy(nombre_act, nodo_act->nombre);
			
			insertar_amigo(nueva_lista, nombre_act);
		
		}
		
		nodo_act = nodo_act->siguiente;
	
	}

	return nueva_lista;

}

lista *fusionar_lista(lista *mi_lista_a, lista *mi_lista_b){
/*
Se asume que las listas de entrada tienen los mismos nombre_x y nombre_y
*/

	lista *nueva_lista = (lista *)malloc(sizeof(lista));
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	
	nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
	strcpy(nueva_lista->nombre_x,mi_lista_a->nombre_x);
	nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
	strcpy(nueva_lista->nombre_y,mi_lista_b->nombre_x);
	
	nodo *nodo_act_a = mi_lista_a->primero;
	nodo *nodo_act_b = mi_lista_b->primero;
	
	char *nombre_amigo_nuevo;
	
	while(nodo_act_a != NULL){
	
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);
		strcpy(nombre_amigo_nuevo,nodo_act_a->nombre);
		insertar_amigo(nueva_lista,nombre_amigo_nuevo);
		
		nodo_act_a = nodo_act_a->siguiente;
	}
	
	while(nodo_act_b != NULL){
	
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);
		strcpy(nombre_amigo_nuevo,nodo_act_b->nombre);
		insertar_amigo(nueva_lista,nombre_amigo_nuevo);
		
		nodo_act_b = nodo_act_b->siguiente;
	}
	
	return nueva_lista;

}

lista *fusionar_facelook(facelook *base_datos_a, facelook *base_datos_b){
/*
Se asume que los facelooks de entrada fueron creados por map
*/


	//facelook *nueva_base = (facelook *)malloc(sizeof(facelook));
	//nueva_base->primero = NULL;
	//nueva_base->ultimo = NULL;
	
	nodo_perfil *nodo_act_a = base_datos_a->primero;
	nodo_perfil *nodo_act_b = base_datos_b->primero;
	
	lista *lista_nueva_act;
	
	while(nodo_act_a != NULL){
	
		while(nodo_act_b != NULL){
			/*
			if (((strcmp(nodo_act_a->perfil->nombre_x,nodo_act_b->perfil->nombre_x) == 0) 
				&& (strcmp(nodo_act_a->perfil->nombre_y,nodo_act_b->perfil->nombre_y) == 0)) ||
			((strcmp(nodo_act_a->perfil->nombre_x,nodo_act_b->perfil->nombre_y) == 0)
				&&(strcmp(nodo_act_a->perfil->nombre_y,nodo_act_b->perfil->nombre_x) == 0))){
			*/
			
				//printf(" %s %s - %s %s \n",nodo_act_a->perfil->nombre_x,nodo_act_a->perfil->nombre_y,
				//						   nodo_act_b->perfil->nombre_x,nodo_act_b->perfil->nombre_y);
			
			if (strcmp(nodo_act_a->perfil->nombre_y,nodo_act_b->perfil->nombre_y) == 0){
				
				lista_nueva_act = fusionar_lista(nodo_act_a->perfil,nodo_act_b->perfil);
				//imprimir_lista(lista_nueva_act);
				//insertar_perfil(nueva_base,lista_nueva_act);
				
				return lista_nueva_act;
			}
			
			nodo_act_b = nodo_act_b->siguiente;
		}
	
	nodo_act_a = nodo_act_a->siguiente;
	}
	
	lista_nueva_act = (lista *)malloc(sizeof(lista));
	lista_nueva_act->nombre_x = (char *)malloc(sizeof(char)*100);
	lista_nueva_act->nombre_y = (char *)malloc(sizeof(char)*100);
	strcpy(lista_nueva_act->nombre_x,base_datos_a->primero->perfil->nombre_x);
	strcpy(lista_nueva_act->nombre_y,base_datos_b->primero->perfil->nombre_x);
	lista_nueva_act->primero = NULL;
	lista_nueva_act->ultimo = NULL;
	
	char *nombre_act = (char *)malloc(sizeof(char)*100);
	strcpy(nombre_act,"-None-");
	insertar_amigo(lista_nueva_act,nombre_act);
	
	return lista_nueva_act;

}


int facelook_len(facelook *base_datos){

	int len = 0;
	nodo_perfil *nodo_act = base_datos->primero;
	
	while(nodo_act != NULL){
	
		len++;
		nodo_act = nodo_act->siguiente;
	
	}
	
	return len;

}

int esta_n_veces_en_lista(nodo *mi_nodo, lista *mi_lista){

	int veces = 0;
	nodo *nodo_act = mi_lista->primero;
	
	while(nodo_act != NULL){
	
		if (strcmp(nodo_act->nombre,mi_nodo->nombre) == 0){
		
			veces++;
		
		}
	
		nodo_act = nodo_act->siguiente;
	}
	
	return veces;
}


lista *reduce_2(lista *mi_lista){


	lista *nueva_lista = (lista *)malloc(sizeof(lista));
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	
	nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
	nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
	
	strcpy(nueva_lista->nombre_x,mi_lista->nombre_x);
	strcpy(nueva_lista->nombre_y,mi_lista->nombre_y);
	
	
	nodo *nodo_act = mi_lista->primero;
	
	char *nombre_act;
	
	while(nodo_act != NULL){
	
		if ((!esta_en_lista(nodo_act, nueva_lista)) && 
			(esta_n_veces_en_lista(nodo_act,mi_lista) > 1)){
		
			nombre_act = (char *)malloc(sizeof(char)*100);
			strcpy(nombre_act, nodo_act->nombre);
			insertar_amigo(nueva_lista,nombre_act);
		
		}
		
		nodo_act = nodo_act->siguiente;
	
	}
	
	if (nueva_lista->primero == NULL){
		
		nombre_act = (char *)malloc(sizeof(char)*100);
		strcpy(nombre_act,"-None-");
		insertar_amigo(nueva_lista, nombre_act);
	}
	
	return nueva_lista;
	
	

}











