#include <stdlib.h>
#include <stdio.h>
#include "lista_proceso.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/*

Autor: Georvic Tur 
Carnet: 12-11402

*/

int main(int argc, char **argv){
	
	
	//Parseo la entrada
	int numero_procesos;
	char *nombre_arch_entrada;
	char *nombre_arch_salida;
	
	if (argc == 4){
		
		
		numero_procesos = atoi(argv[1]);
		if (numero_procesos == 0){
			numero_procesos = 1;
		}
		nombre_arch_entrada = argv[2];
		nombre_arch_salida = argv[3];
		
	}
	else if (argc == 3){
	
		numero_procesos = 1;
		nombre_arch_entrada = argv[1];
		nombre_arch_salida = argv[2];
	}
	else{
	
		printf("Numero incorrecto de argumentos\n");
		exit(1);
	}
	
	
	

	facelook *base_datos = (facelook *)malloc(sizeof(facelook));
	
	if (base_datos == NULL){
	
		perror("Error al hacer malloc\n");
		exit(1);
	
	}
	
	//Leo el archivo
	leer_archivo(base_datos, nombre_arch_entrada);
	//imprimir_facelook(base_datos);
	
	//Arreglo de listas
	int cantidad_lineas_archivo = facelook_len(base_datos);
	nodo_perfil *array_usuarios[cantidad_lineas_archivo];
	
	
	//Guardo cada nodo de la lista en un arreglo
	nodo_perfil *nodo_act_perf = base_datos->primero;
	int pos;
	for(pos = 0; pos < cantidad_lineas_archivo; pos++){
	
		array_usuarios[pos] = nodo_act_perf;
		nodo_act_perf = nodo_act_perf->siguiente;
	
	}
	
	
	//Creacion de los semaforos
	char *nombre_semaforo[numero_procesos];
	sem_t *semaforos[numero_procesos];
	
	for(pos = 0; pos < numero_procesos; pos++){
	
		nombre_semaforo[pos] = (char *)malloc(sizeof(char)*100);
		sprintf(nombre_semaforo[pos], "sem_map_%d", pos);
		semaforos[pos] = sem_open(nombre_semaforo[pos], O_CREAT, 0644, 0);
	}
	
	
	
	//Semaforo del padre
	//sem_t *semaforo = sem_open("sem_1", O_CREAT, 0644, 0);
	
	//Semaforo de los hijos
	sem_t *semaforo_h;
	
	//PID en int y string de cada hijo
	pid_t pid_hijos[numero_procesos];
	char *pid_string[numero_procesos];
	
	//Archivo correspondiente al proceso hijo actual
	FILE *arch_proceso_act;
	
	//PID del hijo actual en string
	char *nombre_pid = (char *)malloc(sizeof(char)*100);
	
	//Permite repartir iteraciones entre los procesos hijos de manera justa
	int num_init_proceso;
	
	facelook *temp;
	
	int t; 
	int f;
	for(f = 0; f<numero_procesos; f++){
	
		//Guardo el PID del HIJO
		pid_hijos[f] = fork();
		
		if(pid_hijos[f] < 0){
			printf("error\n");
		}
		else if(pid_hijos[f] == 0){
		
		//Código que ejecuta el hijo
		
			//Se sincroniza con el padre
			semaforo_h = sem_open(nombre_semaforo[f], O_CREAT, 0644, 0);
			
			//Espera a que el padre le asigne un numero y lo guarde en un archivo
			sem_wait(semaforo_h);
			
			//Abre el archivo con el numero asignado y lo lee
			sprintf(nombre_pid,"%d.txt", getpid());
			arch_proceso_act = fopen(nombre_pid,"r");
			fscanf(arch_proceso_act, "%d%*c", &num_init_proceso);
			fclose(arch_proceso_act);
			
			//Limpia dicho archivo
			FILE *archivo_res = fopen(nombre_pid, "w");
			fclose(archivo_res);
			
			//Guarda el resultado de map en el archivo correspondiente
			for(t = num_init_proceso; t<cantidad_lineas_archivo; t+=numero_procesos){
			
				temp = map(array_usuarios[t]->perfil);
				facelook_to_string( temp , nombre_pid );
				eliminar_facelook(temp);
			}
			
			//Deja de usar semaforo
			sem_close(semaforo_h);
			//printf("FIN %d\n",getpid());
			
			//Liberar memoria
			
			eliminar_facelook(base_datos);
			
			int f;
			for(f=0;f<numero_procesos;f++){
				free(nombre_semaforo[f]);
				sem_close(semaforos[f]);
			}
			
			free(nombre_pid);
			
			exit(0);
		}
		
		
	}
	
	//Codigo que el padre ejecuta mientras los hijos esperan
	
	
	//El padre le asigna a cada hijo un unico numero y se lo comunica por archivo
	for(f = 0; f<numero_procesos; f++){
	
		pid_string[f] = (char *)malloc(sizeof(char)*100);
		sprintf(pid_string[f],"%d.txt",pid_hijos[f]);
		//printf("%s\n",pid_string[f]);
		arch_proceso_act = fopen(pid_string[f],"w");
		fprintf(arch_proceso_act,"%d\n", f);
		fclose(arch_proceso_act);
		free(pid_string[f]);
	}
	
	
	
	//Le comunica a los hijos que ya pueden tener acceso a sus archivos
	for(f = 0; f<numero_procesos; f++){
		sem_post(semaforos[f]);
		sem_post(semaforos[f]);
	}
	
	
	
	//Espera a que todos los hijos terminen
	while(wait(NULL) > 0);
	
	
	//Elimina a los semaforos
	for(f = 0; f<numero_procesos; f++){
		sem_close(semaforos[f]);
		sem_unlink(nombre_semaforo[f]);
	}
	
	
	//Lee los resultados que cada hijo guardo en su respectivo archivo
	//    y los guarda en un arreglo
	
	int posicion_array_pid = 0;
	facelook *facelook_array[cantidad_lineas_archivo];
	
	for(f = 0; f < numero_procesos; f++){
	
		pid_string[f] = (char *) malloc(sizeof(char)*100);
		sprintf(pid_string[f],"%d.txt",pid_hijos[f]);
		leer_archivo_pid(facelook_array, &posicion_array_pid, pid_string[f]);
		//free(pid_string[f]);
	}
	
	
	
	//Crea todos los pares y los guarda en un arreglo

	int i;
	int cantidad_pares_listas = cantidad_lineas_archivo*(cantidad_lineas_archivo-1)/2;
	lista *lista_array[cantidad_pares_listas];
	int pos_nodo_a = 0;
	int pos_nodo_b = 1;
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		lista_array[i] = fusionar_facelook(facelook_array[pos_nodo_a], facelook_array[pos_nodo_b]);
		//imprimir_lista(lista_array[i]);
		
		pos_nodo_b++;
		if(pos_nodo_b == cantidad_lineas_archivo){
		
			pos_nodo_b = (++pos_nodo_a) + 1;
			
			if (pos_nodo_a == cantidad_lineas_archivo-1){
				break;
			}
		}
	
	}
	
	
	//CONCURRENCIA
	
	
	sem_t *semaforos_reduce[numero_procesos];
	
	for(i = 0; i < numero_procesos; i++){
	
		sprintf(nombre_semaforo[i], "sem_red_%d", i);
		semaforos_reduce[i] = sem_open(nombre_semaforo[i], O_CREAT, 0644, 0);
	
	}
	
	//sem_t *semaforo_reduce = sem_open("sem_2", O_CREAT, 0644, 0);
	sem_t *semaforo_reduce_h;
	
	//Quita la redundancia de los pares creados arriba
	
	//lista *lista_array_reduced[cantidad_pares_listas];
	FILE *archivo_reduce_act;
	lista *lista_reduce;
	facelook *base_reduce;
	
	for(i = 0; i < numero_procesos; i++){
	
		pid_hijos[i] = fork();
		
		if (pid_hijos[i] < 0){
			printf("ERROR\n");
			exit(1);
		}
		else if (pid_hijos[i] == 0){
		
			//Codigo que los hijos ejecutan
			
			//Hijo espera por la asignacion de numero por archivo
			semaforo_reduce_h = sem_open(nombre_semaforo[i], O_CREAT, 0644, 0);
			sem_wait(semaforo_reduce_h);
			
			//Lee el numero asignado en archivo
			sprintf(nombre_pid, "%d.txt", getpid());
			archivo_reduce_act = fopen(nombre_pid, "r");
			fscanf(archivo_reduce_act, "%d%*c", &num_init_proceso);
			fclose(archivo_reduce_act);
			
			//Limpia archivo
			archivo_reduce_act = fopen(nombre_pid, "w");
			fclose(archivo_reduce_act);
			
			
			//Reduce redundancia de listas y las guarda en archivos
			for(f = num_init_proceso; f < cantidad_pares_listas; f += numero_procesos){
			
				base_reduce = (facelook *)malloc(sizeof(facelook));
				base_reduce->primero = NULL;
				base_reduce->ultimo = NULL;
				
				
				lista_reduce = reduce_2(lista_array[f]);
				insertar_perfil(base_reduce, lista_reduce);
				facelook_to_string( base_reduce , nombre_pid );
				
				eliminar_facelook(base_reduce);
				
			}
			
			//Deja de usar semaforo
			sem_close(semaforo_reduce_h);
			//printf("FIN %d\n",getpid());
			
			//Libera Memoria
			
			eliminar_facelook(base_datos);
			for(f=0;f<numero_procesos;f++){
				free(nombre_semaforo[f]);
				free(pid_string[f]);
				sem_close(semaforos_reduce[f]);
			}
			
			for(f=0;f<cantidad_pares_listas;f++){
				eliminar_lista(lista_array[f]);
			}
			
			for(f=0;f<cantidad_lineas_archivo;f++){
				eliminar_facelook(facelook_array[f]);
			}
			
			free(nombre_pid);
			
			exit(0);
		
		}
	
	}
	
	
	//Codigo que el padre ejecuta
	
	//Le asigna a cada proceso un unico numero por archivo
	
	FILE *archivo_pid_reduce;
	
	for(i = 0; i < numero_procesos; i++){
	
		sprintf(pid_string[i],"%d.txt",pid_hijos[i]);
		archivo_pid_reduce = fopen(pid_string[i], "w");
		fprintf(archivo_pid_reduce, "%d\n", i);
		fclose(archivo_pid_reduce);
		//free(pid_string[i]);
	
	}
	

	
	
	//Le comunica a los hijos que ya pueden tener acceso a sus archivos
	for(i = 0; i<numero_procesos; i++){
		sem_post(semaforos_reduce[i]);
		sem_post(semaforos_reduce[i]);
	}
	
	
	
	//Espera a que todos los hijos terminen
	while(wait(NULL) > 0);
	
	for(i = 0; i < numero_procesos ; i++){
	
		sem_close(semaforos_reduce[i]);
		sem_unlink(nombre_semaforo[i]);
		free(nombre_semaforo[i]);
	
	}
	
	
	//Lee los resultados que cada hijo guardo en su respectivo archivo
	//    y los guarda en un arreglo
	
	
	
	//int posicion_array_pid_reduce = 0;
	//facelook *facelook_array[cantidad_lineas_archivo];
	facelook *base_datos_reducida = (facelook *)malloc(sizeof(facelook));
	base_datos_reducida->primero = NULL;
	base_datos_reducida->ultimo = NULL;
	
	
	//Lee las listas guardads por hijos en archivos y las almacena en un facelook
	for(f = 0; f < numero_procesos; f++){
	
		//pid_string[f] = (char *) malloc(sizeof(char)*100);
		//sprintf(pid_string[f],"%d.txt",pid_hijos[f]);
		//printf("archivo: %s\n",pid_string[f]);
		leer_archivo_pid_reduce(base_datos_reducida, pid_string[f]);
		//free(pid_string[f]);
	}
	
	
	//imprimir_facelook(base_datos_reducida);
	
	//Limpia archivo de salida
	FILE *archivo_salida_limpiar = fopen(nombre_arch_salida, "w");
	fclose(archivo_salida_limpiar);
	
	//Guarda en archivo de salida
	facelook_to_string(base_datos_reducida, nombre_arch_salida);
	
	
	//Libera las memoria dinamica
	
	
	
	for(i = 0;i<cantidad_pares_listas;i++){
	
		eliminar_lista(lista_array[i]);
		
		
	}
	
	
	//eliminar_facelook(base_reduce);
	
	
	for(i = 0;i<cantidad_lineas_archivo;i++){
	
		eliminar_facelook(facelook_array[i]);
	}

	free(nombre_pid);
	
	eliminar_facelook(base_datos);

	eliminar_facelook(base_datos_reducida);
	
	//free(base_datos);

	for(f=0;f<numero_procesos;f++){
		free(pid_string[f]);
		//free(nombre_semaforo[f]);
	}
	
	

	return 0;

}
