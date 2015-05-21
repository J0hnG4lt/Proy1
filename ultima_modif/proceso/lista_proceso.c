#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*

Autor: Georvic Tur 
Carnet: 12-11402

*/


//ESTRUCTURAS DE DATOS

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



//FUNCIONES


void insertar_amigo(lista *mi_lista ,char *nombre_amigo){

/*

Inserta un amigo de nombre "nombre_amigo" en la lista de usuario "mi_lista"

*/

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

/*
Inserta la lista de amigos "nuevo_usuario" en la lista de usuarios "base_datos"
*/


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

/*
Imprime la lista "mi_lista".

Ejemplo: usuario -> amigo1 amigo2 ...
         usuario1 usuario2 -> amigo1 amigo2 ...
*/

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

/*
Libera de memoria la lista "mi_lista"
*/

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

/*
Libera de memoria la lista de usuarios "base_datos"
*/

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

/*
Lee el archivo de nombre "nombre_archivo" que contiene listas de la forma
                usuario -> amigo1 amigo2 ...
 y las guarda en la lista de usuarios "base_datos".
*/

	FILE *archivo_n = fopen(nombre_archivo, "r");

	if (archivo_n == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}
	
	//Calculo el numero de lineas del archivo
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

	//Inicializo
	base_datos->primero = NULL;
	base_datos->ultimo = NULL;
	
	lista *usuario_nuevo;
	char *nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
	char *nombre_amigo_nuevo;
	char *linea = (char *)malloc(sizeof(char)*100);
	char *input_tok;

	int num_lineas_act=0;


	//Leo linea por linea
	while(fscanf(archivo,"%[^\n]%*c",linea) != EOF){

		
		if(lines == num_lineas_act){break;}
		num_lineas_act++;

		//Divido en tokens la linea
		input_tok = strtok(linea, " ");
		strcpy(nombre_usuario_nuevo,input_tok);
		
		strtok(NULL," ");
		
		//Creo una nueva lista de amigos por cada linea
		usuario_nuevo = (lista *)malloc(sizeof(lista));
		usuario_nuevo->primero = NULL;
		usuario_nuevo->ultimo = NULL;
		usuario_nuevo->nombre_x = NULL;
		usuario_nuevo->nombre_y = NULL;
		
		insertar_amigo(usuario_nuevo,nombre_usuario_nuevo);
		
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);

		while((input_tok = strtok(NULL, " ")) != NULL){
		
			//Creo un nodo amigo por cada token
			strcpy(nombre_amigo_nuevo,input_tok);
			insertar_amigo(usuario_nuevo,nombre_amigo_nuevo);
			nombre_amigo_nuevo =  (char *)malloc(sizeof(char)*100);
			
		}
		
		free(nombre_amigo_nuevo);
		nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
		insertar_perfil(base_datos,usuario_nuevo);
		
		
	}
	free(linea);
	free(nombre_usuario_nuevo);
	fclose(archivo);

}



void imprimir_facelook(facelook *base_datos){

/*
Imprime en terminal la lista de usuarios "base_datos"
*/

	nodo_perfil *nodo_act = base_datos->primero;
	
	while(nodo_act != NULL){
	
		imprimir_lista(nodo_act->perfil);
		nodo_act = nodo_act->siguiente;
	}

}


