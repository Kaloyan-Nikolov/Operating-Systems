#!/bin/bash

if [ $(id -u) -ne 0 ]; then
    echo "You need root permissions!"
    exit 1
fi

ROOT_RSS=0;
while read CUR_RSS; do
    ROOT_RSS=$(expr $ROOT_RSS + $CUR_RSS)
done < <(ps -u root -o rss=)

ALL_USERS="$(ps -e -o user= | sort | uniq)"

for USR in $ALL_USERS; do
    if [ "$USR" = "root" ]; then
        continue
    fi

    HOMEDIR="$(cat /etc/passwd | egrep "^${USR}:" | cut -d: -f 6)"

    #if none of the 3 ifs is true we continue
    CHECK=0
    if [ ! -d "${HOMEDIR}" ];then
        CHECK=1
    fi

    OWNER_DIR="$(find / -type d -name "${HOMEDIR}" -printf "%u\n" 2> /dev/null)"
    if [ "${OWNER_DIR}" != "${USR}" ]; then
        CHECK=1
    fi

    if [ -n "${OWNER_DIR}" ] && sudo -u "${OWNER_DIR}" [ ! -w "${HOMEDIR}" ]
    then
       CHECK=1
    fi

    if [ $CHECK -eq 0 ]; then
        continue
    fi

    USER_RSS=0
    while read CUR_RSS; do
        USER_RSS=$(expr $USER_RSS + $CUR_RSS)
    done < <(ps -u "$USR" -o rss=)

    echo "USER_RSS: $USER_RSS for ${USR}  ROOT_RSS: $ROOT_RSS"
    if [ ${USER_RSS} -gt ${ROOT_RSS} ]; then
        #we have to kill all processes
        while read CURR_PID; do
            echo "we are killing process with pid: $CURR_PID"
            kill -s TERM $CURR_PID
            sleep 2
            kill -s KILL $CURR_PID
        done < <(ps -u "${USR}" -o pid= | tr -d ' ')
    fi
done

exit 0
