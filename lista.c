#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

#define ERROR -1
#define EXITO 0



typedef struct nodo{	
	lista_t* lista_apuntada;
	struct nodo* nodo_siguiente;
}nodo_t;

struct lista{
	void* info;
	struct nodo* primer_nodo;
	struct nodo* nodo_final;
	int numero_listas;
};

struct lista_iterador{
	lista_t* lista;
	nodo_t* nodo;
	size_t indice;
};

lista_t* lista_crear(){
	lista_t* lista_creada=NULL;
	lista_creada=calloc(1,sizeof(lista_t));
	if(lista_creada==NULL)
		printf("La lista no ha sido creada, inténtelo más tarde.\n");
	return lista_creada;
}

bool lista_vacia(lista_t* lista){
	if(lista==NULL)
		return true;
	if(lista[0].numero_listas==0&&lista[0].info==NULL)
		return true;
	return false;
}

//Crea un puntero a un nodo
nodo_t* crear_nodo(){
	nodo_t* nodo_nuevo=NULL;
	nodo_nuevo=calloc(1,sizeof(nodo_t));
	if(nodo_nuevo==NULL)
		printf("Error creando sistema de nodos.\n");	
	return nodo_nuevo;
}


//Actualiza la lista madre
void actualizar_primer_lista(lista_t* lista, void* elemento, nodo_t* nodo_final){
	lista->numero_listas++;
	lista->nodo_final=nodo_final;
	if(nodo_final!=NULL)
		return;
	else
		lista->info=elemento;
}

//Actualiza nodos y datos acorde.
void actualizar_nodos_y_datos(lista_t* lista, lista_t* lista_nueva, nodo_t* nodo_nuevo, void* elemento){
	lista_nueva->info=elemento;
	nodo_nuevo->lista_apuntada=lista_nueva;
	if(lista->numero_listas==1){
		lista->primer_nodo=nodo_nuevo;
		return;
	}
	lista->nodo_final->nodo_siguiente=nodo_nuevo;

}

int lista_insertar(lista_t* lista, void* elemento){
	if(lista==NULL)
		return ERROR;
	if(lista->numero_listas==0){
		actualizar_primer_lista(lista, elemento, NULL);
		return EXITO;
	}
	lista_t* lista_nueva=NULL;
	lista_nueva=lista_crear();
	nodo_t* nodo_nuevo=NULL;
	nodo_nuevo=crear_nodo();
	if(lista_nueva==NULL||nodo_nuevo==NULL)
		return ERROR;
	actualizar_nodos_y_datos(lista, lista_nueva, nodo_nuevo, elemento);
	actualizar_primer_lista(lista, elemento, nodo_nuevo);
	return EXITO;
}

//Función recursiva que inserta nodo en cierta posición de cadena nodos, acorde con int auxiliar.
void insertar_nodo_recursivamente(nodo_t* cadena_nodos, nodo_t* nodo_auxiliar, nodo_t* nodo_nuevo, int auxiliar){
	if(auxiliar==1){
		nodo_auxiliar=cadena_nodos->nodo_siguiente;
		cadena_nodos->nodo_siguiente=nodo_nuevo;
		nodo_nuevo->nodo_siguiente=nodo_auxiliar;
	}
	else{
		auxiliar--;
		insertar_nodo_recursivamente(cadena_nodos->nodo_siguiente, nodo_auxiliar, nodo_nuevo, auxiliar); 
	}
}

//Iguala las listas envíadas
void igualar_listas(lista_t* lista_actualizada, lista_t* lista){
	lista_actualizada->nodo_final=lista->nodo_final;
	lista_actualizada->primer_nodo=lista->primer_nodo;
	lista_actualizada->info=lista->info;
}


//Inserta en posición cero, haciendo los cambios correspondientes.
int lista_insertar_en_posicion_0(lista_t* lista, void* elemento){
	lista_t* lista_auxiliar=NULL;
	nodo_t* nodo_nuevo=NULL;
	lista_auxiliar=lista_crear();
	nodo_nuevo=crear_nodo();
	if((nodo_nuevo==NULL)||(lista_auxiliar==NULL))
		return ERROR;
	igualar_listas(lista_auxiliar, lista);
	lista->primer_nodo=nodo_nuevo;
	nodo_nuevo->lista_apuntada=lista_auxiliar;
	nodo_nuevo->nodo_siguiente=lista_auxiliar->primer_nodo;
	lista->info=elemento;
	lista->numero_listas++;
	if(lista->nodo_final==NULL)
		lista->nodo_final=nodo_nuevo;
	return EXITO;
}

//Inserta en posición uno, haciendo los cambios correspondientes.
int insertar_en_posicion_1(lista_t* lista, void* elemento){
	lista_t* lista_nueva=NULL;
	lista_nueva=lista_crear();
	nodo_t* nodo_nuevo=NULL;
	nodo_nuevo=crear_nodo();
	if((nodo_nuevo==NULL)||(lista_nueva==NULL))
		return ERROR;
	nodo_nuevo->lista_apuntada=lista_nueva;
	nodo_nuevo->nodo_siguiente=lista->primer_nodo;
	lista->primer_nodo=nodo_nuevo;
	lista_nueva->info=elemento;
	lista->numero_listas++;
	return EXITO;
}

