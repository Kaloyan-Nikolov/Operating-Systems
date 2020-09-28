#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if(argc != 2)
        errx(1, "Usage: ./main <filename>");

    pid_t child = fork();
    if(child == -1)
        err(2, "could not fork");

    if(child == 0)
    {
        if(execlp("sort", "sort", argv[1], (char*)NULL) == -1)
            err(3, "could not execute sort");
    }

    int status;
    if(wait(&status) == -1)
        err(4, "could not wait for child");

    if(WIFEXITED(status)==0)
        errx(5, "child did not finish normally");

    if(WEXITSTATUS(status) != 0)
        warnx("Child did not finish with exit code 0");

    exit(0);
}
