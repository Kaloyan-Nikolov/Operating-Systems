#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    char cmd[5]="echo";
    if(argc >= 3)
        errx(1, "Invalid number of arguments! Usage: %s [cmd]", argv[0]);

    if(argc == 2)
    {   
        size_t strl = strlen(argv[1]);
        if(strl > 4)
            errx(2, "Entered argument must be a command of no more than 4 char");      
        
        strcpy(cmd, argv[1]);
    }

    char arg1[5];
    char arg2[5];
    int8_t cnt1 = 0;
    int8_t cnt2 = 0;
    ssize_t rd;
    char buf[4096];
    while((rd = read(0, buf, sizeof(buf))) > 0)
    {
        for(uint16_t i = 0; i < rd;)
        {
            while(i < rd && buf[i] != ' ' && buf[i] != '\n')
            {
                arg1[cnt1]=buf[i];
                cnt1++;
                i++;
                if(cnt1 > 4)
                    errx(3, "Entered parameters cannot be longer that 4 chars!");
            }
            arg1[cnt1]='\0';
            i++;
            while(i < rd && buf[i] != ' ' && buf[i] != '\n')
            {
                arg2[cnt2]=buf[i];
                cnt2++;
                i++;
                if(cnt2 > 4)
                    errx(4, "Entered parameters cannot be more than 4 chars!");
            }
            arg2[cnt2]='\0';
            i++;
            size_t len1 = strlen(arg1);
            size_t len2 = strlen(arg2);

            if(len1 == 0 && len2 == 0)
            {
                break;
            }

            const pid_t child = fork();
            if(child == -1)
                err(5, "Could not execute child!");
            if(child == 0)
            {
                if(len2 > 0)
                {
                    if(execlp(cmd, cmd, arg1, arg2, (char*)NULL) == -1)
                        err(7, "Could not execute %s!", cmd);

                }
                if(execlp(cmd, cmd, arg1, (char*)NULL) == -1)
                    err(6, "Could not execute %s!", cmd);
            }

            if(wait(NULL) == -1)
                err(7, "Could not execute wait!");

            cnt1=0;
            cnt2=0;
        }

    }
    if( rd < 0)
    {
        err(8, "Could not read from stdin");
    }

    exit(0);
}
