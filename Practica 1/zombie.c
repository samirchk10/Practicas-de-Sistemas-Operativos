#include <stdio.h>    // Standard I/O
#include <unistd.h>   // Unix Standard
#include <sys/wait.h> // System Wait
#include <stdlib.h>   // Standard library
#include <string.h>   // String library
#include <signal.h>   // SIGKILL

void print_child_status(int child_pid) {
    char cmd[256];
    snprintf(cmd, 256, "ps | grep %d", child_pid);
    system(cmd);
}

int main (int argc, char** argv) {
    int status;
    int pid = fork();

    if (pid == 0) {
        // Child process exits very fast
        sleep(1);
        exit(0);
    } else {
        printf("Initial child status:\n");
        print_child_status(pid);

        // Parent process waits for its child to DIE
        sleep(3);

        // Check the dead child status
        printf("Dead child status:\n");

        // YOUR CODE HERE

        // Send a SIGKILL signal to the child process, this is done using the
        // `kill` syscall, you can see the manual here: https://man7.org/linux/man-pages/man2/kill.2.html

        // YOUR CODE HERE

        // Give some time to the child zombie process to react
        sleep(3);

        // Check the dead child status, it is still there? Why?
        printf("Dead child status after SIGKILL:\n");

        // YOUR CODE HERE

        // Wait for the zombie process
        wait(&status);

        // Check again the child process status, is the process still arround,
        // why?
        printf("Dead child status after wait:\n");

        // YOUR CODE HERE

    }
    return 0;
}
