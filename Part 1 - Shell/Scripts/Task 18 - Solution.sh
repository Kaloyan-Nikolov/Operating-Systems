#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: ./myscript.sh FILE DIRNAME "
    exit 1
fi

FILE=$1
DIRNAME=$2

if [ ! -d "${DIRNAME}" ] || [ ! -w "${DIRNAME}" ]; then
    echo "Please enter writable dir!"
    exit 2
fi

if [ "$(find "${DIRNAME}" -mindepth 1 2> /dev/null | wc -l)" -ne 0 ]; then
    echo "Please enter emtpy dir!"
    exit 3
fi

if [ ! -f "${FILE}" ] || [ ! -r "${FILE}" ]; then
    echo "Please enter readable file!"
    exit 4
fi

NUMBER=1
while read LINE; do
    CUR_NAME="$( echo "${LINE}" | cut -d : -f 1 | sed -E "s/\(.*//" | awk '$1=$1' )"
    CUR_NUMBER="$(fgrep "$CUR_NAME" < <(cat "${DIRNAME}/dict.txt" 2> /dev/null) | cut -d ";" -f 2 )"
    if [ -z "$CUR_NUMBER" ]; then
        echo "${CUR_NAME};${NUMBER}" >> "${DIRNAME}/dict.txt"
        CUR_NUMBER=$NUMBER
        NUMBER=$(expr $NUMBER + 1)
    fi

    echo "$LINE" | cut -d ":" -f 2 >> "${DIRNAME}/${CUR_NUMBER}.txt"

done < <(cat "${FILE}")

exit 0
