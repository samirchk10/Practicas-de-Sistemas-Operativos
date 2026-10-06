/*
 *  Copyright (c) 2019 by Santiago Marco-Sola  <santiagomsola@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * PROJECT: Operating Systems. Practical examples
 * AUTHOR(S): Santiago Marco-Sola <santiago.marco@uab.cat>
 * DESCRIPTION:
 *   This example creates two linux processes. Each one prints
 *   the value of the integer 'number' stored in its own memory space
 */

#include <stdio.h>    // Standard I/O
#include <unistd.h>   // Unix Standard
#include <sys/wait.h> // System Wait
#include <stdlib.h>   // Standard library

int main(int argc,char** argv) {
  int pid, status, number = 300;
  // Create a process (fork)
  if ((pid=fork())==0) {
    // Child process
    number = 400;
    printf("Child process: Number is %d\n",number);
    // Terminate OK
    exit(0);
  } else {
    // Father process
    number = 500;
    printf("Father process: Number is %d\n",number);
    // Wait for child to finish
    wait(&status);
  }
  return 0; // Terminate OK
}
