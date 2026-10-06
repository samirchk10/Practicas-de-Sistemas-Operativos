# Practica 1 - Processos

### Ejercicio 1: Creación de procesos

### `pexample1.c`

```c
int main(int argc, char** argv) {
    int status;
    int pid;

    // Create a process (fork)
    if ((pid = fork()) == 0) {
        // Child process
        printf("[%s] Child process (PID=%d)\n", timestamp(), getpid());
        exit(0); // Terminate OK
    } else {
        // Father process
        printf("[%s] Father process (PID=%d)\n", timestamp(), getpid());
        wait(&status); // Wait for child to finish
    }

    return 0; // Return OK
}
```

**a) ¿Comparten el padre y el hijo el mismo código del programa? ¿Ejecutan el padre y el hijo las mismas líneas de código? ¿Cómo discriminamos qué partes del código ejecuta cada proceso?**

- Sí, comparten el mismo código: `fork()` crea una copia exacta del proceso padre. No ejecutarán las mismas líneas. Ambos continúan después del `fork()` y los distingue el valor de retorno de `fork()`. Ese valor es 0 para el hijo, mayor que 0 para el padre y menor que 0 si da error.

**b) ¿Qué función realiza `getpid()`? ¿Puede `getpid()` devolver el mismo resultado a dos procesos diferentes? ¿Puede devolver cero? Describe qué pruebas has realizado para comprobarlo.**

- Devuelve el identificador (PID) del proceso que la llama. Dos procesos activos a la vez nunca devolverán el mismo PID. El PID 0 está reservado para el kernel.
- Pruebas realizadas:
  - Ejecutar el programa varias veces para ver que los PIDs siempre son diferentes.
  - Para ver que el proceso con PID 0 está reservado para el kernel:

```bash
    top -l 1 -pid 0 -stats pid,command
```

```
    PID COMMAND
    0   kernel_task
```

**c) ¿Qué pasaría si el hijo llamara a `wait()`?**

- `wait()` hace que un proceso espere a que termine uno de sus hijos. Pero el proceso hijo no ha creado ningún hijo propio, así que no tiene a quién esperar. Por tanto, `wait()` no se bloquea: devuelve -1, indicando error, y la variable `errno` toma el valor `ECHILD` ("No child processes"). Podemos probarlo añadiendo estas líneas al código que ejecuta el proceso hijo:

```c
int r = wait(&status);
printf("[%s] Hijo: wait() devolvió %d\n", timestamp(), r);
perror("wait");
```

```
[2026:10:06 20:28:50:444139] Father process (PID=13936)
[2026:10:06 20:28:50:444367] Child process  (PID=13937)
wait: No child processes
```

**d) ¿Es cierto que, sabiendo que el padre crea al hijo con `fork()`, el padre siempre se ejecuta primero? ¿Cómo puedes comprobarlo?**

- No. Después del `fork()` existen dos procesos independientes (padre e hijo) y el planificador del SO decide cuál se ejecuta primero. Que el padre haya creado al hijo no le da prioridad. En un ordenador con varios núcleos, pueden ejecutarse a la vez. Para comprobarlo podemos:
  - Añadir `sleep(1)` en el padre justo antes del `printf()`. Así vemos que el hijo sí puede ejecutarse antes que el padre:

```
    Sistemas Operativos/Practicas/Práctica 1 ❯ ./exemple1
    [2026:10:06 21:36:16:670474] Child process  (PID=15518)
    [2026:10:06 21:36:17:670776] Father process (PID=15517)
```

**Código a desarrollar:**

```c
#define NUM_FILLS 50

int main(int argc, char** argv)
{
    int status;
    int pid;

    for (int i = 0; i < NUM_FILLS; i++)
    {
        if ((pid=fork())==0)
        {
            printf("[%s] Child process (PID=%d)\n", timestamp(), getpid());
            exit(0);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_FILLS; i++)
    {
        wait(&status);
    }

    printf("[%s] Father process (PID=%d): tots els fills han acabat\n", timestamp(), getpid());
    return 0;
}
```
Extender el código anterior para crear 50 procesos hijos. Cada proceso hijo tiene que imprimir su timestamp y terminar. El proceso padre deberá esperar la finalización de todos los procesos hijos y, solo entonces, terminar.

**e) ¿Qué se observa en los resultados al ejecutar varias veces el programa?**

- Se crean los 50 hijos y cada uno imprime su timestamp y su PID; al final el padre imprime un mensaje. Los PIDs cambian en cada ejecución, y los hijos no siempre aparecen en el orden en que se crearon:

```
    [2026:10:06 22:02:40:474150] Child process (PID=16329)
    [2026:10:06 22:02:40:474069] Child process (PID=16328)
```

**f) ¿Genera siempre los mismos resultados?**

- No, cambian los PIDs, los timestamps y el orden de los mensajes de los hijos. Solo se mantiene la cantidad de hijos que se crean y que el padre termina el último.

**g) ¿Se ejecutan los procesos siempre en el mismo orden? ¿Cómo lo has comprobado? ¿Por qué pasa esto?**

- No. Para comprobarlo ejecutamos el programa varias veces y comparamos los timestamps. Esto pasa porque el orden de ejecución lo decide el planificador del SO, no el orden de creación.
