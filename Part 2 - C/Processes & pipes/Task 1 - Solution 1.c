#include <err.h>
#include <unistd.h>
//#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <filename>", argv[0]);

    if(execlp("sort", "sort", argv[1], (char*)NULL) == -1)
        err(2, "Could not execute sort!");

    // exit(0) - not needed - will never execute;
}
