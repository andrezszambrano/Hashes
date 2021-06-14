#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"
#include "tda_pruebas.h"
#include "hash_iterador.h"

#define EXITO 0
#define ERROR -1

#define MAX_CLAVE 50



void destruir_info(void* clave){

}


bool imprimir_claves_hash(hash_t* hash, const char* clave, void* aux){
	int* aux2=(int*)aux;
	*aux2=*aux2+1;
	printf("Clave: %s\n",clave);

	return false;
}

int* insercion_masiva(hash_t* hash){
	int* vector=calloc(25, sizeof(int));
	for(int i=0; i<25; i++)
		vector[i]=i;

	void* ptr_vector[25];
	for(int i=0; i<25; i++)
		ptr_vector[i]=(vector+i);

	hash_insertar(hash,"Posicion 0", ptr_vector[0]);
	hash_insertar(hash,"Posicion 1",ptr_vector[1]);
	hash_insertar(hash,"Posicion 2", ptr_vector[2]);
	hash_insertar(hash,"Posicion 3", ptr_vector[3]);
	hash_insertar(hash,"Posicion 4", ptr_vector[4]);
	hash_insertar(hash,"Posicion 5", ptr_vector[5]);
	hash_insertar(hash,"Posicion 6", ptr_vector[6]);
	hash_insertar(hash,"Posicion 7", ptr_vector[7]);
	hash_insertar(hash,"Posicion 8", ptr_vector[8]);
	hash_insertar(hash,"Posicion 9", ptr_vector[9]);
	hash_insertar(hash,"Posicion 10", (ptr_vector[10]));
	hash_insertar(hash,"Posicion 11", (ptr_vector[11]));
	hash_insertar(hash,"Posicion 12", (ptr_vector[12]));
	hash_insertar(hash,"Posicion 13", (ptr_vector[13]));
	hash_insertar(hash,"Posicion 14", (ptr_vector[14]));
	hash_insertar(hash,"Posicion 15", (ptr_vector[15]));
	hash_insertar(hash,"Posicion 16", (ptr_vector[16]));
	hash_insertar(hash,"Posicion 17", (ptr_vector[17]));
	hash_insertar(hash,"Posicion 18", (ptr_vector[18]));
	hash_insertar(hash,"Posicion 19", (ptr_vector[19]));
	hash_insertar(hash,"Posicion 20", (ptr_vector[20]));
	hash_insertar(hash,"Posicion 21", (ptr_vector[21]));
	hash_insertar(hash,"Posicion 22", (ptr_vector[22]));
	hash_insertar(hash,"Posicion 23", (ptr_vector[23]));
	hash_insertar(hash,"Posicion 24", (ptr_vector[24]));
	return vector;
}


void pruebas_parametros_null(tda_pruebas_t* pruebas){
	{
		printf("------------------------------------------------\n");
		printf("Funciones de hash.h\n");
		printf("------------------------------------------------\nMando NULL como Hash y como clave.\n");
		hash_t* hash=hash_crear(NULL, 0);
		char clave[2];
		int num=2;
		void* ptr=&num;
		tda_pruebas_afirmar(pruebas, "Hash_insertar devuelve error.", (hash_insertar(NULL, clave, ptr)==ERROR)&&(hash_insertar(hash, NULL, ptr)==ERROR));
		tda_pruebas_afirmar(pruebas, "Hash_quitar devuelve error.", (hash_quitar(NULL, clave)==ERROR)&&(hash_quitar(hash, NULL)==ERROR));
		tda_pruebas_afirmar(pruebas, "Hash_obtener devuelve NULL.", (hash_obtener(NULL, clave)==NULL)&&(hash_obtener(hash, NULL)==NULL));
		tda_pruebas_afirmar(pruebas, "Hash_contiene devuelve false.", (hash_contiene(NULL, clave)==false)&&(hash_contiene(hash, NULL)==false));
		printf("Mando a hash_iterador interno Hash NULL y función NULL.\n");
		tda_pruebas_afirmar(pruebas, "Hash_con_cada_clave devuelve 0.", (hash_con_cada_clave(NULL, &imprimir_claves_hash,NULL)==0)&&(hash_con_cada_clave(hash, NULL, NULL)==0));
		tda_pruebas_avisar(pruebas,"Destrucción de Hash NULL.");
		hash_destruir(NULL);
		printf("------------------------------------------------\n");
		hash_destruir(hash);
	}
	{
		printf("Funciones de hash_iterador.c\n");
		printf("------------------------------------------------\nMando Hash NULL a hash_iterador_crear.\n");
		hash_iterador_t* iterador=hash_iterador_crear(NULL);
		tda_pruebas_afirmar(pruebas, "Hash_iterador_crear devuelve NULL.", !iterador);
		printf("Mando iterador NULL a las otras funciones.\n");
		tda_pruebas_afirmar(pruebas, "hash_iterador_siguiente devuelve NULL.", !hash_iterador_siguiente(iterador));
		tda_pruebas_afirmar(pruebas, "hash_iterador_tiene_siguiente devuelve false.", !hash_iterador_tiene_siguiente(iterador));
		tda_pruebas_avisar(pruebas,"Destrucción de iterador NULL.");
		printf("------------------------------------------------\n");
		hash_iterador_destruir(iterador);
	}
}

