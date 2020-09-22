#!/bin/bash

if [ $# -ne 1 ];then
    echo "Usage: $0 <username>"
    exit 1
fi

USERNAME=$1

if [ "$(id -u "${USERNAME}" 2> /dev/null | wc -l)" -eq 0 ]; then
    echo "Please enter valid username!"
    exit 2
fi

if [ $(id -u) -ne 0 ]; then
   echo "You need root permissions!"
   exit 3
fi

#A
FOO_PROCESSES="$(ps -u "${USERNAME}" --no-headers | wc -l)"
ALL_USERS="$(ps -e -o user= | sort | uniq)"
echo "${USERNAME} has ${FOO_PROCESSES} processes."
for USR in $ALL_USERS; do
    if [ "${USR}" = "${USERNAME}" ]; then
        continue
    fi

    USR_PROCESSES="$(ps -u "${USR}" --no-headers | wc -l)"
    if [ "${USR_PROCESSES}" -gt "${FOO_PROCESSES}" ]; then
        echo "USER: ${USR}"
    fi
done

#B
ALL_TIME=0;
ALL_PR=0;
for USR in $ALL_USERS; do
    USR_TIME=0;
    USR_PR=0;
    while read CURTIME; do
        USR_TIME=$(expr "${USR_TIME}" + "${CURTIME}")
        USR_PR=$(expr "${USR_PR}" + 1)
    done < <(ps -u ${USR} -o times=)
    ALL_TIME=$(expr $ALL_TIME + $USR_TIME)
    ALL_PR=$(expr $ALL_PR + $USR_PR)
    if [ "${USR_PR}" -eq 0 ];then
        echo "${USR} does not have any processes!"
    else
        AVG_USER=$(expr $USR_TIME / $USR_PR)
        echo "$USR has avg time: $AVG_USER"
    fi
done

if [ "${ALL_PR}" -eq 0 ];then
    echo "There are not any processes!"
else
    AVG_TIME=$(expr $ALL_TIME / $ALL_PR)
    echo "AVG time for all users is $AVG_TIME"
fi

#C
TWICE_AVG=$(expr $AVG_TIME \* 2)
while read CUR_TIME CUR_PID; do
    if [ "${CUR_TIME}" -gt "${TWICE_AVG}" ]; then
        echo "We will kill process ${CUR_PID} which worked for ${CUR_TIME}"
        kill -s TERM $CUR_PID
        sleep 2
        kill -s KILL $PID
    fi
done < <(ps -u ${USERNAME} -o times=,pid=)

exit 0
