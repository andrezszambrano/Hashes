#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "lista.h"



#define EXITO 0
#define ERROR -1
#define MAX_CLAVE 50
#define CAPACIDAD_MINIMA 3
#define FACTOR_DE_OCUPACION 1
#define PROCEDIMIENTO_OBTENER 2
#define PROCEDIMIENTO_CONTIENE 1
#define PROCEDIMIENTO_BORRAR 0



typedef size_t(*funcion_de_hash_t)(unsigned char*);

typedef bool (*funcion_hash_con_cada_elemento_t)(hash_t* , const char* , void* );

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

typedef struct estructura_auxiliar{
	char clave[MAX_CLAVE];
	bool clave_repetida;
	int contador;
}estructura_auxiliar_t;

typedef struct estructura_destruir{
	hash_destruir_dato_t destructor;
	int cant_elementos_destruidos;
}estructura_destruir_t;

typedef struct estructura_hash_con_cada_elemento{
	hash_t* hash;
	void* aux_iterador;
	int cant_elementos_recorridos;
	bool parar_recorrido;
	funcion_hash_con_cada_elemento_t funcion;
}estructura_hash_con_cada_elemento_t;


//Función de hasheo. Es O(n) pero ésto le asegura la máxima aleatoriedad posible. Las claves de una letra devuelven el mismo número, por lo que se recomendará en el readme utilizar claves de más de una letra.
size_t funcion_de_hasheo(unsigned char* clave){

	size_t hash=2003;
	size_t hasheo_aux=69;
	size_t pos=0;
	while(clave[pos]){
		hash=hash+pos*clave[pos]*(hasheo_aux-pos);
		pos++;
	}

	return hash;
}



hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){

	hash_t* hash=calloc(1, sizeof(hash_t));
	if(!hash)
		return NULL;

	if(capacidad<=CAPACIDAD_MINIMA)
		hash->capacidad=CAPACIDAD_MINIMA;
	else
		hash->capacidad=capacidad;

	hash->vector_listas=calloc(hash->capacidad, sizeof(lista_t*));
	if(!hash->vector_listas){
		free(hash);
		return NULL;
	}

	hash->destructor=destruir_elemento;



	hash->funcion_de_hash=&funcion_de_hasheo;

	return hash;
}


//Dada una posición del vector lista del hash y un elemento, se crea una lista para dicha posición y se inserta dicho elemento.
int hash_insertar_posicion_vacia(hash_t* hash, elemento_lista_t* elemento_lista, size_t posicion){

	hash->vector_listas[posicion]=lista_crear();
	if(!hash->vector_listas[posicion])
		return ERROR;
	hash->cantidad_elementos++;

	return lista_insertar(hash->vector_listas[posicion], (void*)elemento_lista);

}

//Función auxiliar que recorre la lista en una posición del vector de listas del hash, para encontrar la posición de la clave buscada en dicha lista.
void recorrer_lista_clave_repetida(void* elemento_arbitrario_lista, void* estructura_auxiliar){

	estructura_auxiliar_t* aux=estructura_auxiliar;

	if(aux->clave_repetida)
		return;

	if(strcmp(aux->clave, ((estructura_auxiliar_t*)(elemento_arbitrario_lista))->clave)==0){
		aux->clave_repetida=true;
		return;
	}

	aux->contador++;
}


//En caso de que ya se encuentre la clave en el Hash, la función elimina la que fue antes insertada.
void procedimiento_clave_repetida(hash_t* hash, size_t posicion, estructura_auxiliar_t* ptr_estructura_aux){

	elemento_lista_t* elemento_a_actualizar=(elemento_lista_t*)lista_elemento_en_posicion(hash->vector_listas[posicion], (size_t)ptr_estructura_aux->contador);
	if(hash->destructor)
		(hash->destructor)(elemento_a_actualizar->elemento);

	free(elemento_a_actualizar);
	//Para actualizar la lista, elimino el elemento con la clave anterior. Luego, al volver a la función principal, se inserta el nuevo elemento con la misma clave.
	lista_borrar_de_posicion(hash->vector_listas[posicion], (size_t)ptr_estructura_aux->contador);
}