void probar_funciones_basicas_hash_c(tda_pruebas_t* pruebas){
	{
		printf("------------------------------------------------\n");
		printf("Probando funciones básicas Hash.c.\n");
	}
	{
		hash_t* hash=hash_crear(NULL, 0);
		printf("------------------------------------------------\nPruebas en Hash vacío\n");
		tda_pruebas_afirmar(pruebas, "Hash se creó correctamente.", hash);
		tda_pruebas_afirmar(pruebas, "Hash vacío tiene 0 elementos.", hash_cantidad(hash)==0);
		tda_pruebas_afirmar(pruebas, "Hash_contiene devuelve false mandandole una clave arbitraria.", (!hash_contiene(hash, "Prueba contiene")) );
		tda_pruebas_afirmar(pruebas, "Hash_obtener devuelve NULL al pedir un elemento con clave arbitraria.", (!hash_obtener(hash, "Prueba obtener")));
		tda_pruebas_afirmar(pruebas, "Hash_obtener devuelve ERROR al pedir quitar un elemento con clave arbitraria.", (hash_quitar(hash, "Prueba quitar")==ERROR));		
		size_t cont=hash_con_cada_clave(hash,NULL ,NULL);
		tda_pruebas_afirmar(pruebas, "Hash_con_cada_clave devuelve 0.", cont==0);		

		printf("------------------------------------------------\n");
		hash_destruir(hash);
	}
	{
		hash_t* hash1=hash_crear(NULL,3);
		hash_t* hash2=hash_crear(NULL,10);
		hash_t* hash3=hash_crear(NULL,50);
		printf("------------------------------------------------\nCreo 3 Hashes distintos, capacidades de: 3, 10, 50.\n");
		tda_pruebas_avisar(pruebas,"Destrucción de los 3 Hashes.");
		printf("------------------------------------------------\n");
		hash_destruir(hash1);
		hash_destruir(hash2);
		hash_destruir(hash3);
	}
	{
		hash_t* hash=hash_crear(&destruir_info, 0);
		int numero=2, elemento=3;
		void* ptr=&numero;
		printf("------------------------------------------------\n");
		tda_pruebas_afirmar(pruebas, "Se insertó correctamente en Hash vacío.", hash_insertar(hash,"Prueba insertar",ptr)==EXITO);
		tda_pruebas_afirmar(pruebas, "Función hash_cantidad devuelve 1 elemento.", hash_cantidad(hash)==1);
		ptr=&elemento;
		printf("\nInserto un elemento con la misma clave.\n");
		tda_pruebas_afirmar(pruebas, "Se insertó correctamente en Hash.", hash_insertar(hash,"Prueba insertar",ptr)==EXITO);		
		tda_pruebas_afirmar(pruebas, "Función hash_cantidad devuelve 1 elemento.", hash_cantidad(hash)==1);
		tda_pruebas_afirmar(pruebas, "Función hash_contiene dice que existe el elemento en el Hash.", hash_contiene(hash,"Prueba insertar"));
		printf("\nBusco el elemento con hash_obtener.\n");
		int* ptr_num=(int*)hash_obtener(hash, "Prueba insertar");
		tda_pruebas_afirmar(pruebas, "Se regresó el elemento correspondiente.", *ptr_num==elemento);	
		printf("\nElimino el elemento.\n");
		tda_pruebas_afirmar(pruebas, "Se eliminó el elemento correspondiente.", (hash_quitar(hash, "Prueba insertar")==EXITO));				
		tda_pruebas_afirmar(pruebas, "Hash tiene 0 elementos", hash_cantidad(hash)==0);		
		printf("------------------------------------------------\n");
		hash_destruir(hash);
	}
	{
		hash_t* hash=hash_crear(NULL, 10);
		printf("------------------------------------------------\nAgrego 10 veces la misma clave a Hash vacío.\n");
		int vector[10];
		for(int i=0; i<10;i++)	
			vector[i]=i;

		void* vector_ptr[10];
		for(int i=0; i<10; i++)
			vector_ptr[i]=(vector+i);

		for(int i=0; i<10; i++)
			hash_insertar(hash, "Prueba misma clave", vector_ptr[i]);

		tda_pruebas_afirmar(pruebas, "Hash contiene solo un elemento.", hash_cantidad(hash)==1);	
		void* ptr=hash_obtener(hash, "Prueba misma clave");
		tda_pruebas_afirmar(pruebas, "Se regresó el último elemento insertado.", *((int*)ptr)==vector[9]);	
		printf("------------------------------------------------\n");
		hash_destruir(hash);
	}
	{
		hash_t* hash=hash_crear(NULL, 10);
		printf("------------------------------------------------\nAgrego 10 claves distintas a Hash vacío.\n");
		char vector_char[20]="Esto es una prueba.";
		void* ptr[10];
		for(int i=0; i<10;i++)
			ptr[i]=(vector_char+2*i);

		for(int i=0; i<10; i++)
			hash_insertar(hash, (vector_char+2*i), ptr[i]);

		tda_pruebas_afirmar(pruebas, "Hay 10 elementos en el Hash.", hash_cantidad(hash)==10);	
		tda_pruebas_avisar(pruebas, "Recorro Hash con hash_con_cada_clave mandandole una función NULL");
		size_t cont=hash_con_cada_clave(hash,NULL ,NULL);
		tda_pruebas_afirmar(pruebas, "Se recorrieron los 10 elementos del Hash.", cont==10);	
		tda_pruebas_avisar(pruebas, "Quito los 10 elementos del Hash, siendo el destructor del Hash nulo.");
		bool hubo_error=false;
		for(int i=0; i<10; i++){
			if(hash_quitar(hash, (vector_char+2*i))==ERROR)
				hubo_error=true;
		}
		tda_pruebas_afirmar(pruebas, "No hubo error al quitar los elementos.", !hubo_error);	
		tda_pruebas_afirmar(pruebas, "Hay 0 elementos en el Hash.", hash_cantidad(hash)==0);
		printf("Pido/quito un elemento arbitrario.\n");
		tda_pruebas_afirmar(pruebas, "Hash_obtener devuelve NULL.", !hash_obtener(hash, (vector_char+5)));			
		tda_pruebas_afirmar(pruebas, "Hash_contiene devuelve false.", !hash_contiene(hash, (vector_char+5)));			
		tda_pruebas_afirmar(pruebas, "Hash_quitar devuelve ERROR.", ERROR==hash_quitar(hash, (vector_char+5)));			
		printf("------------------------------------------------\n");
		hash_destruir(hash);
	}
	{
		hash_t* hash=hash_crear(&destruir_info, 5);
	
		
		printf("------------------------------------------------\nInserto 25 claves distintas en Hash vacío.\n");
		int *vector=insercion_masiva(hash);

		int i=0;
		printf("Hash_con_cada_clave y las imprimo.\n");
		size_t cont=hash_con_cada_clave(hash, &imprimir_claves_hash,&i);
		tda_pruebas_afirmar(pruebas,"Hash_con_cada_clave regresa el número de claves totales.",cont==hash_cantidad(hash));
		printf("Inserto clave repetida.\n");
		int elemento=0;
		hash_insertar(hash, "Posicion 10", &elemento);
		tda_pruebas_afirmar(pruebas,"Hash_cantidad regresa el mismo número de claves totales.",cont==hash_cantidad(hash));
		printf("Quito dos elementos arbitrarios del Hash.\n");
		hash_quitar(hash,"Posicion 15");
		hash_quitar(hash,"Posicion 19");
		tda_pruebas_afirmar(pruebas,"Hash_cantidad regresa el número de claves totales correspondiente.",cont-2==hash_cantidad(hash));
		printf("Vuelvo a recorrer el hash para verificar que las posiciones no han cambiado.\n");
		cont=hash_con_cada_clave(hash, &imprimir_claves_hash,&i);
		tda_pruebas_afirmar(pruebas,"Hash_con_cada_clave regresa el número de claves totales.",cont==hash_cantidad(hash));
		printf("------------------------------------------------\n");
		hash_destruir(hash);
		free(vector);
	}
}

