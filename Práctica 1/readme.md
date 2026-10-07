# Práctica 1: Procesos

Estudio del ciclo de vida de los procesos en Unix mediante llamadas al sistema POSIX en C: creación, concurrencia, memoria, comunicación entre procesos, procesos huérfanos y zombies, y ejecución de programas. La práctica termina con la implementación de un **mini-shell** propio.

Las respuestas completas a las preguntas de la práctica están en [RESPUESTAS.md](./RESPUESTAS.md).

## Contenido

### 1. Creación de procesos
Creación de procesos con `fork()` y sincronización con `wait()`. Se lanzan 50 procesos hijos y se comprueba que el orden en que se ejecutan lo decide el planificador del sistema operativo, no el orden de creación, y que el padre no siempre se ejecuta primero.

### 2. Espacio de memoria y concurrencia
Tras un `fork()`, padre e hijo tienen copias independientes de sus variables. Se demuestra que un proceso no puede ver ni modificar la memoria del otro, y se analiza cómo se ejecutarían ambos procesos en un procesador con una sola CPU mediante cambios de contexto.

### 3. Comunicación entre procesos con pipes
Envío de datos del padre al hijo con `pipe()`, `write()` y `read()`. Se comprueba que el dato no se pierde aunque el hijo lea más tarde, porque queda guardado en el buffer del kernel.

### 4. Procesos huérfanos y zombies
- **Huérfanos:** cuando el padre termina antes que el hijo, el sistema asigna al hijo un nuevo padre. Se imprime su PID para identificarlo.
- **Zombies:** cuando el hijo termina antes de que el padre haga `wait()`, queda en estado zombie. Se comprueba que no responde a `SIGKILL` y que solo desaparece cuando el padre recoge su estado con `wait()`.

### 5. Mini-shell con `execvp()`
Intérprete de comandos que repite este ciclo:

1. Lee un comando del usuario
2. Crea un proceso hijo con `fork()`
3. Reemplaza el hijo por el programa pedido con `execvp()`
4. Espera a que termine con `wait()`

Funciona con comandos como `ls`, `cat`, `echo` o `ps`.

## Ficheros

| Fichero | Descripción |
|---------|-------------|
| `exemple1.c` | Creación de 50 procesos hijos |
| `exemple2.c` | Memoria independiente entre padre e hijo |
| `exemple3.c` | Comunicación con pipes |
| `orphan.c` | Proceso huérfano |
| `zombie.c` | Proceso zombie |
| `mini-shell.c` | Intérprete de comandos |

## Compilación y ejecución

```bash
gcc mini-shell.c -o mini-shell
./mini-shell
```

Cambia el nombre del fichero para compilar cualquiera de los otros ejemplos.

> Probado en macOS. Algunos resultados (como el PID del nuevo padre de un proceso huérfano) pueden variar en Linux.

## Conceptos aprendidos

`fork()` · `wait()` · `exit()` · `getpid()` · `getppid()` · `pipe()` · `read()` · `write()` · `kill()` · `execvp()` · planificador del SO · cambio de contexto · descriptores de fichero · procesos huérfanos · procesos zombies · señales
