#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_SIZE 2048

void parse_command (char* cmd_line, char **cmd_argv) {
	while (*cmd_line != '\0') {
		while (*cmd_line == ' ' || *cmd_line == '\t' || *cmd_line == '\n') *cmd_line++ = '\0';
		*cmd_argv++ = cmd_line;
		while (*cmd_line != '\0' && *cmd_line != ' ' && *cmd_line != '\t' && *cmd_line != '\n') 
			cmd_line++;
	}
	*cmd_argv = NULL;  
}

void execute_command (char* command, char **cmd_argv) {
	// This function must:
	// 1. Create a child process
	// 2. The child process should be converted to the cmd_line process (using cmd_argv arguments)
	// 3. The parent process must wait for the child process to finish

	// !! YOUR CODE HERE !!
}

int main (int argc, char** argv) {
    char cmd_line[INPUT_SIZE];
    char pwd[INPUT_SIZE];
    char *cmd_argv[32];

    while (1) {
        getcwd(pwd, INPUT_SIZE);
        printf("SO-2022-shell:%s$> ", pwd);

        // Read the user input
        if (fgets(cmd_line, INPUT_SIZE, stdin) == 0) {
            // EOF = CTRL+D (exit terminal)
            printf("\n");
            break;
        }

        // Remove the newline character from the input
        char* nl = strchr(cmd_line, '\n');;
        if (nl) *nl = '\0';

        // If the there is not input, don't execute anything
        if (strnlen(cmd_line, INPUT_SIZE) == 0) continue;

        // The "exit" command will finish the terminal session
        if (strncmp(cmd_line, "exit", 4) == 0) break;

		parse_command(cmd_line, cmd_argv);
        if (strncmp(cmd_line, "cd", 2) == 0) {
            // cd is a shell builtin command, change the current process path
            if (cmd_argv[1] != NULL)
                if (chdir(cmd_argv[1]) < 0) printf("-minishell: cd: %s: No such file or directory\n", cmd_argv[1]);
        } else {
		    execute_command(cmd_line, cmd_argv);
        }
    }
	return 0;
}