//Inicializa la estructura auxiliar para recorrer la lista de claves.
void inicializar_estructura_auxiliar(estructura_auxiliar_t* estructura, char* clave){
	estructura->clave_repetida=false;
	strcpy(estructura->clave, clave);
	estructura->contador=0;
}


//Inserta elemento en una lista ya existente. Se busca sí la clave ya se había ingresado, en caso contrario se suma a la cantidad de elementos del hash. 
int hash_insertar_colision(hash_t* hash, elemento_lista_t* elemento_lista, size_t posicion){

	estructura_auxiliar_t estructura;
	inicializar_estructura_auxiliar(&estructura, elemento_lista->clave);
	
	void* ptr=&estructura;
	lista_con_cada_elemento(hash->vector_listas[posicion], &recorrer_lista_clave_repetida, ptr);

	if(((estructura_auxiliar_t*)(ptr))->clave_repetida)
		procedimiento_clave_repetida(hash, posicion,(estructura_auxiliar_t*)ptr);
	else
		hash->cantidad_elementos++;
	
	return lista_insertar(hash->vector_listas[posicion],(void*)elemento_lista);
	
}


bool es_primo_r(int n, int divisor){

	if(divisor==1)
		return true;

	if(n%divisor==0&&divisor!=n)
		return false;

	return es_primo_r(n, divisor-2);

}
//Función auxiliar que busca sí n es un número primo. Una joyita del parcial.
bool es_primo(int n){

	if(n==1||n<1)
		return false;

	int decremento=0;
	if(n%2==0)
		decremento=1;

	return es_primo_r(n, n-decremento);


} 
//Función auxiliar que busca el número primo más cercano al número del parámetro.
int numero_primo_cercano(int numero){
	if(numero%2==0)
		numero++;

	while(!es_primo(numero))
		numero=numero+2;
	
	return numero;

}

//Inserta en el hash auxiliar los elementos de una lista del vector de listas viejo. Después libera el espacio.
void procedimiento_rehasheo(hash_t* hash, int posicion_vieja, hash_t* hash_aux){

	int i=0;

	while(i<(int)lista_elementos(hash->vector_listas[posicion_vieja])){
		elemento_lista_t* elemento_lista=lista_elemento_en_posicion(hash->vector_listas[posicion_vieja], (size_t)i);
		hash_insertar(hash_aux, elemento_lista->clave, elemento_lista->elemento);
		free(elemento_lista);
		i++;
	}

	lista_destruir(hash->vector_listas[posicion_vieja]);
}

//Función auxiliar que iguala los contenidos del hash auxiliar al hash original.
void igualar_hashes(hash_t* hash, hash_t* hash_aux){
	hash->vector_listas=hash_aux->vector_listas;
	hash->capacidad=hash_aux->capacidad;
}

//Función que es utilizada cuando el factor de ocupación es igualado o superado.
//Crea un hash auxiliar con una nueva capacidad. 
//Se insertan los elementos del hash original al hash auxiliar. Al final se igualan los hashes y se libera el hash auxiliar.
int rehashear(hash_t* hash){
	
	int nueva_capacidad=numero_primo_cercano(2*((int)hash->capacidad)+1);
	hash_t* hash_aux=hash_crear(hash->destructor, (size_t)nueva_capacidad);
	if(!hash_aux)
		return ERROR;

	int contador=0;
	while(contador<hash->capacidad){
		if(hash->vector_listas[contador])
			procedimiento_rehasheo(hash, contador, hash_aux);

		contador++;
	}

	free(hash->vector_listas);
	igualar_hashes(hash, hash_aux);
	free(hash_aux);
	return EXITO;
}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){

	if(!hash||!clave)
		return ERROR;

	unsigned char* copia_clave=(unsigned char*) clave;

	size_t posicion=((hash->funcion_de_hash)(copia_clave))%(hash->capacidad);

	elemento_lista_t* elemento_lista=calloc(1, sizeof(elemento_lista_t));
	if(!elemento_lista)
		return ERROR;

	elemento_lista->elemento=elemento;
	strcpy(elemento_lista->clave, clave);

	int aux; 
	if(!hash->vector_listas[posicion])
		aux = hash_insertar_posicion_vacia(hash, (void*)elemento_lista, posicion);
		
	else
		aux = hash_insertar_colision(hash, (void*)elemento_lista, posicion);

	int aux2=EXITO;
	if(((size_t)(hash->cantidad_elementos))/(hash->capacidad)>=FACTOR_DE_OCUPACION)
		aux2=rehashear(hash);

	if((aux==EXITO)&&(aux2==EXITO))
		return EXITO;

	return ERROR;
}

