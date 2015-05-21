#include <stdlib.h>
#include <stdio.h>
#include "lista_thread.h"
#include <pthread.h>


/*

Autor: Georvic Tur 
Carnet: 12-11402

*/


//Variables Globales para Threads usados en map_thread
nodo_perfil **array_usuarios = NULL;
int cant_usuarios;
int cant_threads;
facelook **facelook_array = NULL;




//Funcion que mapea con threads
void *map_thread(void *num_thread){

	int i;
	
	for(i = *(int *)num_thread; i < cant_usuarios; i += cant_threads ){
	
	//Seccion critica
		facelook_array[i] = map(array_usuarios[i]->perfil);
	}
	
	pthread_exit(NULL);

}


//Variables Globales para Threads usados en map_thread
lista **lista_array_reduced = NULL;
lista **lista_array = NULL;
int numero_pares;


//Aplica reduce con threads
void *reduce_thread(void *num_thread){

	int i;
	
	
	for(i = *(int *)num_thread; i<numero_pares; i += cant_threads){
	
		lista_array_reduced[i] = reduce_2(lista_array[i]);
	
	}
	
	pthread_exit(NULL);


}



int main(int argc, char **argv){
	
	//Parseo la entrada
	int numero_threads;
	char *nombre_arch_entrada;
	char *nombre_arch_salida;
	
	if (argc == 4){
		
		
		numero_threads = atoi(argv[1]);
		if (numero_threads == 0){
			numero_threads = 1;
		}
		nombre_arch_entrada = argv[2];
		nombre_arch_salida = argv[3];
		
	}
	else if (argc == 3){
	
		numero_threads = 1;
		nombre_arch_entrada = argv[1];
		nombre_arch_salida = argv[2];
	}
	else{
	
		printf("Numero incorrecto de argumentos\n");
		exit(1);
	}

	//Guarda la lista de usuarios con su correspondiente lista de amigos
	facelook *base_datos = (facelook *)malloc(sizeof(facelook *));
	
	if (base_datos == NULL){
	
		perror("Error al hacer malloc\n");
		exit(1);
	
	}
	
	
	//Leo archivo y guardo en base_datos
	leer_archivo(base_datos, nombre_arch_entrada);
	//printf("Iniciales\n");
	//imprimir_facelook(base_datos);
	
	int cantidad_lineas_archivo = facelook_len(base_datos);
	
	//Inicializo las vairables globales
	facelook_array = (facelook **)malloc(sizeof(facelook *)*cantidad_lineas_archivo);
	array_usuarios = (nodo_perfil **)malloc(sizeof(nodo_perfil *)*cantidad_lineas_archivo);
	
	nodo_perfil *nodo_perfil_act = base_datos->primero;
	
	
	//Guardo en arreglo
	int j;
	for(j=0;j<cantidad_lineas_archivo;j++){
	
		array_usuarios[j] = nodo_perfil_act;
		nodo_perfil_act = nodo_perfil_act->siguiente;
	}
	
	
	
	cant_usuarios = cantidad_lineas_archivo;
	cant_threads = numero_threads;
	//Fin inicializar globales
	
	pthread_t threads_array[cant_threads];
	int *id_thread_array[cant_threads];
	
	
	//Creo threads para mapear
	
	int d;
	for(d=0;d<cant_threads;d++){
	
		id_thread_array[d] = (int *)malloc(sizeof(int));
		*id_thread_array[d] = d;
		
		pthread_create(&threads_array[d], NULL, map_thread, (void *)id_thread_array[d]);
	
		pthread_join(threads_array[d], NULL);
		
		//free(id_thread_array[d]);
	}
	
	
	for(d=0;d<cant_threads;d++){
		free(id_thread_array[d]);
	}
	
	
	int cantidad_pares_listas = cantidad_lineas_archivo*(cantidad_lineas_archivo-1)/2;
	
	//Inicializo variables globales de thread_reduce
	
	lista_array_reduced = (lista **)malloc(sizeof(lista *)*cantidad_pares_listas);
	lista_array = (lista **)malloc(sizeof(lista *)*cantidad_pares_listas); 
	numero_pares = cantidad_pares_listas;
	
	
	
	int pos_nodo_a = 0;
	int pos_nodo_b = 1;
	int i;
	
	//Creo los pares
	
	for(i = 0;i<cantidad_pares_listas;i++){
		
		lista_array[i] = fusionar_facelook(facelook_array[pos_nodo_a], facelook_array[pos_nodo_b]);
		
		
		pos_nodo_b++;
		if(pos_nodo_b == cantidad_lineas_archivo){
		
			pos_nodo_b = (++pos_nodo_a) + 1;
			
			if (pos_nodo_a == cantidad_lineas_archivo-1){
				break;
			}
		}
	
	}
	
	//Ids de threads
	pthread_t thread_array_reduce[cant_threads];
	int *id_thread_array_reduce[cant_threads];
	
	
	//Creo threads para aplicar reduce
	for(d = 0; d < cant_threads; d++){
	
		id_thread_array_reduce[d] = (int *)malloc(sizeof(int));
		*id_thread_array_reduce[d] = d;
		
		pthread_create(&thread_array_reduce[d], NULL, reduce_thread, (void *)id_thread_array_reduce[d]);
	
		pthread_join(thread_array_reduce[d], NULL);
		
		//free(id_thread_array_reduce);
	}
	
	for(d=0;d<cant_threads;d++){
		free(id_thread_array_reduce[d]);
	}
	
	//Procedo a quitar la redundancia
	
	facelook *base_datos_reducida = (facelook *)malloc(sizeof(facelook));
	base_datos_reducida->primero = NULL;
	base_datos_reducida->ultimo = NULL;
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		//lista_array_reduced[i] = reduce_2(lista_array[i]);
		
		//printf("Reducidos\n");
		//imprimir_lista(lista_array_reduced[i]);
		insertar_perfil(base_datos_reducida, lista_array_reduced[i]);
	
	}
	
	
	//Limpia archivo de salida
	FILE *archivo_salida_limpiar = fopen(nombre_arch_salida, "w");
	fclose(archivo_salida_limpiar);
	
	//Guarda en archivo de salida
	facelook_to_string(base_datos_reducida, nombre_arch_salida);
	
	eliminar_facelook(base_datos_reducida);
	
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		
		eliminar_lista(lista_array[i]);
		//eliminar_lista(lista_array_reduced[i]);
	
	}
	
	free(lista_array);
	free(lista_array_reduced);
	
	for(i = 0;i<cantidad_lineas_archivo;i++){
	
		eliminar_facelook(facelook_array[i]);
	
	}
	
	free(facelook_array);
	free(array_usuarios);
	//free(base_datos_reducida);
	
	
	
	eliminar_facelook(base_datos);
	
	return 0;

}
