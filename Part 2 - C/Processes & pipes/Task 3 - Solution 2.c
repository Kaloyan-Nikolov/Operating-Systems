#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if(argc != 1)
        errx(1, "Invalid number of arguments! Usage: %s", argv[0]);

    int a[2];
    if(pipe(a) == -1)
        err(2 , "Could not create pipe");

    const pid_t pid_cut = fork();
    if(pid_cut == -1)
        err(3, "fork failed");
    if(pid_cut == 0)
    {
        // will write in a;
        close(a[0]);
        if(dup2(a[1],1) == -1)
            err(4, "dup2 failed");
        close(a[1]);
        if(execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) == -1)
            err(5, "exec failed");
    }

    int b[2];
    if(pipe(b) == -1)
        err(6, "pipe failed");

    pid_t pid_sort = fork();
    if(pid_sort == -1)
        err(7, "fork failed");

    if(pid_sort == 0)
    {
        // will read from a;
        // wiil write in b;
        close(a[1]);
        if(dup2(a[0],0)== -1)
            err(8, "dup2 failed");
        close(a[0]);
        close(b[0]);
        if(dup2(b[1],1) == -1)
            err(9, "dup2 failed");
        close(b[1]);
        if(execlp("sort", "sort", (char*)NULL) == -1)
            err(10, "exec failed");
    }

    close(a[0]);
    close(a[1]);

    int c[2];
    if(pipe(c) == -1)
        err(11, "pipe failed");

    pid_t pid_uniq = fork();
    if(pid_uniq == -1)
        err(12, "fork failed");
    if(pid_uniq == 0)
    {
        // will read from b;
        // will write in c;
        close(b[1]);
        if(dup2(b[0], 0) == -1)
            err(13, "dup2 failed");
        close(b[0]);
        close(c[0]);
        if(dup2(c[1],1) == -1)
            err(14, "dup2 failed");
        close(c[1]);
        if(execlp("uniq", "uniq", "-c", (char*)NULL) == -1)
            err(15, "exec failed");
    }

    close(b[0]);
    close(b[1]);

    pid_t pid_sortn = fork();
    if(pid_sortn == -1)
        err(16, "fork failed");

    if(pid_sortn == 0)
    {
        // will read from c;
        close(c[1]);
        if(dup2(c[0], 0) == -1)
            err(16, "dup2 failed");
        close(c[0]);

        if(execlp("sort", "sort", "-n", (char*)NULL) == -1)
            err(17, "exec failed");
    }

    close(c[0]);
    close(c[1]);

    int status;
    if(waitpid(pid_sortn, &status, 0) == -1)
    {
        err(20, "waitpid failed");
    }

    if(WIFEXITED(status) == 0)
    {
        errx(21, "Did not finish nornally");
    }

    if(WEXITSTATUS(status) != 0)
    {
        errx(22, "Did not finish with exit status 0!");
    }

    exit(0);
}
