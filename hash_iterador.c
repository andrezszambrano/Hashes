#include "hash.h"
#include "hash_iterador.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAVE 50

struct hash_iter{
	hash_t* hash;
	int claves_recorridas;
	int posicion_en_vector_lista;
	int posicion_en_lista_actual;
};

typedef size_t(*funcion_de_hash_t)(unsigned char*);

struct hash{
	hash_destruir_dato_t destructor;
	int cantidad_elementos;
	lista_t** vector_listas;
	size_t capacidad;
	funcion_de_hash_t funcion_de_hash;
};


typedef struct elemento_lista{
	char clave[MAX_CLAVE];
	void* elemento;
}elemento_lista_t;

hash_iterador_t* hash_iterador_crear(hash_t* hash){

	if(!hash)
		return NULL;

	hash_iterador_t* hash_iterador=calloc(1, sizeof(hash_iterador_t));
	if(!hash_iterador)
		return NULL;

	hash_iterador->hash=hash;
	return hash_iterador;

}

bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador)
		return false;

	if(iterador->hash->cantidad_elementos<=iterador->claves_recorridas)
		return false;
	else
		return true;
}



const char* hash_iterador_siguiente(hash_iterador_t* iterador){

	if(!iterador)
		return NULL;

	if(!hash_iterador_tiene_siguiente(iterador))
		return NULL;

	iterador->claves_recorridas++;
	
	if(iterador->hash->vector_listas[iterador->posicion_en_vector_lista]){
		if(lista_elementos(iterador->hash->vector_listas[iterador->posicion_en_vector_lista])>iterador->posicion_en_lista_actual){
			elemento_lista_t* elemento=(elemento_lista_t*)lista_elemento_en_posicion(iterador->hash->vector_listas[iterador->posicion_en_vector_lista],(size_t)iterador->posicion_en_lista_actual);
			(iterador->posicion_en_lista_actual)++;
			return elemento->clave;
		}
	}

	(iterador->posicion_en_lista_actual)=0;
	int contador=1;
	bool elemento_en_vector=true;

	while(elemento_en_vector){

		if(iterador->hash->vector_listas[iterador->posicion_en_vector_lista+contador]){
			elemento_lista_t* elemento=(elemento_lista_t*)lista_elemento_en_posicion(iterador->hash->vector_listas[iterador->posicion_en_vector_lista+contador],(size_t)iterador->posicion_en_lista_actual);
			(iterador->posicion_en_lista_actual)++;
			iterador->posicion_en_vector_lista=iterador->posicion_en_vector_lista+contador;
			return elemento->clave;
		}
		contador++;

		if(contador>iterador->hash->capacidad)
			elemento_en_vector=false;
	}

	return NULL;

}





void hash_iterador_destruir(hash_iterador_t* iterador){

	free(iterador);

}
