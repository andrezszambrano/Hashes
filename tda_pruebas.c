#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tda_pruebas.h"



void rojo(){
	printf("\033[1;31m");
}
void verde(){
	printf("\033[1;32m");
}
void negro(){
	printf("\033[0m");
}
void amarillo(){
	printf("\033[1;33m");
}

tda_pruebas_t* tda_pruebas_crear(){
	tda_pruebas_t* pruebas=NULL;
	pruebas=calloc(1,sizeof(tda_pruebas_t));
	if(pruebas==NULL)
		printf("Error creando tda de pruebas\n");
	return pruebas;
}

void tda_pruebas_avisar(tda_pruebas_t* pruebas, char* aviso){
	amarillo();
	printf("AVISO: %s\n",aviso);
	negro();
}

void tda_pruebas_afirmar(tda_pruebas_t* pruebas, char* descripcion, bool declaracion){
	pruebas->pruebas_totales++;
	if(declaracion){
		verde();
		pruebas->pruebas_exitosas++;
	}
	else{
		rojo();
	}
	printf("PRUEBA: %s %s\n", descripcion, declaracion?"EXITO":"FALLA");
	negro();
}

void tda_pruebas_mostrar_estadisticas(tda_pruebas_t* pruebas){
	negro();
	printf("Pruebas totales: %i. Pruebas exitosas: %i.\n",pruebas->pruebas_totales, pruebas->pruebas_exitosas);
}

void tda_pruebas_destruir(tda_pruebas_t* pruebas){
	free(pruebas);
}