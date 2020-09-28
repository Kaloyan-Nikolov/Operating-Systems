#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if(argc != 2)
        errx(1, "Invalid number of arguments. Usage: %s <filepath>", argv[0]);

    if(mkfifo("mypipe", 0600) == -1)
        err(2, "mkfifo failed");

    int fd = open("mypipe", O_WRONLY);
    if(fd == -1)
        err(3, "open failed");

    if(dup2(fd, 1) == -1)
        err(4, "dup failed");

    if(execlp("cat", "cat", argv[1], (char *)NULL) == -1)
        err(5, "execlp failed");
}
