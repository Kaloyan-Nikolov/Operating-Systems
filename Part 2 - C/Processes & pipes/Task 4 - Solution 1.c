#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if(argc > 2)
        errx(1, "Invalid number of arguments! Usage: %s [cmd]", argv[0]);

    char* cmd = "echo";
    if(argc == 2)
    {
        if(strlen(argv[1]) > 4)
            errx(2, "CMD must be 4 chars at most!");

        cmd = argv[1];
    }

    char line[256];
    ssize_t rd;
    char args[2][5];
    uint8_t argNum = 0;
    uint8_t cnt = 0;
    while((rd = read(0, line, sizeof(line))) > 0)
    {
        for(uint16_t i = 0; i < rd; i++)
        {
            if(line[i] == ' ' || line[i] == '\n')
            {
                args[argNum][cnt]=0;
                cnt = 0;
                argNum++;
            }
            else
            {
                if(cnt == 4)
                    errx(3, "Arg is too long");

                args[argNum][cnt++]=line[i];
            }
            if(argNum == 2)
            {
                argNum = 0;
                const pid_t child = fork();
                if(child == -1)
                    err(4, "fork failed");
                if(child == 0)
                {
                    char *arr[] = { cmd, args[0], args[1], NULL };
                    if(execvp(cmd, arr) == -1)
                        err(5, "exec failed");
                }
                int status;
                if(wait(&status) == -1)
                    err(5, "could not wait");
                if(WIFEXITED(status) == 0)
                    warnx("%s did not finish normally!", cmd);
                if(WEXITSTATUS(status) != 0)
                    warnx("%s did not finish with exit code 0!", cmd);
            }
        }
    }
    if(argNum == 1)
    {
        const pid_t child = fork();
        if(child == -1)
            err(4, "fork failed");
        if(child == 0)
        {
            char *arr[] = { cmd, args[0], NULL };
            if(execvp(cmd, arr) == -1)
                err(5, "exec failed");
        }
        int status;
        if(wait(&status) == -1)
            err(5, "could not wait");
        if(WIFEXITED(status) == 0)
            warnx("%s did not finish normally!", cmd);
        if(WEXITSTATUS(status) != 0)
            warnx("%s did not finish with exit code 0!", cmd);
    }

    exit(0);
}
