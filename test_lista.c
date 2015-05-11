#include <stdlib.h>
#include <stdio.h>
#include "lista.h"


int main(int argc, char **argv){
	

	facelook *base_datos = (facelook *)malloc(sizeof(facelook));
	
	if (base_datos == NULL){
	
		perror("Error al hacer malloc\n");
		exit(1);
	
	}
	
	leer_archivo(base_datos, argv[1]);
	
	printf("Iniciales\n");
	imprimir_facelook(base_datos);
	
	int cantidad_lineas_archivo = facelook_len(base_datos);
	facelook *facelook_array[cantidad_lineas_archivo];
	
	
	
	nodo_perfil *nodo_perfil_act = base_datos->primero;
	int i=0;;
	while(nodo_perfil_act != NULL){
		
		facelook_array[i] = map(nodo_perfil_act->perfil);
		
		//imprimir_facelook(facelook_array[i]);
		
		i++;
		nodo_perfil_act = nodo_perfil_act->siguiente;
		
	}
	
	
	int cantidad_pares_listas = cantidad_lineas_archivo*(cantidad_lineas_archivo-1)/2;
	lista *lista_array[cantidad_pares_listas];
	
	//nodo_perfil *nodo_act_a = facelook_array[0];
	//nodo_perfil *nodo_act_b = facelook_array[1]; //CUIDADO
	
	int pos_nodo_a = 0;
	int pos_nodo_b = 1;
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		lista_array[i] = fusionar_facelook(facelook_array[pos_nodo_a], facelook_array[pos_nodo_b]);
		
		//printf("facelook_test\n");
		//imprimir_facelook(facelook_array[pos_nodo_a]);
		
		//printf("facelook_test\n");
		//imprimir_facelook(facelook_array[pos_nodo_b]);
		
		//printf("Fusionados\n");
		//imprimir_lista(lista_array[i]);
		
		pos_nodo_b++;
		if(pos_nodo_b == cantidad_lineas_archivo){
		
			pos_nodo_b = (++pos_nodo_a) + 1;
			
			if (pos_nodo_a == cantidad_lineas_archivo-1){
				break;
			}
		}
	
	}
	
	lista *lista_array_reduced[cantidad_pares_listas];
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		lista_array_reduced[i] = reduce_2(lista_array[i]);
		
		printf("Reducidos\n");
		imprimir_lista(lista_array_reduced[i]);
	
	}
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		
		eliminar_lista(lista_array[i]);
		eliminar_lista(lista_array_reduced[i]);
	
	}
	
	for(i = 0;i<cantidad_lineas_archivo;i++){
	
		eliminar_facelook(facelook_array[i]);
	
	}
	

	
	eliminar_facelook(base_datos);
	
	return 0;

}
