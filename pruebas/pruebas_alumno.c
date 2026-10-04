#include "pa2m.h"
#include "../src/lista.h"
#include "../src/pila.h"
#include "../src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------
 *  FUNCIONES AUXILIARES Y DESTRUCTORAS
 * --------------------------------------------------------------------- */

int comparador_enteros(void *a, void *b)
{
	if (!a || !b)
		return -1;
	int int_a = *(int *)a;
	int int_b = *(int *)b;
	return int_a - int_b;
}

bool contar_elementos_iterador_interno(void *elemento, void *extra)
{
	size_t *contador = (size_t *)extra;
	(*contador)++;
	return true;
}

bool cortar_en_segundo_elemento(void *elemento, void *extra)
{
	size_t *contador = (size_t *)extra;
	(*contador)++;
	return (*contador < 2);
}

void destructor_dummy(void *elemento)
{
	int *flag = (int *)elemento;
	*flag = 1;
}

/* ---------------------------------------------------------------------
 *  PRUEBAS DE TDA LISTA - UNITARIAS
 * --------------------------------------------------------------------- */

void dada_una_lista_nueva_sus_propiedades_son_correctas()
{
	lista_t *lista = lista_crear();

	pa2m_afirmar(lista != NULL, "Se puede crear una lista correctamente.");
	pa2m_afirmar(lista_esta_vacia(lista) == true,
		     "Una lista nueva está vacía.");
	pa2m_afirmar(lista_cantidad(lista) == 0,
		     "Una lista nueva tiene cantidad 0.");

	lista_destruir(lista);
}

void dada_una_lista_nula_las_operaciones_no_falla_y_devuelven_valores_por_defecto()
{
	pa2m_afirmar(lista_esta_vacia(NULL) == true,
		     "lista_esta_vacia(NULL) devuelve true.");
	pa2m_afirmar(lista_cantidad(NULL) == 0,
		     "lista_cantidad(NULL) devuelve 0.");
	pa2m_afirmar(lista_obtener(NULL, 0) == NULL,
		     "lista_obtener(NULL, 0) devuelve NULL.");
	pa2m_afirmar(lista_eliminar(NULL, 0) == NULL,
		     "lista_eliminar(NULL, 0) devuelve NULL.");
	pa2m_afirmar(lista_reemplazar(NULL, NULL, 0) == NULL,
		     "lista_reemplazar(NULL, ...) devuelve NULL.");

	void *encontrado = (void *)0x123;
	int res_busqueda = lista_buscar(NULL, &res_busqueda, comparador_enteros,
					&encontrado);
	pa2m_afirmar(res_busqueda == -1,
		     "lista_buscar(NULL, ...) devuelve -1.");
	pa2m_afirmar(encontrado == NULL,
		     "lista_buscar(NULL, ...) deja NULL en encontrado.");
}

void insertar_en_posicion_invalidas_falla()
{
	lista_t *lista = lista_crear();
	int a = 10, b = 20, elem_invalido = 99;

	pa2m_afirmar(lista_insertar(lista, &elem_invalido, 1) == false,
		     "No se puede insertar en pos 1 de una lista vacía.");
	pa2m_afirmar(lista_cantidad(lista) == 0,
		     "La lista sigue teniendo 0 elementos.");

	lista_insertar(lista, &a, 0);
	lista_insertar(lista, &b, 1);

	pa2m_afirmar(
		lista_insertar(lista, &elem_invalido, 3) == false,
		"No se puede insertar en pos 3 de una lista con 2 elementos.");
	pa2m_afirmar(
		lista_insertar(lista, &elem_invalido, 100) == false,
		"No se puede insertar en pos 100 de una lista con 2 elementos.");
	pa2m_afirmar(lista_cantidad(lista) == 2, "La cantidad no varió.");

	lista_destruir(lista);
}

void insertar_y_obtener_elementos_en_orden_correcto()
{
	lista_t *lista = lista_crear();
	int a = 10, b = 20, c = 30;

	pa2m_afirmar(lista_insertar(lista, &a, 0) == true,
		     "Insertar primer elemento (10) en pos 0.");
	pa2m_afirmar(lista_insertar(lista, &b, 1) == true,
		     "Insertar segundo elemento (20) en pos 1.");
	pa2m_afirmar(lista_insertar(lista, &c, 2) == true,
		     "Insertar tercer elemento (30) en pos 2.");
	pa2m_afirmar(lista_cantidad(lista) == 3, "La cantidad es 3.");

	pa2m_afirmar(lista_obtener(lista, 0) == &a, "Elemento en pos 0 es 10.");
	pa2m_afirmar(lista_obtener(lista, 1) == &b, "Elemento en pos 1 es 20.");
	pa2m_afirmar(lista_obtener(lista, 2) == &c, "Elemento en pos 2 es 30.");
	pa2m_afirmar(lista_obtener(lista, 99) == NULL,
		     "Obtener pos fuera de rango devuelve NULL.");

	lista_destruir(lista);
}

