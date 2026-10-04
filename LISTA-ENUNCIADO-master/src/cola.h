#ifndef COLA_H_
#define COLA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct cola cola_t;

/**
 * Crea una cola
 */
cola_t *cola_crear();

/**
 * Encola un elemento. Devuelve la cola o NULL en caso de error.
 */
cola_t *cola_encolar(cola_t *cola, void *elemento);

/**
 * Desencola un elemento. Devuelve el elemento desencolado o NULL en caso de error.
 */
void *cola_desencolar(cola_t *cola);

/**
 * Devuelve el primer elemento de la cola o NULL en caso de error.
 */
void *cola_frente(cola_t *cola);

/**
 * Devuelve la cantidad de elementos en la cola.
 */
size_t cola_cantidad(cola_t *cola);

/**
 * Destruye la cola
 */
void cola_destruir(cola_t *cola);

/**
 * Destruye la cola aplicando la función destructora a cada elemento.
 */
void cola_destruir_todo(cola_t *cola, void (*destructor)(void *));

#endif // COLA_H_
