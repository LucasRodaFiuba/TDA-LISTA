#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *primer_nodo;
	nodo_t *ultimo_nodo;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *corriente;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

bool lista_esta_vacia(lista_t *lista)
{
	return !lista || lista->cantidad == 0;
}

bool lista_vacia(lista_t *lista)
{
	return lista_esta_vacia(lista);
}

size_t lista_cantidad(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}

bool lista_insertar(lista_t *lista, void *dato, size_t posicion)
{
	if (!lista)
		return false;

	// Si la posición buscada es mayor a la cantidad actual de elementos, es inválida.
	if (posicion > lista->cantidad)
		return false;

	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;

	nuevo->elemento = dato;
	nuevo->siguiente = NULL;

	// Caso 1: Insertar al inicio (posicion 0)
	if (posicion == 0) {
		nuevo->siguiente = lista->primer_nodo;
		lista->primer_nodo = nuevo;
		if (lista->cantidad == 0) {
			lista->ultimo_nodo = nuevo;
		}
		lista->cantidad++;
		return true;
	}

	// Caso 2: Insertar justo al final (posicion == cantidad)
	if (posicion == lista->cantidad) {
		lista->ultimo_nodo->siguiente = nuevo;
		lista->ultimo_nodo = nuevo;
		lista->cantidad++;
		return true;
	}

	// Caso 3: Insertar en el medio (0 < posicion < cantidad)
	nodo_t *actual = lista->primer_nodo;
	for (size_t i = 0; i < posicion - 1; i++) {
		actual = actual->siguiente;
	}

	nuevo->siguiente = actual->siguiente;
	actual->siguiente = nuevo;
	lista->cantidad++;

	return true;
}

void *lista_eliminar(lista_t *lista, size_t posicion)
{
	if (lista_esta_vacia(lista) || posicion >= lista->cantidad)
		return NULL;

	nodo_t *a_eliminar = NULL;

	if (posicion == 0) {
		a_eliminar = lista->primer_nodo;
		lista->primer_nodo = a_eliminar->siguiente;
		if (lista->cantidad == 1) {
			lista->ultimo_nodo = NULL;
		}
	} else {
		nodo_t *actual = lista->primer_nodo;
		for (size_t i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}

		a_eliminar = actual->siguiente;
		actual->siguiente = a_eliminar->siguiente;

		if (posicion == lista->cantidad - 1) {
			lista->ultimo_nodo = actual;
		}
	}

	void *dato = a_eliminar->elemento;
	free(a_eliminar);
	lista->cantidad--;

	return dato;
}

void *lista_obtener(lista_t *lista, size_t posicion)
{
	if (lista_esta_vacia(lista) || posicion >= lista->cantidad)
		return NULL;

	if (posicion == lista->cantidad - 1) {
		return lista->ultimo_nodo->elemento;
	}

	nodo_t *actual = lista->primer_nodo;
	for (size_t i = 0; i < posicion; i++) {
		actual = actual->siguiente;
	}

	return actual->elemento;
}

void *lista_reemplazar(lista_t *lista, void *dato, size_t posicion)
{
	if (lista_esta_vacia(lista) || posicion >= lista->cantidad)
		return NULL;

	nodo_t *actual = lista->primer_nodo;
	for (size_t i = 0; i < posicion; i++) {
		actual = actual->siguiente;
	}

	void *anterior = actual->elemento;
	actual->elemento = dato;
	return anterior;
}

int lista_buscar(lista_t *lista, void *buscado,
		 int (*comparador)(void *, void *), void **elemento_encontrado)
{
	if (lista_esta_vacia(lista) || !comparador) {
		if (elemento_encontrado) {
			*elemento_encontrado = NULL;
		}
		return -1;
	}

	nodo_t *actual = lista->primer_nodo;
	int pos = 0;

	while (actual) {
		if (comparador(actual->elemento, buscado) == 0) {
			if (elemento_encontrado) {
				*elemento_encontrado = actual->elemento;
			}
			return pos;
		}
		actual = actual->siguiente;
		pos++;
	}

	if (elemento_encontrado) {
		*elemento_encontrado = NULL;
	}

	return -1;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	nodo_t *actual = lista->primer_nodo;
	while (actual) {
		nodo_t *siguiente = actual->siguiente;
		free(actual);
		actual = siguiente;
	}

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (!lista)
		return;

	nodo_t *actual = lista->primer_nodo;
	while (actual) {
		nodo_t *siguiente = actual->siguiente;
		if (destructor) {
			destructor(actual->elemento);
		}
		free(actual);
		actual = siguiente;
	}

	free(lista);
}

/* ---------------------------------------------------------------------
 *  ITERADORES
 * --------------------------------------------------------------------- */

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (lista_esta_vacia(lista) || !f)
		return 0;

	size_t cont = 0;
	nodo_t *actual = lista->primer_nodo;

	while (actual) {
		cont++;
		if (!f(actual->elemento, extra)) {
			break;
		}
		actual = actual->siguiente;
	}

	return cont;
}

size_t lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *extra)
{
	return lista_con_cada_elemento(lista, f, extra);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	lista_iterador_t *it = malloc(sizeof(lista_iterador_t));
	if (!it)
		return NULL;

	it->corriente = lista->primer_nodo;
	return it;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	return it && it->corriente != NULL;
}

bool lista_iterador_se_puede_iterar(lista_iterador_t *it)
{
	return lista_iterador_hay_mas_elementos(it);
}

void lista_iterador_avanzar(lista_iterador_t *it)
{
	if (it && it->corriente) {
		it->corriente = it->corriente->siguiente;
	}
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	lista_iterador_avanzar(it);
}

void *lista_iterador_actual(lista_iterador_t *it)
{
	if (!it || !it->corriente)
		return NULL;
	return it->corriente->elemento;
}

void *lista_iterador_obtener_elemento(lista_iterador_t *it)
{
	return lista_iterador_actual(it);
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	free(it);
}