void reemplazar_elementos_funciona_correctamente()
{
	lista_t *lista = lista_crear();
	int a = 10, b = 20, c = 30, nuevo = 99;

	lista_insertar(lista, &a, 0);
	lista_insertar(lista, &b, 1);
	lista_insertar(lista, &c, 2);

	pa2m_afirmar(lista_reemplazar(lista, &nuevo, 1) == &b,
		     "Reemplazar pos 1 devuelve el elemento anterior.");
	pa2m_afirmar(lista_obtener(lista, 1) == &nuevo,
		     "lista_obtener 1 devuelve el nuevo elemento.");
	pa2m_afirmar(lista_reemplazar(lista, &nuevo, 99) == NULL,
		     "Reemplazar fuera de rango devuelve NULL.");

	lista_destruir(lista);
}

void eliminar_elementos_mantiene_consistencia()
{
	lista_t *lista = lista_crear();
	int a = 10, b = 20, c = 30;

	lista_insertar(lista, &a, 0);
	lista_insertar(lista, &b, 1);
	lista_insertar(lista, &c, 2);

	pa2m_afirmar(lista_eliminar(lista, 1) == &b,
		     "Eliminar pos 1 devuelve 20.");
	pa2m_afirmar(lista_cantidad(lista) == 2, "La cantidad es 2.");
	pa2m_afirmar(lista_eliminar(lista, 0) == &a,
		     "Eliminar pos 0 devuelve 10.");
	pa2m_afirmar(lista_eliminar(lista, 0) == &c,
		     "Eliminar pos 0 restante devuelve 30.");
	pa2m_afirmar(lista_esta_vacia(lista) == true, "La lista quedó vacía.");
	pa2m_afirmar(lista_eliminar(lista, 0) == NULL,
		     "Eliminar en lista vacía devuelve NULL.");

	lista_destruir(lista);
}

void buscar_elementos_en_lista()
{
	lista_t *lista = lista_crear();
	int a = 5, b = 15, c = 25;

	lista_insertar(lista, &a, 0);
	lista_insertar(lista, &b, 1);
	lista_insertar(lista, &c, 2);

	void *encontrado = NULL;
	int pos = lista_buscar(lista, &b, comparador_enteros, &encontrado);
	pa2m_afirmar(pos == 1, "Buscar elemento existente devuelve pos 1.");
	pa2m_afirmar(encontrado == &b, "Encontrado apunta al elemento.");

	int no_existe = 999;
	encontrado = (void *)0x123;
	pa2m_afirmar(lista_buscar(lista, &no_existe, comparador_enteros,
				  &encontrado) == -1,
		     "Buscar inexistente devuelve -1.");
	pa2m_afirmar(encontrado == NULL,
		     "Deja NULL en encontrado si no existe.");

	lista_destruir(lista);
}

void iteradores_lista_internos_y_externos()
{
	lista_t *lista = lista_crear();
	int a = 5, b = 15, c = 25;

	lista_insertar(lista, &a, 0);
	lista_insertar(lista, &b, 1);
	lista_insertar(lista, &c, 2);

	size_t recorridos = 0;
	size_t aplicados = lista_iterar(
		lista, contar_elementos_iterador_interno, &recorridos);
	pa2m_afirmar(aplicados == 3 && recorridos == 3,
		     "Iterador interno recorre todos los elementos.");

	size_t parciales = 0;
	lista_iterar(lista, cortar_en_segundo_elemento, &parciales);
	pa2m_afirmar(parciales == 2,
		     "Iterador interno corta si la función retorna false.");

	lista_iterador_t *it = lista_iterador_crear(lista);
	pa2m_afirmar(it != NULL, "Crear iterador externo exitoso.");
	pa2m_afirmar(lista_iterador_se_puede_iterar(it) == true,
		     "Se puede iterar al inicio.");
	pa2m_afirmar(lista_iterador_obtener_elemento(it) == &a,
		     "Elemento actual es 5.");

	lista_iterador_siguiente(it);
	pa2m_afirmar(lista_iterador_obtener_elemento(it) == &b,
		     "Avanzar iterador muestra 15.");

	lista_iterador_siguiente(it);
	lista_iterador_siguiente(it);
	pa2m_afirmar(lista_iterador_se_puede_iterar(it) == false,
		     "Final de iteración alcanzado.");

	lista_iterador_destruir(it);
	lista_destruir(lista);
}

