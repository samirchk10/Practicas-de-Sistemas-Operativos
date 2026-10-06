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

## Ejercicio 2: Espacio de memoria de procesos y concurrencia

### `pexample2.c`

```c
int main(int argc, char** argv) {
    int pid, status, number = 300;

    // Create a process (fork)
    pid = fork();
    if (pid == 0) {
        // Child process
        number = 400;
        printf("Child process: Number is %d\n", number);
        // Terminate OK
        exit(0);
    } else {
        // Father process
        number = 500;
        printf("Father process: Number is %d\n", number);
        // Wait for child to finish
        wait(&status);
    }

    return 0; // Terminate OK
}
```
**a) ¿Imprimen los dos procesos el mismo número por pantalla? ¿Por qué?**

- No. El `fork()` crea una copia del espacio de memoria del padre para el hijo. Justo después del `fork()`, los dos tienen una variable `number` con el valor 300, pero son dos variables diferentes, cada una en la memoria de su proceso. Cuando al hijo le asignamos 400, solo cambia su copia; cuando al padre le asignamos 500, solo cambia su copia de la variable `number`. Un proceso no puede ver ni modificar las variables del otro.

```
    Sistemas Operativos/Practicas/Práctica 1 ❯ ./exemple2
    Father process: Number is 500
    Child process: Number is 400
```

**b) ¿Qué pasaría si ambas llamadas a `printf()` se hicieran antes de la asignación a `number`? ¿Por qué?**

- Los dos imprimirían 300. El hijo hereda una copia de la memoria del padre en el momento del `fork()`, cuando `number` valía 300 y todavía no se había cambiado.

```
    Sistemas Operativos/Practicas/Práctica 1 ❯ ./exemple2
    Child process: Number is 300
    Father process: Number is 300
```

**c) ¿Depende el resultado de la ejecución del programa del orden en que los dos procesos asignan valor a la variable `number`? ¿Por qué?**

- No. Aunque el orden de ejecución lo decide el planificador y puede variar, cada proceso asigna el valor a su propia copia de `number`. La asignación de un proceso no afecta a la variable del otro.

**d) ¿En qué punto del programa se reserva espacio para cada variable `number`?**

- El `number` del padre se reserva al empezar a ejecutarse `main()`, en la pila del proceso, al declarar `int pid, status, number = 300;`.
- El `number` del hijo se crea en el `fork()`: el SO duplica el espacio de memoria del padre (incluida la pila), así el hijo obtiene su propia copia de `number`, con el valor 300 que tenía en ese momento.

**e) ¿Es necesaria más de una CPU para ejecutar el código anterior? En caso negativo, describe cómo se ejecutarían los dos procesos en un procesador con una sola CPU. Por ejemplo, puedes explicar las instrucciones que se van ejecutando en cada proceso.**

- No. Una posible secuencia de ejecución sería:
  - **Padre:** empieza `main()`, reserva sus variables en la pila y asigna `number = 300`.
  - **Padre:** llama a `fork()`. El SO crea el proceso hijo con una copia del espacio de memoria del padre (incluido `number = 300`) y lo pone en la cola de procesos listos.
  - **Padre** (sigue teniendo la CPU): `fork()` le devuelve el PID del hijo, así que entra en el `else`. Asigna `number = 500` en su copia e imprime `Father process: Number is 500`.
  - **Padre:** llama a `wait(&status)`. Como el hijo todavía no ha terminado, el padre se bloquea. El planificador del SO hace un cambio de contexto y le da la CPU al hijo.
  - **Hijo:** continúa justo después del `fork()`, que en su caso devuelve 0, así que entra en el `if`. Asigna `number = 400` en su propia copia de la variable `number` e imprime `Child process: Number is 400`.
  - **Hijo:** llama a `exit(0)` y termina. El SO avisa al padre.
  - **Padre:** se desbloquea, `wait()` retorna y el padre ejecuta `return 0` y termina.
 
## Ejercicio 3: Comunicación entre procesos. Pipes

### `pexample3.c`

```c
int main(int argc, char **argv) {
    int fd[2];
    int pid, status;

    // Create an unnamed pipe (store pipe descriptors into fd)
    pipe(fd);

    // Fork
    if ((pid = fork()) == 0) {
        // Child process
        printf("Child process: Created\n");
        // Read integer from pipe
        int number = 0;
        read(fd[0], &number, sizeof(int));
        // Print number
        printf("Child process: Number read %d\n", number);
        // Terminate OK
        exit(0);
    } else {
        // Father process
        int number = 900;
        // Write integer into the pipe
        printf("Father process\n");
        write(fd[1], &number, sizeof(int));
        printf("Father process: Number written\n");
        // Wait for child to finish
        wait(&status);
    }

    return 0; // Terminate OK
}
```
Hay que destacar cómo crear un pipe. Usaremos `int fd[2]`, donde `fd[0]` será el extremo de lectura y `fd[1]` el extremo de escritura. Lo tendremos que crear antes del `fork()`, porque al hacer `fork()` el hijo hereda una copia de los descriptores del padre, así que los dos tienen acceso al mismo pipe. Si se crea después, cada uno tendría un pipe diferente y no podrían comunicarse.

`pipe(fd)` le pide al SO que cree un pipe y devuelve sus dos extremos guardándolos en el array `fd`:

- En `fd[0]` pone un número que identifica el extremo de lectura.
- En `fd[1]` pone un número que identifica el extremo de escritura.

Estos números son descriptores de fichero, identificadores que el sistema utiliza para todo lo que se puede leer o escribir (ficheros, la terminal, pipes...).

**a) ¿Qué sucede si el padre envía el dato antes de que el hijo esté leyendo desde el otro extremo? ¿Se pierde el dato? ¿Salta un error?**

- No se pierde ni da error. El pipe tiene un buffer en el kernel: cuando el padre hace `write()`, el dato se guarda allí y se queda esperando. Cuando el hijo llama más tarde a `read()`, lo encontrará y lo leerá. Si añadimos un `sleep(2)` en el hijo antes del `read()`, el padre escribirá "Number written" mientras el hijo todavía duerme, y 2 segundos después lo leerá igualmente:

```
    Sistemas Operativos/Practicas/Práctica 1 ❯ ./exemple3
    Father process
    Father process: Number written
    Child process: Created
    Child process: Number read 900
```

**b) ¿Qué sucede si el hijo quiere leer el dato antes de que el padre haya escrito en el otro extremo? ¿El hijo ignorará la llamada a `read()`?**


