#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//loop
//  line = readline
//  args = split(line) -> if build-in
//  sh_execute               do_buildin[args]
//                        else                                       |
//                           do_fork         --> pid == fork()       |
//                                               children? execvp   ====> lauch another app
//                                               parent? waitpid     |

int do_cd(char **args);
int do_exit(char **args);

char *builtin_str[] = {
    "cd",
    "exit"
};

int (*builtin_func[]) (char **) = {
        &do_cd,
        &do_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int do_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "cd expected at least one argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("WTF in do_cd");
        }
    }
    return 1;
}

int do_exit(char **args)
{
    printf("%s",args[0]);
    return 0;
}

int sh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("WTF in sh_launch children process");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("WTF in sh_launch error forking");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);//blocking
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int sh_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return sh_launch(args);
}

char *read_line(void)
{
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) {
        printf("error..can't get stdin\n");
    } else {
        printf(">");
    }
    return line;
}

char** split_line(char *line)
{
    char delim[] = " \t\r\n\a";
    int bufsize = 64;
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token;
    int position = 0;

    if (!tokens) {
        fprintf(stderr, "split_line allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token!=NULL) {
        tokens[position++] = token;
        if(position > bufsize) {
            printf("do you really need more than 64 args for a shell?\n" );
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, delim);
    }
    return tokens;
}


void repl(void)
{
    char* line;
    char** args;
    int status;

    do {
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char *argv[])
{
    repl();
    return 0;
}
