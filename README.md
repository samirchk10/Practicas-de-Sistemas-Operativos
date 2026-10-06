# Sistemas Operativos: programación de sistemas en C

Prácticas de la asignatura **Sistemas Operativos** del grado en Ingeniería Informática en la Universidad Autónoma de Barcelona. Trabajo a bajo nivel con el sistema operativo: creación y sincronización de procesos, gestión de memoria y comunicación entre procesos mediante llamadas al sistema POSIX.

## Competencias

- **Programación en C** a bajo nivel, con gestión manual de memoria y descriptores de fichero
- **Llamadas al sistema POSIX:** `fork()`, `wait()`, `exit()`, `pipe()`, `read()`, `write()`, `execvp()`, `kill()`
- **Concurrencia:** comportamiento no determinista de procesos y papel del planificador del SO
- **Comunicación entre procesos (IPC)** con pipes
- **Gestión del ciclo de vida de procesos:** procesos huérfanos, zombies y señales (`SIGKILL`)
- **Depuración y verificación** de hipótesis con pruebas propias y análisis de la salida

## Prácticas

### [Práctica 1: Procesos](./Practica%201)

Estudio completo del ciclo de vida de los procesos en Unix, culminando en la implementación de un **mini-shell** propio. Incluye:

- **Creación de procesos:** creación de múltiples hijos con `fork()` y sincronización con el padre mediante `wait()`
- **Concurrencia:** experimentos para demostrar que el orden de ejecución lo decide el planificador, no el orden de creación
- **Aislamiento de memoria:** análisis de cómo padre e hijo tienen copias independientes de sus variables tras un `fork()`
- **Comunicación entre procesos:** envío de datos padre → hijo mediante pipes y comportamiento del buffer del kernel
- **Procesos huérfanos y zombies:** identificación del nuevo padre de un proceso huérfano y análisis del estado zombie, incluido por qué no responde a `SIGKILL` hasta que el padre hace `wait()`
- **Mini-shell:** intérprete de comandos que lee órdenes del usuario, crea un proceso hijo y lo reemplaza con el programa pedido usando `execvp()` (soporta `ls`, `cat`, `echo`, `ps`...)

*(Más prácticas próximamente)*

## Tecnologías

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![macOS](https://img.shields.io/badge/macOS-000000?style=flat&logo=apple&logoColor=white)

## Cómo ejecutarlo

```bash
cd "Practica 1"
gcc exemple1.c -o exemple1
./exemple1
```

## Contacto

**Samir Channagui** · [LinkedIn](https://www.linkedin.com/in/samirck/)
# Practicas-de-Sistemas-Operativos