void lista_destruir_todo_aplica_destructor()
{
	lista_t *lista = lista_crear();
	int flag1 = 0, flag2 = 0;

	lista_insertar(lista, &flag1, 0);
	lista_insertar(lista, &flag2, 1);

	lista_destruir_todo(lista, destructor_dummy);
	pa2m_afirmar(flag1 == 1 && flag2 == 1,
		     "lista_destruir_todo aplica destructor a cada elemento.");
}

void pruebas_lista_agrupadas()
{
	pa2m_nuevo_grupo("Pruebas de Lista - Creación y Nulos");
	dada_una_lista_nueva_sus_propiedades_son_correctas();
	dada_una_lista_nula_las_operaciones_no_falla_y_devuelven_valores_por_defecto();

	pa2m_nuevo_grupo("Pruebas de Lista - Inserción y Selección");
	insertar_en_posicion_invalidas_falla();
	insertar_y_obtener_elementos_en_orden_correcto();

	pa2m_nuevo_grupo("Pruebas de Lista - Reemplazo y Eliminación");
	reemplazar_elementos_funciona_correctamente();
	eliminar_elementos_mantiene_consistencia();

	pa2m_nuevo_grupo("Pruebas de Lista - Búsqueda e Iteradores");
	buscar_elementos_en_lista();
	iteradores_lista_internos_y_externos();

	pa2m_nuevo_grupo("Pruebas de Lista - Destrucción");
	lista_destruir_todo_aplica_destructor();
}

/* ---------------------------------------------------------------------
 *  PRUEBAS DE TDA PILA - UNITARIAS
 * --------------------------------------------------------------------- */

void dada_una_pila_nueva_sus_propiedades_son_correctas()
{
	pila_t *pila = pila_crear();

	pa2m_afirmar(pila != NULL, "Se crea la pila correctamente.");
	pa2m_afirmar(pila_esta_vacia(pila) == true,
		     "La pila creada está vacía.");
	pa2m_afirmar(pila_cantidad(pila) == 0, "Pila vacía tiene cantidad 0.");
	pa2m_afirmar(pila_tope(pila) == NULL,
		     "pila_tope en vacía devuelve NULL.");
	pa2m_afirmar(pila_desapilar(pila) == NULL,
		     "pila_desapilar en vacía devuelve NULL.");

	pila_destruir(pila);
}

void pila_apilar_y_desapilar_respeta_orden_lifo()
{
	pila_t *pila = pila_crear();
	int a = 1, b = 2, c = 3;

	pa2m_afirmar(pila_apilar(pila, &a) == true, "Apilar 1.");
	pa2m_afirmar(pila_tope(pila) == &a, "Tope es 1.");
	pa2m_afirmar(pila_apilar(pila, &b) == true, "Apilar 2.");
	pa2m_afirmar(pila_apilar(pila, &c) == true, "Apilar 3.");
	pa2m_afirmar(pila_cantidad(pila) == 3, "Cantidad es 3.");
	pa2m_afirmar(pila_tope(pila) == &c, "Tope es 3 (LIFO).");

	pa2m_afirmar(pila_desapilar(pila) == &c, "Desapilar devuelve 3.");
	pa2m_afirmar(pila_desapilar(pila) == &b, "Desapilar devuelve 2.");
	pa2m_afirmar(pila_desapilar(pila) == &a, "Desapilar devuelve 1.");
	pa2m_afirmar(pila_esta_vacia(pila) == true, "Pila queda vacía.");

	pila_destruir(pila);
}

void pila_volumen()
{
	pila_t *pila = pila_crear();
	size_t cantidad = 1000;
	bool apilado_ok = true;

	for (size_t i = 0; i < cantidad; i++) {
		apilado_ok = pila_apilar(pila, (void *)i);
	}
	pa2m_afirmar(apilado_ok && pila_cantidad(pila) == cantidad,
		     "Se apilaron 1000 elementos.");

	bool desapilado_ok = true;
	for (size_t i = cantidad; i > 0; i--) {
		void *desapilado = pila_desapilar(pila);
		if ((size_t)desapilado != (i - 1)) {
			desapilado_ok = false;
		}
	}
	pa2m_afirmar(desapilado_ok && pila_esta_vacia(pila),
		     "Se desapilaron 1000 elementos en orden LIFO.");

	pila_destruir(pila);
}

