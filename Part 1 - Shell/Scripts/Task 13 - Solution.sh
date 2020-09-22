#!/bin/bash

#for every user
# - number processes
# - all rss
# - avg rss
# - if biggest rss > avg*2 -> kill

if [ $# -ne 0 ]; then
    echo "Usage: ./myscript"
    exit 1
fi

if [ "$(id -u)" -ne 0 ];then
    echo "You need root permissions!"
    exit 2
fi

while read USR; do
    NUM_PR=0
    ALL_RSS=0
    while read CUR_RSS CUR_PID; do
        NUM_PR=$(expr $NUM_PR + 1)
        ALL_RSS=$(expr $ALL_RSS + $CUR_RSS)
        LAST_RSS=$CUR_RSS
        LAST_PR=$CUR_PID
    done < <(ps -u "${USR}" -o rss=,pid= | sort -nk1)
    if [ "${NUM_PR}" -eq 0 ]; then
        echo "User: ${USR} does not have any processes!"
        continue
    fi

    echo "User: ${USR} has ${NUM_PR} processes and uses ${ALL_RSS}."
    AVG_RSS=$(expr $ALL_RSS / $NUM_PR)
    TWICE_AVG=$(expr $AVG_RSS \* 2)
    if [ ${LAST_RSS} -gt "${TWICE_AVG}" ]; then
        echo "We will kill process: ${LAST_PR}"
        kill -s TERM "${LAST_PR}"
        sleep 2
        kill -s KILL "${LAST_PR}"
    fi
done < <(ps -e -o user= | sort | uniq)
