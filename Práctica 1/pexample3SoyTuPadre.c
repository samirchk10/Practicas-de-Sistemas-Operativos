#include <stdio.h>    // Standard I/O
#include <unistd.h>   // Unix Standard
#include <sys/wait.h> // System Wait
#include <stdlib.h>   // Standard library
#include <string.h>   // String library

#define BUFFER_SIZE 100

int main(int argc,char** argv) {
  int fd[2];
  int pid, status;
  // Create an unnamed pipe (store pipe descriptors into fd)
  pipe(fd);
  // Fork
  if ((pid=fork())==0) {
    // Child only reads
    close(fd[1]);
    // Child process
    printf("Child process: Created\n");
    // Buffer of characters to store the received string
    char buffer[BUFFER_SIZE];
    int n = read(fd[0],buffer, BUFFER_SIZE);
    // Print received string and how many bytes it takes
    printf("Child process: string read \"%s\" (%d bytes)\n", buffer, n);
    // We close the reading side because we don't need it anymore
    close(fd[0]);
    // Terminate OK
    exit(0);
  } else {
    // We don't need the reading side
    close(fd[0]);
    // String to send to the child
    char message[] = "SOY TU PADRE";
    printf("Father process\n");
    // Write message into the pipe (+1 to also send the '\0' that marks the end of the string)
    write(fd[1],message,strlen(message) + 1);
    printf("Father process: Message written\n");
    // Closing the writing side of the pipe because we have already sent the message
    close(fd[1]);
    // Wait for child to finish
    wait(&status);
  }
  return 0; // Terminate OK
}
