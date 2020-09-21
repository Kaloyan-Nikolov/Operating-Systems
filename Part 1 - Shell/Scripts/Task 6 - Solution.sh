#!/bin/bash
  
if [ $# -ne 1 ]; then
    echo "Usage: ./myscript <number>"
    exit 1
fi

NUMBER=$1

if egrep -qv "^[+-]?[0-9]+$" < <(echo "$NUMBER"); then
    echo "Please enter a number!"
    exit 2
fi

if [ $(id -u) -ne 0 ]; then
    echo "You need root permissions!"
    exit 3
fi

ALL_USERS=$(ps -e -o user= | sort | uniq)
for CUR_USER in $ALL_USERS; do
    ALL_RSS=0
    while read CUR_RSS PID; do
        ALL_RSS=$(expr $CUR_RSS + $ALL_RSS)
        RSS_LAST_PROCESS=$CUR_RSS
        LAST_PID=$PID
    done < <(ps -u "$CUR_USER" -o rss=,pid= | sort -n)

    echo "CUR_USER: ${CUR_USER}, RSS: ${ALL_RSS}, Last process: ${LAST_PID} and its RSS: ${RSS_LAST_PROCESS}"

    if [ $ALL_RSS -gt $NUMBER ]; then
        echo "Killing process with PID: ${LAST_PID} which uses ${RSS_LAST_PROCESS} rss"
        kill -s TERM $LAST_PID
        sleep 2
        kill -s KILL $LAST_PID
    fi
done

exit 0
