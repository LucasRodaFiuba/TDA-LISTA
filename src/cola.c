#include "cola.h"
#include "lista.h"
#include <stdlib.h>

struct cola {
	lista_t *lista;
};

cola_t *cola_crear()
{
	cola_t *cola = malloc(sizeof(cola_t));
	if (!cola)
		return NULL;

	cola->lista = lista_crear();
	if (!cola->lista) {
		free(cola);
		return NULL;
	}

	return cola;
}

bool cola_esta_vacia(cola_t *cola)
{
	return !cola || lista_cantidad(cola->lista) == 0;
}

bool cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola)
		return false;

	return lista_insertar(cola->lista, elemento,
			      lista_cantidad(cola->lista));
}

void *cola_desencolar(cola_t *cola)
{
	if (cola_esta_vacia(cola))
		return NULL;

	return lista_eliminar(cola->lista, 0);
}

void *cola_frente(cola_t *cola)
{
	if (cola_esta_vacia(cola))
		return NULL;

	return lista_obtener(cola->lista, 0);
}

size_t cola_cantidad(cola_t *cola)
{
	if (!cola)
		return 0;

	return lista_cantidad(cola->lista);
}

void cola_destruir(cola_t *cola)
{
	if (!cola)
		return;

	lista_destruir(cola->lista);
	free(cola);
}

void cola_destruir_todo(cola_t *cola, void (*destructor)(void *))
{
	if (!cola)
		return;

	lista_destruir_todo(cola->lista, destructor);
	free(cola);
}