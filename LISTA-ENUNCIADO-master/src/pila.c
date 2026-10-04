#include "pila.h"
#include "lista.h"
#include <stdlib.h>

struct pila {
	lista_t *lista;
};

pila_t *pila_crear()
{
	pila_t *pila = malloc(sizeof(pila_t));
	if (!pila)
		return NULL;

	pila->lista = lista_crear();
	if (!pila->lista) {
		free(pila);
		return NULL;
	}

	return pila;
}

bool pila_esta_vacia(pila_t *pila)
{
	return !pila || lista_cantidad(pila->lista) == 0;
}

bool pila_apilar(pila_t *pila, void *elemento)
{
	if (!pila)
		return false;

	return lista_insertar(pila->lista, elemento, 0);
}

void *pila_desapilar(pila_t *pila)
{
	if (pila_esta_vacia(pila))
		return NULL;

	return lista_eliminar(pila->lista, 0);
}

void *pila_tope(pila_t *pila)
{
	if (pila_esta_vacia(pila))
		return NULL;

	return lista_obtener(pila->lista, 0);
}

size_t pila_cantidad(pila_t *pila)
{
	if (!pila)
		return 0;

	return lista_cantidad(pila->lista);
}

void pila_destruir(pila_t *pila)
{
	if (!pila)
		return;

	lista_destruir(pila->lista);
	free(pila);
}

void pila_destruir_todo(pila_t *pila, void (*destructor)(void *))
{
	if (!pila)
		return;

	lista_destruir_todo(pila->lista, destructor);
	free(pila);
}