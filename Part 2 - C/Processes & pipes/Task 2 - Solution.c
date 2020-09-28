#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 1)
        errx(1, "Invalid number of arguments! Usage: %s", argv[0]);

    while(1)
    {
        write(1, "> My prompt:",12 );
        char buf[256];

        int cnt = 0;
        while(read(0, buf + cnt, sizeof(char)) > 0 && *(buf + cnt) != '\n')
        {
            cnt++;
        }
        buf[cnt]=0;

        if(strlen(buf) == 0)
        {
            continue;
        }

        if(strcmp(buf,"exit") == 0)
        {
            write(1, "Exiting!\n",9);
            break;
        }

        char cmd[280];
        sprintf(cmd, "/usr/bin/%s", buf);

        const pid_t child = fork();
        if(child == -1)
        {
            err(2, "Fork failed");
        }
        if(child == 0)
        {
            if(execlp(cmd, cmd, (char*)NULL) == -1)
                err(3, "exec failed");
        }

        int status;
        if(wait(&status) == -1)
        {
            err(4, "wait failed");
        }

        if(WIFEXITED(status) == 0)
        {
            warnx("%s did not finished normally!", cmd);
        }
        else if ( WEXITSTATUS(status) != 0)
        {
            warnx("%s did not finish with exit code 0", cmd);
        }
    }

    exit(0);
}
