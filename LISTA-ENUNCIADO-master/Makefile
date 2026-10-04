CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes

SRC = src/*.c
PRUEBAS_SRC = pruebas/pruebas_alumno.c

# Compila el ejecutable principal
build:
	$(CC) $(CFLAGS) main.c $(SRC) -o tp

# Compila y ejecuta las pruebas del alumno
test:
	$(CC) $(CFLAGS) $(PRUEBAS_SRC) $(SRC) -o pruebas_alumno
	./pruebas_alumno

# Corre las pruebas del alumno con Valgrind
valgrind:
	$(CC) $(CFLAGS) $(PRUEBAS_SRC) $(SRC) -o pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

# Limpia los ejecutables generados
clean:
	rm -f *.o tp pruebas_alumno testing