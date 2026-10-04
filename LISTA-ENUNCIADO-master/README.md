<div align="right">
    <img width="32px" src="img/algo2.svg">
</div>

# TP

## Información del estudiante

* Nombre y Apellido: Lucas Roda
* Padrón: 114402
* Mail: lroda@fi.uba.ar
* Username de GitHub: LucasRodaFiuba

## Índice
* [1. Instrucciones](#1-Instrucciones)
  * [1.1. Compilar el proyecto](#11-Compilar-el-proyecto)
  * [1.2. Ejecutar las pruebas](#12-Ejecutar-las-pruebas)
  * [1.3. Ejecutar el programa con Valgrind](#13-Ejecutar-el-programa-con-Valgrind)
* [2. Funcionamiento](#2-Funcionamiento)
* [3. Estructura](#3-Estructura)
  * [3.1. Diagrama de memoria](#31-Diagrama-de-memoria)
  * [3.2. Análisis de complejidades](#32-Análisis-de-complejidades)
* [4. Decisiones de diseño y/o complejidades de implementación](#4-Decisiones-de-diseño-yo-complejidades-de-implementación)
* [5. Respuestas a las preguntas teóricas](#5-Respuestas-a-las-preguntas-teóricas)

## 1. Instrucciones

> [!NOTE]
> Las siguientes instrucciones de compilación y prueba mediante el `Makefile` están pensadas para ejecutarse en un entorno basado en Linux (como WSL o Ubuntu), donde se encuentran instaladas las herramientas `build-essential` (`gcc`, `make`) y `valgrind`.

### 1.1. Compilar el proyecto

```bash
make build
```
### 1.2. Ejecutar las pruebas

```bash
make test
```

### 1.3. Ejecutar el programa con Valgrind
Para verificar la ausencia de fugas de memoria y accesos inválidos:
```bash
make valgrind
```

## 2. Funcionamiento
El programa principal realiza operaciones aritméticas vectoriales elemento por elemento recibidas a través de la línea de comandos. Recibe un operador en el primer argumento (+, -, *, /) seguido de múltiples cadenas de enteros separados por comas que representan los vectores.
Al iniciarse, el programa parsea cada cadena, almacena las estructuras vectoriales dentro de un TDA Lista y resuelve la operación posición por posición. Si un vector resulta ser más corto que la longitud máxima evaluada o si se detecta una división por cero, el programa imprime el carácter E en esa posición concreta para indicar el error. La secuencia resultante se imprime en pantalla delimitada por comas.

<div align="center">
  <img src="img/diagrama_flujo_programa.svg" width="70%">
  <p>Diagrama de flujo del programa explicado con más detalle.</p>
</div>

## 3. Estructura
- TDA Lista: Es una lista simplemente enlazada representada por una estructura principal lista_t que mantiene un contador cantidad de elementos y dos referencias clave: primer_nodo y ultimo_nodo. Cada nodo (nodo_t) contiene un puntero genérico void * al dato almacenado y un puntero siguiente que apunta al nodo contiguo.
- TDA Pila: Se implementó reutilizando internamente la estructura lista_t (wrapper). Al apilar y desapilar únicamente por la cabeza/inicio de la lista, se logra la disciplina de acceso LIFO (Last In, First Out).
- TDA Cola: Se implementó reutilizando internamente la estructura lista_t (wrapper). Al encolar al final (usando el puntero ultimo_nodo) y desencolar al inicio, se garantiza la disciplina FIFO (First In, First Out).

### 3.1 Diagrama de memoria
<div align="center">
  <img src="img\diagrama_memoria.png" width="70%">
  <p>Diagrama de memoria de la estructura.</p>
</div>

### 3.2. Análisis de complejidades

| Función | Complejidad | Justificación |
| :--- | :---: | :--- |
| `lista_crear` | $O(1)$ | Asigna dinámicamente el bloque de memoria de la cabecera mediante `calloc`[cite: 3]. |
| `lista_insertar` (extremos) | $O(1)$ | Al insertar en posición 0 o al final, utiliza los punteros directos `primer_nodo` y `ultimo_nodo` sin recorrer la lista[cite: 3]. |
| `lista_insertar` (medio) | $O(n)$ | En el peor caso debe avanzar nodo a nodo hasta la posición $n-1$[cite: 3]. |
| `lista_eliminar` (inicio) | $O(1)$ | Desengancha y reconecta directamente el puntero `primer_nodo`[cite: 3]. |
| `lista_eliminar` (medio/fin) | $O(n)$ | Requiere recorrer secuencialmente hasta el nodo anterior al que se desea eliminar[cite: 3]. |
| `lista_obtener` | $O(n)$ | Avanza elemento por elemento, a excepción de la última posición que accede vía `ultimo_nodo`[cite: 3]. |
| `lista_buscar` | $O(n)$ | Aplica la función comparadora recorriendo secuencialmente todos los nodos hasta hallar coincidencia o llegar al final[cite: 3]. |
| `pila_apilar` / `pila_desapilar` | $O(1)$ | Llama a la inserción y eliminación en la posición 0 de la lista enlazada[cite: 3, 4, 9]. |
| `cola_encolar` / `cola_desencolar` | $O(1)$ | Reutiliza la inserción al final y la eliminación al inicio de la lista enlazada[cite: 3, 4, 9]. |

## 4. Decisiones de diseño y/o complejidades de implementación
La mayor complejidad técnica residió en lograr que las primitivas de Pila y Cola operen en tiempo constante $O(1)$ reutilizando la lista simplemente enlazada. Para lograr que cola_encolar cumpla con $O(1)$, la lista mantiene explícitamente un puntero ultimo_nodo que se actualiza en cada inserción o eliminación relevante.
Por otro lado, para garantizar compatibilidad total con C99 estricto (-std=c99 -pedantic), se decidió no recurrir a funciones fuera del estándar como strdup y en su lugar se implementó la función auxiliar mi_strdup para el duplicado de cadenas al parsear vectores en main.c.

## 5. Respuestas a las preguntas teóricas

### 5.1 Explicar qué es una lista, lista enlazada y lista doblemente enlazada.
Una lista es un tipo de dato abstracto que esta compuesto por una agrupación de elementos, en la cual cada uno tiene un sucesor (exceptuando el ultimo elemento de la lista) y un predecesor (exceptuando el primer elemento de la lista).
Lista Simplemente Enlazada: Colección de nodos dispersos en memoria unidos mediante un enlace unidireccional siguiente.   
Ventajas: Altamente flexible; insertar o borrar en los extremos es $O(1)$ sin mover datos en memoria.   
Desventajas: Se pierde el acceso directo por índice ($O(n)$) y requiere overhead de memoria por guardar los punteros.   
Lista Doblemente Enlazada: Cada nodo contiene dos referencias: siguiente y anterior.
Diferencia interna: Permite recorridos en ambos sentidos y borrar un nodo conocido en $O(1)$.
Desventaja: Ocupa más espacio en memoria por nodo (dos punteros) y requiere actualizar más referencias en cada operación.
### 5.2 Explicar qué es una lista circular y de qué maneras se puede implementar.
Una lista circular es aquella en la cual el último nodo no apunta a NULL, sino que su puntero siguiente vuelve a referenciar al primer nodo de la secuencia.Maneras de implementación:Simplemente enlazada circular: El último nodo referencia a la cabeza.Doblemente enlazada circular: El siguiente del último apunta al primero, y el anterior del primero apunta al último.Con puntero a la cola: Manteniendo únicamente un puntero al último nodo, se puede acceder tanto al final como al inicio (tail->siguiente) en $O(1)$.
### 5.3 Explicar la diferencia de funcionamiento entre cola y pila.
- Pila (LIFO - Last In, First Out): El último elemento ingresado es el primero en ser retirado. Todas las operaciones (apilar, desapilar, tope) se realizan sobre un único extremo denominado "tope".  
- Cola (FIFO - First In, First Out): El primer elemento ingresado es el primero en ser retirado. Sus operaciones trabajan en extremos opuestos: las inserciones se hacen por el "final" y las extracciones por el "frente".  
### 5.4 Explicar la diferencia entre un iterador interno y uno externo.
- Iterador Interno: La propia estructura gestiona el recorrido mediante una función lista_iterar que recibe una función de callback y un puntero de contexto. El usuario define la acción sobre cada elemento, y la iteración se detiene si la función retorna false.
- Iterador Externo: Es una estructura independiente (lista_iterador_t) que almacena el estado actual del recorrido (corriente). Le permite al usuario controlar cuándo avanzar (siguiente), verificar si restan elementos o pedir el dato actual según la lógica de su propio código.