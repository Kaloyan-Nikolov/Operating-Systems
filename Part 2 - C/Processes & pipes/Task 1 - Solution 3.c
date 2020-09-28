#include <err.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <filename>", argv[0]);

    int pf[2];
    if(pipe(pf) == -1)
        err(2, "Could not create pipe!");

    pid_t child1 = fork();
    if(child1 == -1) 
        err(3, "Could not execute fork!");

    if(child1 == 0)
    {   
        // will write in pipe
        close(pf[0]);
        if(dup2(pf[1],1) == -1)
            err(4, "Could not execute dup2!");

        close(pf[1]);
        if(execlp("cat", "cat", argv[1], (char*)NULL)==-1)
            err(5, "Could not execute cat!");
    }           

    // will not write in pipe
    close(pf[1]);
    if(dup2(pf[0],0)==-1)
        err(6, "Could not execute the 2nd dup2");

    close(pf[0]);
    if(execlp("sort", "sort", (char*)NULL) == -1)
        err(7, "Could not execute sort!");
}