//Procedimiento que, dada una lista del vector lista del hash, elimina de dicha lista el elemento que se encuentra en el contador.
void* procedimineto_hash_borrar(hash_t* hash, elemento_lista_t* elemento_lista, size_t posicion, int contador, int* ptr_auxiliar){
	
	if(hash->destructor)
		(hash->destructor)(elemento_lista->elemento);

	free(elemento_lista);
	*ptr_auxiliar=lista_borrar_de_posicion(hash->vector_listas[posicion], (size_t)contador);

	if(lista_vacia(hash->vector_listas[posicion])){
		lista_destruir(hash->vector_listas[posicion]);
		hash->vector_listas[posicion]=NULL;
	}
	
	return ptr_auxiliar; 
}



//Función con múltiples funcionalidades, que busca un elemento con cierta clave.
//Sí es invocada por la función hash_obtener o hash_contiene, se devuelve el elemento a la función original, o NULL en caso de que no se encuentre en la lista.
//Sí es invocada por la función hash_quitar, se hace un procedimiento adicional para borrar el elemento de la lista.
void* procedimiento_hash_tiene(hash_t* hash, size_t posicion, const char* clave, int aux_funcion, int* ptr_auxiliar){
	estructura_auxiliar_t estructura;
	char* clave_aux=(char*)clave;
	inicializar_estructura_auxiliar(&estructura, clave_aux);

	void* ptr=&estructura;
	lista_con_cada_elemento(hash->vector_listas[posicion], &recorrer_lista_clave_repetida, ptr);
	
	if(!estructura.clave_repetida)
		return NULL;
	
	elemento_lista_t* elemento_lista = lista_elemento_en_posicion(hash->vector_listas[posicion], (size_t)estructura.contador);

	if(aux_funcion==PROCEDIMIENTO_BORRAR)
		return procedimineto_hash_borrar(hash, elemento_lista,posicion, estructura.contador, ptr_auxiliar);

	return (void*)(elemento_lista->elemento);
}


void* hash_obtener(hash_t* hash, const char* clave){

	if(!hash||!clave)
		return NULL;

	unsigned char* copia_clave=(unsigned char*)clave;
	size_t posicion=((hash->funcion_de_hash)(copia_clave))%(hash->capacidad);
	
	if(!hash->vector_listas[posicion])
		return NULL;
	
	return procedimiento_hash_tiene(hash, posicion,clave, PROCEDIMIENTO_OBTENER, NULL);
		

}

bool hash_contiene(hash_t* hash, const char* clave){

	if(!hash||!clave)
		return false;

	unsigned char* copia_clave=(unsigned char*)clave;
	size_t posicion=(hash->funcion_de_hash)(copia_clave)%(hash->capacidad);

	if(!hash->vector_listas[posicion])
		return false;
	else{
		void* ptr_aux=procedimiento_hash_tiene(hash, posicion, clave, PROCEDIMIENTO_CONTIENE, NULL);
		if(!ptr_aux)
			return false;
		else
			return true;
	}
}