//Inserta en posición arbitraria entre 1 y el tope de listas, haciendo los cambios necesarios.
int lista_insertar_entre_1_y_tope(lista_t* lista, void* elemento, size_t posicion){
	nodo_t* nodo_auxiliar=NULL;
	nodo_auxiliar=crear_nodo();
	nodo_t* nodo_nuevo=NULL;
	nodo_nuevo=crear_nodo();
	lista_t* lista_nueva=NULL;
	lista_nueva=lista_crear();
	if((nodo_auxiliar==NULL)||(nodo_nuevo==NULL)||(lista_nueva==NULL))
		return ERROR;
	int auxiliar= (int)posicion-1;
	insertar_nodo_recursivamente(lista->primer_nodo, nodo_auxiliar, nodo_nuevo, auxiliar);
	lista_nueva->info=elemento;
	nodo_nuevo->lista_apuntada=lista_nueva;
	lista->numero_listas++; 
	free(nodo_auxiliar);
	return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	if(lista==NULL)
		return ERROR;
	if(posicion==0){
		if(lista_vacia(lista)){
			actualizar_primer_lista(lista, elemento,NULL);
			return EXITO;
		}else
			return lista_insertar_en_posicion_0(lista, elemento);
	}else if(posicion==1)
		return insertar_en_posicion_1(lista, elemento);
	else if((posicion>=(size_t)lista->numero_listas))
		return lista_insertar(lista, elemento);
	else
		return lista_insertar_entre_1_y_tope(lista, elemento, posicion);
	return ERROR;
}

//Función recursiva que busca el nodo a borrar.
void borrar_nodo_final_recursivamente(nodo_t* cadena_nodos, nodo_t* nodo_a_borrar, lista_t* lista){
	if(cadena_nodos->nodo_siguiente==nodo_a_borrar){
		free(nodo_a_borrar);
		lista->nodo_final=cadena_nodos;
		cadena_nodos->nodo_siguiente=NULL;
	}else
		borrar_nodo_final_recursivamente(cadena_nodos->nodo_siguiente, nodo_a_borrar, lista);
}
//Borra el primer y único nodo.
int borrar_primer_nodo(lista_t* lista){
	free(lista->nodo_final);
	lista->nodo_final=NULL;
	lista->primer_nodo=NULL;
	lista->numero_listas--;
	return EXITO;
}
//Hace que la lista madre esté vacía.
int borrar_primera_lista(lista_t* lista){
	lista->info=NULL;
	lista->numero_listas--;
	return EXITO;
}

int lista_borrar(lista_t* lista){
	if(lista==NULL)
		return ERROR;
	if(lista_vacia(lista))
		return ERROR;
	if(lista->numero_listas==1)
		return borrar_primera_lista(lista);
	else
		free(lista->nodo_final->lista_apuntada);
	if(lista->numero_listas==2)
		return borrar_primer_nodo(lista);		
	else{
		borrar_nodo_final_recursivamente(lista->primer_nodo, lista->nodo_final, lista);
		lista->numero_listas--;
	}
	return EXITO;
}

//Función recursiva que borra un nodo de cadena nodos, acorde con el auxiliar y la posición escogida.
void borrar_nodo_de_posicion_recursivamente(nodo_t* cadena_nodos, size_t posicion, int auxiliar){
	if(auxiliar+2==(int)posicion){
		nodo_t* nodo_auxiliar=cadena_nodos->nodo_siguiente;
		cadena_nodos->nodo_siguiente=cadena_nodos->nodo_siguiente->nodo_siguiente;
		free(nodo_auxiliar->lista_apuntada);
		free(nodo_auxiliar);
	}else{
		auxiliar++;
		borrar_nodo_de_posicion_recursivamente(cadena_nodos->nodo_siguiente, posicion, auxiliar);
	}
}

//Borra lista de la posición cero.
int lista_borrar_posicion_0(lista_t* lista){
	if(lista->primer_nodo==NULL){
		lista->info=NULL;
		lista->nodo_final=NULL;
		lista->numero_listas=0;
		return EXITO;
	}
	lista->info=lista->primer_nodo->lista_apuntada->info;
	free(lista->primer_nodo->lista_apuntada);
	nodo_t* nodo_auxiliar=lista->primer_nodo->nodo_siguiente;
	free(lista->primer_nodo);
	lista->primer_nodo=nodo_auxiliar;
	lista->numero_listas--;
	return EXITO;
}

//Borra lista de la posición 1
int lista_borrar_posicion_1(lista_t* lista){
	free(lista->primer_nodo->lista_apuntada);
	if(lista->numero_listas==2){
		free(lista->primer_nodo);
		return EXITO;
	}
	nodo_t* nodo_auxiliar=lista->primer_nodo->nodo_siguiente;
	free(lista->primer_nodo);
	lista->primer_nodo=nodo_auxiliar;
	lista->numero_listas--;
	return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if(lista==NULL)
		return ERROR;
	if(lista_vacia(lista))
		return ERROR;
	if(posicion==0){
		return lista_borrar_posicion_0(lista);
	}else if(posicion==1)
		return lista_borrar_posicion_1(lista);
	else if((int)posicion>=lista->numero_listas-1)
		return lista_borrar(lista);
	else{
		borrar_nodo_de_posicion_recursivamente(lista->primer_nodo, posicion, 0);
		lista->numero_listas--;
		return EXITO;
	}
	return ERROR;
}

