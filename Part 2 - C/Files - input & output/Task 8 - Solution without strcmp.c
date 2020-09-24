#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int16_t isContained(const char c, const char* arg)
{   
    for(int16_t i = 0; arg[i] != 0; i++)
    {   
        if(c == arg[i])
            return i;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
        errx(1, "Invalid number of arguments! Usage: %s [options] set1 [set2]", argv[0]);

    if(strcmp(argv[1], "-s") == 0)
    {
        //squeeze
        char c;
        char prevC;
        while(read(0, &c, sizeof(char)) > 0)
        {
            if(isContained(c, argv[2]) != -1)
            {
                if(prevC == c)
                    continue;
            }
            write(1, &c, sizeof(char));
            prevC = c;
        }
    }
    else if(strcmp(argv[1], "-d")==0)
    {
        //delete
        char c;
        while(read(0, &c, sizeof(char)) > 0)
        {
            if(isContained(c, argv[2]) == -1)
            {
                write(1, &c, sizeof(char));
            }
        }
    }
    else
    {
        // change
        if(strlen(argv[1]) != strlen(argv[2]))
            errx(2, "set1 and set2 must have equal length!");
        char c;
        while(read(0, &c, sizeof(char)) > 0)
        {
            int16_t pos = isContained(c, argv[1]);
            if(pos == -1)
                write(1, &c, sizeof(c));
            else
            {
                c = argv[2][pos];
                write(1, &c, sizeof(c));
            }
        }
    }

    exit(0);
}
