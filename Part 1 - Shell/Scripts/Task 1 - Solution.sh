#!/bin/bash

if [ $# -ne 1 ];then
    echo "Usage: $0 <filename>"
    exit 1
fi

FILENAME="${1}"

if [ ! -f "${FILENAME}" ] || [ ! -r "${FILENAME}" ];then
    echo "Please enter readable file!"
    exit 2
fi

CNT=1
NEWCONTENT=$(mktemp)
while read LINE; do
    echo "${CNT}. ${LINE}" >> "${NEWCONTENT}"
    CNT=$(expr ${CNT} + 1)
done < <(cat "${FILENAME}" | cut -d " " -f 4- )

cat "${NEWCONTENT}" | sort -t " " -k 2

rm -- "${NEWCONTENT}"

exit 0