void pruebas_pila_agrupadas()
{
	pa2m_nuevo_grupo("Pruebas de Pila - Operaciones Base");
	dada_una_pila_nueva_sus_propiedades_son_correctas();
	pila_apilar_y_desapilar_respeta_orden_lifo();

	pa2m_nuevo_grupo("Pruebas de Pila - Volumen");
	pila_volumen();
}

/* ---------------------------------------------------------------------
 *  PRUEBAS DE TDA COLA - UNITARIAS
 * --------------------------------------------------------------------- */

void dada_una_cola_nueva_sus_propiedades_son_correctas()
{
	cola_t *cola = cola_crear();

	pa2m_afirmar(cola != NULL, "Se crea la cola correctamente.");
	pa2m_afirmar(cola_esta_vacia(cola) == true,
		     "La cola creada está vacía.");
	pa2m_afirmar(cola_cantidad(cola) == 0, "Cola vacía tiene cantidad 0.");
	pa2m_afirmar(cola_frente(cola) == NULL,
		     "cola_frente en vacía devuelve NULL.");
	pa2m_afirmar(cola_desencolar(cola) == NULL,
		     "cola_desencolar en vacía devuelve NULL.");

	cola_destruir(cola);
}

void cola_encolar_y_desencolar_respeta_orden_fifo()
{
	cola_t *cola = cola_crear();
	int a = 100, b = 200, c = 300;

	pa2m_afirmar(cola_encolar(cola, &a) == true, "Encolar 100.");
	pa2m_afirmar(cola_frente(cola) == &a, "Frente es 100.");
	pa2m_afirmar(cola_encolar(cola, &b) == true, "Encolar 200.");
	pa2m_afirmar(cola_encolar(cola, &c) == true, "Encolar 300.");
	pa2m_afirmar(cola_cantidad(cola) == 3, "Cantidad es 3.");
	pa2m_afirmar(cola_frente(cola) == &a,
		     "Frente sigue siendo 100 (FIFO).");

	pa2m_afirmar(cola_desencolar(cola) == &a, "Desencolar devuelve 100.");
	pa2m_afirmar(cola_frente(cola) == &b, "Nuevo frente es 200.");
	pa2m_afirmar(cola_desencolar(cola) == &b, "Desencolar devuelve 200.");
	pa2m_afirmar(cola_desencolar(cola) == &c, "Desencolar devuelve 300.");
	pa2m_afirmar(cola_esta_vacia(cola) == true, "Cola queda vacía.");

	cola_destruir(cola);
}

void cola_volumen()
{
	cola_t *cola = cola_crear();
	size_t cantidad = 1000;
	bool encolado_ok = true;

	for (size_t i = 0; i < cantidad; i++) {
		encolado_ok = cola_encolar(cola, (void *)i);
	}
	pa2m_afirmar(encolado_ok && cola_cantidad(cola) == cantidad,
		     "Se encolaron 1000 elementos.");

	bool desencolado_ok = true;
	for (size_t i = 0; i < cantidad; i++) {
		void *desencolado = cola_desencolar(cola);
		if ((size_t)desencolado != i) {
			desencolado_ok = false;
		}
	}
	pa2m_afirmar(desencolado_ok && cola_esta_vacia(cola),
		     "Se desencolaron 1000 elementos en orden FIFO.");

	cola_destruir(cola);
}

void pruebas_cola_agrupadas()
{
	pa2m_nuevo_grupo("Pruebas de Cola - Operaciones Base");
	dada_una_cola_nueva_sus_propiedades_son_correctas();
	cola_encolar_y_desencolar_respeta_orden_fifo();

	pa2m_nuevo_grupo("Pruebas de Cola - Volumen");
	cola_volumen();
}

/* ---------------------------------------------------------------------
 *  MAIN DE PRUEBAS
 * --------------------------------------------------------------------- */

int main()
{
	pa2m_nuevo_grupo("================ PRUEBAS TDA LISTA ================");
	pruebas_lista_agrupadas();

	pa2m_nuevo_grupo("================ PRUEBAS TDA PILA ================");
	pruebas_pila_agrupadas();

	pa2m_nuevo_grupo("================ PRUEBAS TDA COLA ================");
	pruebas_cola_agrupadas();

	return pa2m_mostrar_reporte();
}