void probar_funciones_basicas_hash_iterador_c(tda_pruebas_t* pruebas){
	{
		printf("------------------------------------------------\n");
		printf("Probando funciones básicas hash_iterador.h.\n");	
	}
	{
		printf("------------------------------------------------\n");
		hash_t* hash=hash_crear(NULL, 5);
		hash_iterador_t* iterador= hash_iterador_crear(hash);
		tda_pruebas_afirmar(pruebas, "Iterador fue creado correctamente con un Hash vacío.", iterador);
		tda_pruebas_afirmar(pruebas, "Iterador no tiene siguiente.", !hash_iterador_tiene_siguiente(iterador));
		tda_pruebas_afirmar(pruebas, "Hash_iterador_siguiente devuelve NULL.", hash_iterador_siguiente(iterador)==NULL);
		printf("------------------------------------------------\n");
		hash_iterador_destruir(iterador);
		hash_destruir(hash);
	}
	{
		printf("------------------------------------------------\nCreo un Hash, le inserto y quito el mismo elemento y uso el iterador.\n");
		hash_t* hash=hash_crear(NULL, 5);
		int elemento=0;
		hash_insertar(hash, "Prueba", &elemento);
		hash_quitar(hash, "Prueba");
		hash_iterador_t* iterador= hash_iterador_crear(hash);
		tda_pruebas_afirmar(pruebas, "Iterador no tiene siguiente.", !hash_iterador_tiene_siguiente(iterador));
		tda_pruebas_afirmar(pruebas, "Iterador_siguiente devuelve NULL.", !hash_iterador_siguiente(iterador));
		printf("------------------------------------------------\n");
		hash_iterador_destruir(iterador);
		hash_destruir(hash);
						

	}
	{
		printf("------------------------------------------------\nCreo Hash y le inserto 10 elementos.\n");
		hash_t* hash=hash_crear(NULL, 5);
		
		char vector_char[20]="Esto es una prueba.";
		void* ptr[10];
		for(int i=0; i<10;i++)
			ptr[i]=(vector_char+2*i);

		for(int i=0; i<10; i++)
			hash_insertar(hash, (vector_char+2*i), ptr[i]);

		hash_iterador_t* iterador= hash_iterador_crear(hash);	
		int cont=0;
		while(hash_iterador_tiene_siguiente(iterador)){
			hash_iterador_siguiente(iterador);
			cont++;
		}
		tda_pruebas_afirmar(pruebas, "Contador de elementos iterados es el correspondiente.",cont==10);
		printf("------------------------------------------------\n");
		hash_iterador_destruir(iterador);
		hash_destruir(hash);
	}
	{
		printf("------------------------------------------------\nCreo Hash y le inserto 25 elementos.\n");
		hash_t* hash=hash_crear(NULL, 5);
		int* vector=insercion_masiva(hash);

		hash_iterador_t* iterador= hash_iterador_crear(hash);	
		int cont=0;
		while(hash_iterador_tiene_siguiente(iterador)){
			char* ptr=(char*)hash_iterador_siguiente(iterador);
			printf("Clave: %s, Elemento: %i.\n",ptr, *((int*)hash_obtener(hash, ptr)));
			cont++;
		}
		tda_pruebas_afirmar(pruebas, "Contador de elementos iterados es el correspondiente.",cont==25);
		printf("------------------------------------------------\n");
		hash_iterador_destruir(iterador);
		hash_destruir(hash);
		free(vector);
	}
}


//Pruebas de caja negra.
void probar_funciones_de_hash(tda_pruebas_t* pruebas){
	printf("Pruebas NULL.\n");
	pruebas_parametros_null(pruebas);
	printf("Probando funciones básicas.\n");
	probar_funciones_basicas_hash_c(pruebas);
	probar_funciones_basicas_hash_iterador_c(pruebas);
}




int main(){
	tda_pruebas_t* tda_pruebas=tda_pruebas_crear();
	printf("\nPRUEBAS DE HASH\n\n");
	probar_funciones_de_hash(tda_pruebas);
	tda_pruebas_mostrar_estadisticas(tda_pruebas);
	tda_pruebas_destruir(tda_pruebas);
	return 0;	
}