facelook *map(lista *mi_lista){

/*
Crea una lista por cada par de usuario y amigo_i de "mi_lista" y la guarda en
una nueva lista de usuarios que se retorna
*/

	//Inicializo
	facelook *base_datos_nueva = (facelook *)malloc(sizeof(facelook));
	base_datos_nueva->primero = NULL;
	base_datos_nueva->ultimo = NULL;
	
	
	nodo *nodo_act;
	
	lista *nueva_lista;
	
	nodo_act = mi_lista->primero;
	nodo *nodo_act_amigo;
	char *nombre_nodo_act_amigo;
	
	//Recorro la mi_lista
	while(nodo_act != NULL){
	
		//Creo una nueva lista de amigos por cada amigo de mi_lista
		nueva_lista = (lista *)malloc(sizeof(lista));
		nueva_lista->primero = NULL;
		nueva_lista->ultimo = NULL;
		
		//Copio sus datos
		nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
		strcpy(nueva_lista->nombre_x,mi_lista->nombre_x);
		
		//Lo unico que distingue a cada lista de amigos es nombre_y
		nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
		strcpy(nueva_lista->nombre_y,nodo_act->nombre);
		
		
		nodo_act_amigo = mi_lista->primero;
		
		
		//Inserto los amigos
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

/*
Retorna 1 si "mi_nodo" esta en "mi_lista". De lo contrario, retorna 0
*/

	
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

/*
Retorna el numero de elementos de la lista
*/

	int len = 0;
	nodo *nodo_act = mi_lista->primero;
	
	while(nodo_act != NULL){
	
		len++;
		nodo_act = nodo_act->siguiente;
	
	}
	
	return len;

}



lista *fusionar_lista(lista *mi_lista_a, lista *mi_lista_b){
/*
Concatena las listas dadas como argumentos.
Se asume que las listas de entrada tienen los mismos nombre_x y nombre_y
*/

	//Inicializo
	lista *nueva_lista = (lista *)malloc(sizeof(lista));
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	
	//Copio datos de cabeza
	nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
	strcpy(nueva_lista->nombre_x,mi_lista_a->nombre_x);
	nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
	strcpy(nueva_lista->nombre_y,mi_lista_b->nombre_x);
	
	nodo *nodo_act_a = mi_lista_a->primero;
	nodo *nodo_act_b = mi_lista_b->primero;
	
	char *nombre_amigo_nuevo;
	
	//Recorro la primera lista
	while(nodo_act_a != NULL){
	
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);
		strcpy(nombre_amigo_nuevo,nodo_act_a->nombre);
		insertar_amigo(nueva_lista,nombre_amigo_nuevo);
		
		nodo_act_a = nodo_act_a->siguiente;
	}
	
	//Recorro la segunda lista
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
Se selecciona un par de listas de usuarios de "base_datos_a" y "base_datos_b"
y se concatenan. Este par debe tener elementos en comun en la lista de amigos.
De lo contrario, se devuelve una lista de la forma 
                    usuario1 usuario2 -> -None-
Se asume que los facelooks de entrada fueron creados por map
*/


	//Inicializo
	nodo_perfil *nodo_act_a = base_datos_a->primero;
	nodo_perfil *nodo_act_b = base_datos_b->primero;
	
	
	lista *lista_nueva_act;
	
	//Recorro la primera lista de usuarios
	while(nodo_act_a != NULL){
	

		nodo_act_b = base_datos_b->primero;
		
		//Recorro la segunda lista de usuarios
		while(nodo_act_b != NULL){
			
			
			//Se fusionan las listas si se ha encontrado un par de usuarios
			if (strcmp(nodo_act_a->perfil->nombre_y,nodo_act_b->perfil->nombre_y) == 0){
				
				lista_nueva_act = fusionar_lista(nodo_act_a->perfil,nodo_act_b->perfil);
				
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
	
	//Si no se encontro ningun par de usuarios
	char *nombre_act = (char *)malloc(sizeof(char)*100);
	strcpy(nombre_act,"-None-");
	insertar_amigo(lista_nueva_act,nombre_act);
	
	return lista_nueva_act;

}


int facelook_len(facelook *base_datos){

/*
Retorna el numero de elementos de la lista de usuarios base_datos
*/

	int len = 0;
	nodo_perfil *nodo_act = base_datos->primero;
	
	while(nodo_act != NULL){
	
		len++;
		nodo_act = nodo_act->siguiente;
	
	}
	
	return len;

}

int esta_n_veces_en_lista(nodo *mi_nodo, lista *mi_lista){

/*
Retorna el nuemro de veces que "mi_nodo" aparece en "mi_lista"
*/

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

/*
Quita las repeticiones de los nombres que aparecen en "mi_lista"
*/

	//Inicializo
	lista *nueva_lista = (lista *)malloc(sizeof(lista));
	nueva_lista->primero = NULL;
	nueva_lista->ultimo = NULL;
	
	nueva_lista->nombre_x = (char *)malloc(sizeof(char)*100);
	nueva_lista->nombre_y = (char *)malloc(sizeof(char)*100);
	
	strcpy(nueva_lista->nombre_x,mi_lista->nombre_x);
	strcpy(nueva_lista->nombre_y,mi_lista->nombre_y);
	
	
	nodo *nodo_act = mi_lista->primero;
	
	char *nombre_act;
	
	//Recorro la lista
	while(nodo_act != NULL){
	
		//Solo se insertan los amigos que no hayan sido insertados
		//Solo se insertan los amigos que aparezcan repetidos en mi_lista
		if ((!esta_en_lista(nodo_act, nueva_lista)) && 
			(esta_n_veces_en_lista(nodo_act,mi_lista) > 1)){
		
			nombre_act = (char *)malloc(sizeof(char)*100);
			strcpy(nombre_act, nodo_act->nombre);
			insertar_amigo(nueva_lista,nombre_act);
		
		}
		
		nodo_act = nodo_act->siguiente;
	
	}
	
	//Si no habia ningun amigo en comun
	if (nueva_lista->primero == NULL){
		
		nombre_act = (char *)malloc(sizeof(char)*100);
		strcpy(nombre_act,"-None-");
		insertar_amigo(nueva_lista, nombre_act);
	}
	
	
	return nueva_lista;
	
	

}



void facelook_to_string(facelook *base_datos, char *nombre_arch){

/*
Guarda en un archivo de nombre "nombre_arch" las listas contenidas en "base_datos"
*/

	//int num_lineas = facelook_len(base_datos);
	
	
	FILE *archivo = fopen(nombre_arch, "a");
	
	nodo_perfil *nodo_act = base_datos->primero;
	nodo *nodo_act_amigo;
	
	while(nodo_act != NULL){
	
		nodo_act_amigo = nodo_act->perfil->primero;
		
		fprintf(archivo, "%s %s ->", nodo_act->perfil->nombre_x,nodo_act->perfil->nombre_y);
		
		while(nodo_act_amigo != NULL){
		
			fprintf(archivo, " %s", nodo_act_amigo->nombre);
			nodo_act_amigo = nodo_act_amigo->siguiente;
		}
		
		fprintf(archivo, "\n");
		
		nodo_act = nodo_act->siguiente;
	
	
	}
	
	fclose(archivo);

}


void leer_archivo_pid(facelook *facelook_array[], int *pos, char *nombre_archivo){

/*
Lee el archivo de nombre "nombre_archivo" y crea con sus contenidos "facelooks *"
distintos que son guardados en "facelook_array" en la posicion "pos"

Usado en la comunicacion de procesos
*/

	FILE *archivo_n = fopen(nombre_archivo, "r");

	if (archivo_n == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}
	int lines=0;
	char carac;
	
	//Cuenta el numero de lineas
	while((carac = fgetc(archivo_n))!=EOF){
	
		if (carac == '\n'){lines++;}
	
	}
	
	fclose(archivo_n);
	
	
	FILE *archivo = fopen(nombre_archivo, "r");
	
	if (archivo == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}

	//Inicializa
	facelook *base_datos = (facelook *)malloc(sizeof(facelook));
	
	char *nombre_anterior = NULL;
	
	base_datos->primero = NULL;
	base_datos->ultimo = NULL;
	
	lista *usuario_nuevo;
	char *nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
	char *nombre_amigo_nuevo;
	char *linea = (char *)malloc(sizeof(char)*100);
	char *input_tok;

	char *nombre_usuario_par  = (char *)malloc(sizeof(char)*100);

	int num_lineas_act=0;

	//Lee linea por linea
	while(fscanf(archivo,"%[^\n]%*c",linea) != EOF){

		
		if(lines <= num_lineas_act){break;}
		num_lineas_act++;
		
		//Divide en tokens la linea
		
		input_tok = strtok(linea, " ");
		strcpy(nombre_usuario_nuevo,input_tok);
		
		input_tok = strtok(NULL," ");
		strcpy(nombre_usuario_par, input_tok);
		
		strtok(NULL," ");
		
		//Inicializa
		usuario_nuevo = (lista *)malloc(sizeof(lista));
		usuario_nuevo->primero = NULL;
		usuario_nuevo->ultimo = NULL;
		usuario_nuevo->nombre_x = NULL;
		usuario_nuevo->nombre_y = NULL;
		
		insertar_amigo(usuario_nuevo,nombre_usuario_nuevo);
		usuario_nuevo->nombre_y = nombre_usuario_par;
		
		
		//Separa en listas de usuarios distintos en caso de que el archivo
		// contenga mas de una
		// Las listas de usuarios distintos son las que tienen nombre_x distinto
		// luego de haberles aplicado map
		if (nombre_anterior != NULL){
		
			if (strcmp(nombre_anterior, usuario_nuevo->nombre_x) != 0){
			
				nombre_anterior = usuario_nuevo->nombre_x;
				
				facelook_array[*pos] = base_datos;
				*pos = *pos + 1;
				
				base_datos = (facelook *)malloc(sizeof(facelook));
				base_datos->primero = NULL;
				base_datos->ultimo = NULL;
			
			}
		
		}
		else{
		
			nombre_anterior = usuario_nuevo->nombre_x;
		
		}
		
		
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);

		//Inerto un amigo por cada token
		while((input_tok = strtok(NULL, " ")) != NULL){
		
			strcpy(nombre_amigo_nuevo,input_tok);
			insertar_amigo(usuario_nuevo,nombre_amigo_nuevo);
			nombre_amigo_nuevo =  (char *)malloc(sizeof(char)*100);
			
		}
		
		free(nombre_amigo_nuevo);
		nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
		nombre_usuario_par  = (char *)malloc(sizeof(char)*100);
		insertar_perfil(base_datos,usuario_nuevo);
		
		
		
	}
	
	facelook_array[*pos] = base_datos;
	*pos = *pos + 1;
	
	free(nombre_usuario_nuevo);
	fclose(archivo);

}



void leer_archivo_pid_reduce(facelook *base_datos_reducida, char *nombre_archivo){

/*
Lee el archivo "nombre_archivo" y guarda sus contenidos en "base_datos_reducida"

Usado en la comunicacion de procesos
*/

	FILE *archivo_n = fopen(nombre_archivo, "r");

	if (archivo_n == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}
	int lines=0;
	char carac;
	
	//Cuenta el numero de lineas
	while((carac = fgetc(archivo_n))!=EOF){
	
		if (carac == '\n'){lines++;}
	
	}
	
	fclose(archivo_n);
	
	
	FILE *archivo = fopen(nombre_archivo, "r");
	
	if (archivo == NULL){
	
		perror("Ocurrio un error al abrir archivo\n");
		exit(1);
	
	}

	//Inicializo
	
	lista *usuario_nuevo;
	char *nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
	char *nombre_amigo_nuevo;
	char *linea = (char *)malloc(sizeof(char)*100);
	char *input_tok;

	char *nombre_usuario_par  = (char *)malloc(sizeof(char)*100);

	int num_lineas_act=0;

	//Leo linea por lina
	while(fscanf(archivo,"%[^\n]%*c",linea) != EOF){

		
		if(lines <= num_lineas_act){break;}
		num_lineas_act++;

		//Encuentro los tokens de la linea
		input_tok = strtok(linea, " ");
		strcpy(nombre_usuario_nuevo,input_tok);
		
		input_tok = strtok(NULL," ");
		strcpy(nombre_usuario_par, input_tok);
		
		strtok(NULL," ");
		
		//Inicializo
		usuario_nuevo = (lista *)malloc(sizeof(lista));
		usuario_nuevo->primero = NULL;
		usuario_nuevo->ultimo = NULL;
		usuario_nuevo->nombre_x = NULL;
		usuario_nuevo->nombre_y = NULL;
		
		insertar_amigo(usuario_nuevo,nombre_usuario_nuevo);
		usuario_nuevo->nombre_y = nombre_usuario_par;
		
		
		
		nombre_amigo_nuevo = (char *)malloc(sizeof(char)*100);

		//Cada token es un nuevo amigo
		while((input_tok = strtok(NULL, " ")) != NULL){
		
			strcpy(nombre_amigo_nuevo,input_tok);
			insertar_amigo(usuario_nuevo,nombre_amigo_nuevo);
			nombre_amigo_nuevo =  (char *)malloc(sizeof(char)*100);
			
		}
		
		free(nombre_amigo_nuevo);
		nombre_usuario_nuevo = (char *)malloc(sizeof(char)*100);
		nombre_usuario_par  = (char *)malloc(sizeof(char)*100);
		insertar_perfil(base_datos_reducida,usuario_nuevo);
		
		
		
	}
	

	
	free(nombre_usuario_nuevo);
	fclose(archivo);

}


