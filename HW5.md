#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *cmd_copy = strdup(argv[i]);
        if (!cmd_copy) {
            perror("strdup");
            continue;
        }

        char *args[128];
        int arg_count = 0;
        char *token = strtok(cmd_copy, " \t");
        while (token != NULL && arg_count < 127) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t");
        }
        args[arg_count] = NULL;

        if (arg_count == 0) {
            free(cmd_copy);
            continue;
        }

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            free(cmd_copy);
            continue;
        }

        pid_t pid_cmd = fork();
        if (pid_cmd == -1) {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            free(cmd_copy);
            continue;
        }

        if (pid_cmd == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        pid_t pid_wc = fork();
        if (pid_wc == -1) {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid_cmd, NULL, 0);
            free(cmd_copy);
            continue;
        }

        if (pid_wc == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execlp("wc", "wc", "-c", NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid_cmd, NULL, 0);
        waitpid(pid_wc, NULL, 0);

        free(cmd_copy);
    }

    return 0;
}