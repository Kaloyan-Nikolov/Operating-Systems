#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        errx(1, "Invalid number of arguments! Usage: %s <input.bin>", argv[0]);

    struct stat st;
    if(stat(argv[1], &st) == -1)
        err(2, "Could not stat!");

    if(!(S_ISREG(st.st_mode)))
        errx(3, "Not regular file!");

    if((st.st_mode & S_IRUSR) == 0)
        errx(4, "Not readable file!");

    struct rec_t
    {   
        uint32_t uid;
        uint16_t s1;
        uint16_t s2;
        uint32_t st;
        uint32_t end;
    }cur_rec;

    if(st.st_size % sizeof(cur_rec) != 0)
        errx(5, "File corrupted!");

    // for every user we need to know whether:
    // sqrt of length of all sessions is bigger than the dispersion D
    // for every such user, we need to know and print the:
    // longest session and uid

    // ALGORITHM
    // while reading from file we find sum of length of all sessions
    // this way after cycle we find avg length
    // but also we save all lengths of sessions in an array
    // this way with second cycle we find the dispersion D

    // now the only thing left to do is one more cycle to print the needed info 
    // from the arr of tupl_t which we have filled during reading from file
    // in the first cycle;

    struct tupl_t{
        uint32_t uid;
        uint32_t len;
        uint8_t checked;
    };

    struct tupl_t arr[16384];
    uint32_t cnt = 0;

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
        err(6, "Could not open!");

    ssize_t rd;
    uint32_t alllens = 0;
    uint32_t now_len = 0;
    while((rd = read(fd, &cur_rec, sizeof(cur_rec))) == sizeof(cur_rec))
    {
        now_len = cur_rec.end - cur_rec.st;
        alllens += now_len;

        arr[cnt].uid = cur_rec.uid;
        arr[cnt].len = now_len;
        arr[cnt].checked = 0;
        cnt++;
    }
    if(rd < 0)
    {
        int myerrno = errno;
        close(fd);
        errno = myerrno;
        err(7, "Could not read!");
    }

    close(fd);
    if(cnt == 0)
    {
        warnx("No input!");
        exit(0);
    }
    uint32_t avg = alllens / cnt;
    uint32_t curD = 0;
    uint32_t temp;
    for(uint32_t i = 0; i < cnt; i++)
    {
        temp = (arr[i].len - avg)*(arr[i].len - avg);
        if(curD > curD + temp)
        {
            errx(10, "It has overflowed!");
        }
        curD += temp;
    }
    uint32_t D;
    D = curD / cnt;

    uint32_t longest = 0;
    uint32_t sumlen = 0;
    uint32_t curuid = 0;
    for(uint32_t i = 0; i < cnt; i++)
    {
        curuid = arr[i].uid;
        sumlen = 0;
        for(uint32_t j = i; j < cnt; j++)
        {
            if(arr[j].uid != curuid)
                continue;
            if(arr[j].checked == 1)
                break;

            arr[j].checked = 1;
            sumlen += arr[j].len;
            if(arr[j].len > longest)
                longest = arr[j].len;
        }
        if(sumlen * sumlen > D)
        {
            char buf[100];
            sprintf(buf, "%d %d\n", curuid, longest);
            write(1, buf, strlen(buf));
        }
    }

    exit(0);
}
