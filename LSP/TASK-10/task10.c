#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: Executing 'ls'\n");
        char *args[] = {"ls", NULL};
        execvp(args[0], args);
        // If execvp returns, it must have failed
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        printf("Parent process: Child process terminated.\n");
    }
    return 0;
}

