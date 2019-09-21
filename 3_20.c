#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024*16
char wmsg[BUFFER_SIZE];
char rmsg[BUFFER_SIZE];
int main(int argc,char **argv) {
    if (argc != 3) {
        printf("Wrong parameter.\n");
        fflush(stdout);
        return 1;
    }
    char *fromFile = argv[1];
    char *toFile = argv[2];
    int fd[2];
    pid_t pid;

    FILE *fp = fopen(fromFile, "r");
    if (fp == NULL)
        return 2;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed.\n");
        return 1;
    }
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    if (pid > 0) {///father process

        FILE *fp = fopen(fromFile, "r");
        if (fp == NULL)
            return 1;
        fread(wmsg, sizeof(char), sizeof(wmsg), fp);
        close(fd[READ_END]);

        write(fd[WRITE_END], wmsg, strlen(wmsg) + 1);

        close(fd[WRITE_END]);
        fclose(fp);
    } else {
        wait(NULL);
        close(fd[WRITE_END]);
        read(fd[READ_END], rmsg, sizeof(rmsg));
        close(fd[READ_END]);
        FILE *fp = fopen(toFile, "w");
        fwrite(rmsg, sizeof(char), strlen(rmsg), fp);
    }
    return 0;
}
