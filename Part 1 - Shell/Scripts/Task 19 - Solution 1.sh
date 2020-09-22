#!/bin/bash
  
if [ $# -ne 2 ];then
    echo "Usage: $0 <a.csv> <b.scv>"
    exit 1
fi

INPUTFILE="$1"
OUTPUTFILE="$2"

echo -n > "${OUTPUTFILE}"

if [ ! -f "${INPUTFILE}" ] || [ ! -r "${INPUTFILE}" ];then
    echo "Please enter readable first file!"
    exit 2
fi

while read LINE; do
    if [ -n "$(cat "${OUTPUTFILE}" 2> /dev/null | egrep ",${LINE}$"  )"  ]; then
        continue
    fi

    NUMBER="$(cat "${INPUTFILE}" | egrep ",${LINE}$" | cut -d ',' -f 1 | sort -n | head -n 1)"
    echo "${NUMBER},${LINE}" >> "${OUTPUTFILE}"
done < <(cat ${INPUTFILE} | cut -d "," -f 2- )
