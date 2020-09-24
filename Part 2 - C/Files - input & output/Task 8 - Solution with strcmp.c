#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 3)
        errx(1, "Invalid number of arguments! Usage: %s <-d | -s | set1 > <set1 | set1 | set2 >", argv[0]);
    // -d set1
    // -s set1
    // set1 set2

    char c;
    if(strcmp(argv[1],"-d")==0)
    {
        while(read(0, &c, sizeof(char)) > 0)
        {
            if(strchr(argv[2],c) == NULL)
            {
                write(1, &c, sizeof(char));
            }
        }

        exit(0);
    }
    if(strcmp(argv[1],"-s")==0)
    {
        char prevC=0;
        while(read(0, &c, sizeof(char)) > 0)
        {
            if(strchr(argv[2],c) != NULL)
            {
                if(prevC == c)
                    continue;
            }
            write(1, &c, sizeof(char));
            prevC=c;
        }

        exit(0);
    }
    if(strlen(argv[1]) != strlen(argv[2]))
    {
        errx(3, "SET1 and SET2 must have equal length!");
    }

    while(read(0, &c, sizeof(c)) > 0)
    {
        char* ptr = strchr(argv[1], c);
        if(ptr != NULL)
        {
            uint16_t index = ptr - argv[1];
            c = argv[2][index];
        }

        write(1, &c, sizeof(char));
    }

    exit(0);
}