int hash_quitar(hash_t* hash, const char* clave){

	if(!hash||!clave)
		return ERROR;

	unsigned char* copia_clave=(unsigned char*)clave;
	size_t posicion=(hash->funcion_de_hash)(copia_clave)%(hash->capacidad);

	if(!hash->vector_listas[posicion])
		return ERROR;
	else{
		int numero_aux;
		void* ptr_aux=procedimiento_hash_tiene(hash, posicion, clave, PROCEDIMIENTO_BORRAR, &numero_aux);
		
		if((!ptr_aux)||(numero_aux==ERROR))
			return ERROR;
		hash->cantidad_elementos--;
		return EXITO;
	}

}



size_t hash_cantidad(hash_t* hash){

	if(!hash)
		return 0;

	return (size_t)hash->cantidad_elementos;

}


//Función que se pasa a lista_con_cada_elemento para usar el destructor, en caso de que éste exista, y se libere la memoria de la estructura.
void funcion_destructora(void* elemento_arbitrario_lista, void* ptr_num_elementos){

	estructura_destruir_t* estructura_destruir=(estructura_destruir_t*)ptr_num_elementos;
	elemento_lista_t* elemento_lista=(elemento_lista_t*)elemento_arbitrario_lista;

	if(estructura_destruir->destructor)
		(estructura_destruir->destructor)(elemento_lista->elemento);

	free(elemento_lista);

	(estructura_destruir->cant_elementos_destruidos)++;
}




void hash_destruir(hash_t* hash){

	if(!hash)
		return;

	int contador=0;
	estructura_destruir_t estructura_destruir;
	estructura_destruir.cant_elementos_destruidos=0;
	estructura_destruir.destructor=hash->destructor;
	void* ptr_aux=&estructura_destruir;
	while((contador<hash->capacidad)||(estructura_destruir.cant_elementos_destruidos<hash->cantidad_elementos)){

		if(hash->vector_listas[contador]){
			lista_con_cada_elemento(hash->vector_listas[contador], &funcion_destructora,ptr_aux);
			lista_destruir(hash->vector_listas[contador]);
		}

		contador++;
	}

	free(hash->vector_listas);
	free(hash);
}

//Función auxiliar que es mandada a lista_con_cada_elemento, para recorrer la lista que se encuentra en una posición del vector listas del hash.
//No se hace nada sí hay que parar el recorrido.
//Sí existe la función mandada a hash_con_cada_clave, entonces se utiliza.
//Al final se adiciona 1 a la cantidad de elementos recorridos.
void funcion_aux_lista_con_cada_elemento(void* elemento_arbitrario_lista, void* ptr_estructura_iterador){

	estructura_hash_con_cada_elemento_t* estructura=(estructura_hash_con_cada_elemento_t*)ptr_estructura_iterador;
	if(estructura->parar_recorrido)
		return;
	
	if(estructura->funcion){
		if((estructura->funcion)(estructura->hash,(((elemento_lista_t*)(elemento_arbitrario_lista))->clave), estructura->aux_iterador))
			estructura->parar_recorrido=true;
	}
	(estructura->cant_elementos_recorridos)++;

}

//Función aux que inicializa la estructura de iteraciones.
void inicializar_estructura_iteradora(hash_t* hash, estructura_hash_con_cada_elemento_t* estructura, bool (*funcion)(hash_t* hash, const char* clave, void* aux),void* aux){
	estructura->hash=hash;
	estructura->aux_iterador=aux;
	estructura->cant_elementos_recorridos=0;
	estructura->funcion=funcion;
	estructura->parar_recorrido=false;

}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){

	if(!hash)
		return 0;

	int contador=0;
	estructura_hash_con_cada_elemento_t estructura;
	inicializar_estructura_iteradora(hash, &estructura, funcion,aux);
	void* ptr=&estructura;
	while((contador<hash->capacidad)&&(!estructura.parar_recorrido)){
		if(hash->vector_listas[contador]){
			lista_con_cada_elemento(hash->vector_listas[contador], &funcion_aux_lista_con_cada_elemento,ptr);
		}
		contador++;
	}

	return (size_t)estructura.cant_elementos_recorridos;

}