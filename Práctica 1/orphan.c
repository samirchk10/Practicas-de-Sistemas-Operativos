#include <stdio.h>    // Standard I/O
#include <unistd.h>   // Unix Standard
#include <sys/wait.h> // System Wait
#include <stdlib.h>   // Standard library
#include <string.h>   // String library

int main (int argc, char** argv) {
    int pid = fork();

    if (pid == 0) {
        // Child process waits for the parent to finish
        sleep(1);

        // From this point, this is an _orphan_ process, as it's parent has
        // already executed the exit.
        //
        // You should print the PID of the new parent (check the getppid()
        // function), and explain why the new parent process is special.

        // ADD YOUR CODE HERE

    } else {
        // Parent process exists very fast, leaving its child orphan :(
        exit(0);
    }
    return 0;
}