//Función que busca nodo recursivamente de cadena nodos, acorde con posicion y auxiliar. 
void* buscar_elemento_de_nodo_recursivamente(nodo_t* cadena_nodos, size_t posicion, int auxiliar){
	if(auxiliar==(int)posicion)
		return cadena_nodos->lista_apuntada->info;
	else{
		auxiliar++;
		return buscar_elemento_de_nodo_recursivamente(cadena_nodos->nodo_siguiente, posicion, auxiliar);
	}
}

//Llama a la función recursiva de arriba.
void* devolver_elemento_intermedio(lista_t* lista, size_t posicion){
	return buscar_elemento_de_nodo_recursivamente(lista->primer_nodo, posicion, 1);
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if(lista==NULL)
		return NULL;
	if((int)posicion>=lista->numero_listas)
		return NULL;
	else if(posicion==0)
		return lista->info;
	else if(posicion==1)
		return lista->primer_nodo->lista_apuntada->info;
	else if((int)posicion==lista->numero_listas-1)
		return lista->nodo_final->lista_apuntada->info;
	else 
		return devolver_elemento_intermedio(lista, posicion);
}

void* lista_ultimo(lista_t* lista){
	if(lista==NULL)
		return NULL;
	if(lista_vacia(lista))
		return NULL;
	else if(lista->numero_listas==1)
		return lista->info;
	else
		return lista->nodo_final->lista_apuntada->info;
}

size_t lista_elementos(lista_t* lista){
	if(lista==NULL)
		return 0;
	return (size_t)lista->numero_listas;
}

//Función recursiva que libera las listas y los nodos que las apuntan.
void destruir_lista_recursivamente(lista_t* lista, nodo_t* cadena_nodos){
	if(cadena_nodos->nodo_siguiente==NULL){
		free(cadena_nodos->lista_apuntada);
		free(lista->nodo_final);
		free(lista);
	}else{
		nodo_t* nodo_auxiliar=cadena_nodos->nodo_siguiente;
		free(cadena_nodos->lista_apuntada);
		free(cadena_nodos);
		destruir_lista_recursivamente(lista, nodo_auxiliar);
	}
}

void lista_destruir(lista_t* lista){
	if(lista==NULL)
		return;
	if((lista_vacia(lista))||(lista->numero_listas==1)){
		free(lista);
		return;
	}else
		destruir_lista_recursivamente(lista, lista->primer_nodo);
}

int lista_apilar(lista_t* lista, void* elemento){
	if(lista==NULL)
		return ERROR;
	return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
	if(lista==NULL)
		return ERROR;
	return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
	if(lista==NULL)
		return NULL;
	return lista_ultimo(lista);
}

int lista_encolar(lista_t* lista, void* elemento){
	if(lista==NULL)
		return ERROR;
	return lista_insertar_en_posicion(lista, elemento, 0);
}

int lista_desencolar(lista_t* lista){
	if(lista==NULL)
		return ERROR;
	return lista_borrar(lista);
}

void* lista_primero(lista_t* lista){
	if(lista==NULL)
		return NULL;
	return lista_ultimo(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(lista==NULL)
		return NULL;
	lista_iterador_t* iterador=NULL;
	iterador=calloc(1, sizeof(lista_iterador_t));
	if(iterador==NULL)
		printf("Error creando iterador.\n");
	iterador->lista=lista;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(iterador==NULL)
		return false;
	else if(iterador->lista==NULL)
		return false;
	else if(lista_vacia((iterador->lista))||(iterador->lista->numero_listas==1))
		return false;
	else if((int)iterador->indice<iterador->lista->numero_listas)
		return true;
	return false;
}


void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(iterador==NULL)
		return NULL;
	else if(iterador->lista==NULL)
		return NULL;
	else if(lista_iterador_tiene_siguiente(iterador)==false)
		return NULL;
	else if((iterador->indice==0)&&(lista_vacia(iterador->lista)==false)){
		iterador->nodo=iterador->lista->primer_nodo;
		void* elemento=iterador->lista->info;
		iterador->indice++;
		return elemento;
	}
	else{
		void* elemento=iterador->nodo->lista_apuntada->info;
		iterador->nodo=iterador->nodo->nodo_siguiente;
		iterador->indice++;
		return elemento;
	}

}

void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador);
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
	lista_iterador_t* iterador=lista_iterador_crear(lista);
	
	if(iterador==NULL)
		return;

	if((lista->info)&&(lista->numero_listas==1))
		funcion(lista->info,contexto);

	while(lista_iterador_tiene_siguiente(iterador))
		funcion(lista_iterador_siguiente(iterador),contexto);
	lista_iterador_destruir(iterador);
}