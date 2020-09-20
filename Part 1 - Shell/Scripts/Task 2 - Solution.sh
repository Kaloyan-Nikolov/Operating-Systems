#!/bin/bash
  
if [ $# -ne 2 ];then
    echo "Usage: $0 <number1> <number2>"
    exit 1
fi

#we assume that we do NOT need to check if $1 and $2 are numbers and if $1 <= $2

NUM1="${1}"
NUM2="${2}"
# if numLines < NUM1 -> a
# if numLines >= NUM1 but < NUM2 -> b
# else -> c

while read LINE; do
    NUM_LINES="$(cat "${LINE}" | wc -l )"
    if [ "${NUM_LINES}" -lt ${NUM1} ]; then
        if [ -z "$(find . -type d 2> /dev/null | egrep "a" )" ]; then
            mkdir "./a/"
        fi

        cp "$LINE" "a/${LINE}"
        continue
    fi

    if [ "${NUM_LINES}" -lt "$NUM2" ]; then
        if [ -z "$(find . -type d 2> /dev/null | egrep "b" )" ]; then
            mkdir "./b/"
        fi

        cp "$LINE" "b/${LINE}"
        continue
    fi

    if [ -z "$(find . -type d 2> /dev/null | egrep "c" )" ]; then
        mkdir "./c/"
    fi

    cp "$LINE" "c/${LINE}"
done < <(find . -type f 2> /dev/null)

exit 0
