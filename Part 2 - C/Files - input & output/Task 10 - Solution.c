#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc != 3 && argc != 5)
        errx(1, "Invalid number of arguments! Usage: %s -c (N | N-M ) OR -d delim -f (N | N-M)",argv[0]);

    if(argc == 3)
    {
        if(strcmp(argv[1],"-c") != 0)
            errx(2, "first arg must be -c");

        uint8_t st = 1;
        uint8_t end = 1;
        if(strlen(argv[2]) == 1 && argv[2][0] > '0' && argv[2][0] <= '9')
        {   
            st = argv[2][0] - '0';
            end = st;
        }
        else if(strlen(argv[2]) == 3 &&  argv[2][0] > '0' && argv[2][0] <= '9'&& argv[2][1] == '-' &&  argv[2][2] > '0' && argv[2][2] <= '9')
        {   
            st = argv[2][0] - '0';
            end = argv[2][2] - '0';
        }
        else
        {   
            errx(3, "Wrong second arg!");
        }

        if(st > end)
            errx(4, "Wrong second arg!");

        char c;
        uint8_t cnt = 1;
        while(read(0, &c, sizeof(c)) > 0)
        {
            if(c == '\n')
            {
                cnt = 1;
                write(1, &c, 1);
                continue;
            }

            if(cnt >= st && cnt <= end)
            {
                write(1, &c, 1);
            }

            cnt++;
        }
    }
    if(argc == 5)
    {
        if(strcmp(argv[1],"-d") != 0)
            errx(5, "first arg must be -d");

        if(strlen(argv[2]) == 0)
            errx(6, "second args must have lenght 1 at least!");

        char delim = argv[2][0];

        if(strcmp(argv[3], "-f") != 0)
            errx(7, "third arg must be -f");

        uint8_t st = 1;
        uint8_t end = 1;
        if(strlen(argv[4]) == 1 && argv[4][0] > '0' && argv[4][0] <= '9')
        {
            st = argv[4][0] - '0';
            end = st;
        }
        else if(strlen(argv[4]) == 3 &&  argv[4][0] > '0' && argv[4][0] <= '9'&& argv[4][1] == '-' &&  argv[4][2] > '0' && argv[4][2] <= '9')
        {
            st = argv[4][0] - '0';
            end = argv[4][2] - '0';
        }
        else
        {
            errx(3, "Wrong fourth arg!");
        }

        if(st > end)
            errx(4, "Wrong fourth arg!");

        char c;
        uint8_t cnt = 1;
        while(read(0, &c, sizeof(c)) > 0)
        {
            if(c == '\n')
            {
                cnt = 1;
                write(1, &c, 1);
            }

            if(c == delim && cnt >= st && cnt < end)
            {
                write(1, &c, 1);
            }
            else if(c != delim && cnt >= st && cnt <= end)
            {
                write(1, &c, 1);
            }

            if(c == delim)
                cnt++;
        }
    }

    exit(0);
}
