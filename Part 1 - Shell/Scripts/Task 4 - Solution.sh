#!/bin/bash
 
while read CUR_HOMEDIR; do
    SEARCHED_NAME="$(echo "${CUR_HOMEDIR}" | cut -d: -f6 | xargs -I {} basename {})"
    LAST_ST_CH="$(find / -type d -name "${SEARCHED_NAME}" -printf "%C@" 2> /dev/null | cut -d "." -f 1)"

    if [ -z "$(echo ${LAST_ST_CH})" ];then
        continue;   
    fi 

    if [ "${LAST_ST_CH}" -ge 1551168000 ] || [ "${LAST_ST_CH}" -lt 1551176100 ];then
        echo "${CUR_HOMEDIR}" | cut -d: -f1,5 | cut -c 2- | sed -E "s/,.*//" | tr ":" "\t"
    fi
done < <(cat /etc/passwd | egrep ":/home/SI/[^:]+:[^:]+$")

exit 0
