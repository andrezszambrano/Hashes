#ifndef __TDA_PRUEBAS_H__
#define __TDA_PRUEBAS_H__

typedef struct tda_pruebas{
	int pruebas_totales;
	int pruebas_exitosas;
}tda_pruebas_t;

/*
Pos: devuelve un puntero a tda_pruebas_t.
*/
tda_pruebas_t* tda_pruebas_crear();

/*
Pre: recibe un string, y un puntero a tda_pruebas.
Pos: imprime por pantalla un aviso.
*/
void tda_pruebas_avisar(tda_pruebas_t* pruebas, char* aviso);

/*
Pre: recibe un string, un booleano y un puntero a tda_pruebas.
Pos: imprime por pantalla el string, añadiendo EXITO en verde sí el booleano es verdadero y ERROR en rojo para el caso contrario.
*/
void tda_pruebas_afirmar(tda_pruebas_t* pruebas, char* descripcion, bool declaracion);

/*
Pre: recibe un puntero a tda_pruebas.
Pos: imprime por pantalla las pruebas totales y las pruebas exitosas.
*/
void tda_pruebas_mostrar_estadisticas(tda_pruebas_t* pruebas);

/*
Pos: destruye tda_pruebas.
*/
void tda_pruebas_destruir(tda_pruebas_t* pruebas);


#endif /* __TDA_PRUEBAS_H__ */