#ifndef PILA_H_
#define PILA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct pila pila_t;

/**
 * Crea una pila
 */
pila_t *pila_crear();

/*
* Apila un elemento en la pila. Devuelve la pila o NULL en caso de error.
*/
pila_t *pila_apilar(pila_t *pila, void *elemento);

/**
 * Desapila un elemento y lo devuelve. Devuelve NULL en caso de error.
 */
void *pila_desapilar(pila_t *pila);

/**
 * Devuelve el primer elemento de la pila y lo devuelve (o NULL en caso de error).
 */
void *pila_tope(pila_t *pila);

/**
 * Devuelve la cantidad de elementos en la pila.
 */
size_t pila_cantidad(pila_t *pila);

/**
 * Destruye la pila
 */
void pila_destruir(pila_t *pila);

/**
 * Destruye la pila aplicando la función destructora a cada elemento.
 */
void pila_destruir_todo(pila_t *pila, void (*destructor)(void *));

#endif // PILA_H_
