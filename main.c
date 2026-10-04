#include "src/lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SUMA '+'
#define RESTA '-'
#define MULTIPLICACION '*'
#define DIVISION '/'

#define MIN_ARGUMENTOS 3
#define ERROR_RETORNO -1
#define EXITO_RETORNO 0

typedef struct {
	int *valores;
	size_t cantidad;
} vector_t;

static char *mi_strdup(const char *s)
{
	if (!s)
		return NULL;
	size_t len = strlen(s);
	char *p = malloc(len + 1);
	if (!p)
		return NULL;
	memcpy(p, s, len + 1);
	return p;
}

static vector_t *parsear_vector(const char *cadena)
{
	vector_t *vec = malloc(sizeof(vector_t));
	if (!vec)
		return NULL;

	vec->valores = NULL;
	vec->cantidad = 0;

	char *copia = mi_strdup(cadena);
	if (!copia) {
		free(vec);
		return NULL;
	}

	char *token = strtok(copia, ",");
	while (token) {
		int *tmp = realloc(vec->valores,
				   sizeof(int) * (vec->cantidad + 1));
		if (!tmp) {
			free(vec->valores);
			free(copia);
			free(vec);
			return NULL;
		}
		vec->valores = tmp;
		vec->valores[vec->cantidad] = atoi(token);
		vec->cantidad++;
		token = strtok(NULL, ",");
	}

	free(copia);
	return vec;
}

static void destruir_vector(void *v)
{
	if (!v)
		return;
	vector_t *vec = (vector_t *)v;
	free(vec->valores);
	free(vec);
}

int main(int argc, char *argv[])
{
	if (argc < MIN_ARGUMENTOS) {
		printf("ERROR\n");
		return EXITO_RETORNO;
	}

	char op = argv[1][0];
	if (op != SUMA && op != RESTA && op != MULTIPLICACION &&
	    op != DIVISION) {
		printf("ERROR\n");
		return EXITO_RETORNO;
	}

	lista_t *lista_vectores = lista_crear();
	if (!lista_vectores) {
		printf("ERROR\n");
		return EXITO_RETORNO;
	}

	size_t max_len = 0;
	for (int i = 2; i < argc; i++) {
		vector_t *v = parsear_vector(argv[i]);
		if (!v) {
			lista_destruir_todo(lista_vectores, destruir_vector);
			printf("ERROR\n");
			return EXITO_RETORNO;
		}
		if (v->cantidad > max_len)
			max_len = v->cantidad;
		lista_insertar(lista_vectores, v,
			       lista_cantidad(lista_vectores));
	}

	size_t num_vectores = lista_cantidad(lista_vectores);

	for (size_t pos = 0; pos < max_len; pos++) {
		bool error_pos = false;

		for (size_t i = 0; i < num_vectores; i++) {
			vector_t *v =
				(vector_t *)lista_obtener(lista_vectores, i);
			if (pos >= v->cantidad) {
				error_pos = true;
				break;
			}
		}

		if (error_pos) {
			printf("E");
		} else {
			vector_t *v0 =
				(vector_t *)lista_obtener(lista_vectores, 0);
			int res = v0->valores[pos];

			for (size_t i = 1; i < num_vectores; i++) {
				vector_t *v = (vector_t *)lista_obtener(
					lista_vectores, i);
				int val = v->valores[pos];

				if (op == SUMA) {
					res += val;
				} else if (op == RESTA) {
					res -= val;
				} else if (op == MULTIPLICACION) {
					res *= val;
				} else if (op == DIVISION) {
					if (val == 0) {
						error_pos = true;
						break;
					}
					res /= val;
				}
			}

			if (error_pos) {
				printf("E");
			} else {
				printf("%d", res);
			}
		}

		if (pos < max_len - 1)
			printf(",");
	}

	printf("\n");

	lista_destruir_todo(lista_vectores, destruir_vector);
	return EXITO_RETORNO;
}