#include <err.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <cmd>", argv[0]);

    char c;
    char myargs[50][256];
    uint16_t cnt = 0;
    uint8_t argCnt = 0;
    while((read(0, &c, sizeof(c))) > 0)
    {
        // prepare arguments
        if(c == ' ' || c == '\n')
        {
            myargs[argCnt][cnt]=0;
            cnt=0;
            argCnt++;
            if(argCnt == 50)
                err(2, "too many args");
        }
        else
        {
            myargs[argCnt][cnt]=c;
            cnt++;
            if(cnt == 256)
                err(3, "arg too long!");
        }

        if(c == '\n')
        {
            char *arr[52];
            arr[0] = argv[1];
            for(uint16_t i = 1; i < argCnt + 1; i++)
                arr[i] = myargs[i-1];
            arr[argCnt+1]=NULL;

            const pid_t child = fork();
            if(child == -1)
                err(4, "fork failed");

            if(child == 0)
            {
                if(execvp(argv[1], arr) == -1)
                    err(5, "exec failed");
            }

            int status;
            if(wait(&status) == -1)
                err(6, "wait failed");
            if(WIFEXITED(status) == 0)
                warnx("cmd did not finish normally!");
            if(WEXITSTATUS(status) != 0)
                warnx("cmd did not finish with exit code 0");

            argCnt = 0; // prepare for next loop
        }
    }

    exit(0);
}
