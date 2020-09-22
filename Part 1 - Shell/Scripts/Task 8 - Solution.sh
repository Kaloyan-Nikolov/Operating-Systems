#!/bin/bash
  
if [ $# -ne 3 ]; then
    echo "Usage: ./myscript filename string1 string2"
    exit 1
fi

FILENAME=$1
STR1=$2
STR2=$3

if [ ! -f "${FILENAME}" ] || [ ! -r "${FILENAME}" ]; then
    echo "Please enter readable file!"
    exit 2
fi

if [ -z "${STR1}" ] || [ -z "${STR2}" ]; then
    echo ""Please enter non-empty strings!
    exit 3
fi

SET1=$(cat "${FILENAME}" | egrep "^${STR1}=" | cut -d'=' -f 2)
SET2=$(cat "${FILENAME}" | egrep "^${STR2}=")

if [ -z "${SET2}" ]; then
    echo "String2 does not exist in file!"
    echo "We do not have to do anything!"
    exit 0
fi

SET2=$(echo "${SET2}" | cut -d '=' -f 2)

if [ -z "${SET2}" ]; then
    echo "String2 does not contain any terms!"
    echo "We do not have to do anything!"
    exit 0
fi

NEWSET2=""
for TERM in ${SET2}; do
    if fgrep -q "${TERM}" <(echo ${SET1}); then
        continue;
    fi

    if [ -z "${NEWSET2}" ]; then
        NEWSET2+="${TERM}"
    else
        NEWSET2+=" ${TERM}"
    fi

done
sed -E -i "s/${STR2}=${SET2}/${STR2}=${NEWSET2}/" $FILENAME

exit 0
