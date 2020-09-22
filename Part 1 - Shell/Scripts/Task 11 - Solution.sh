#!/bin/bash
  
if [ $# -eq 0 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <dirname> [<number>]"
    exit 1
fi

# $# -eq 1 or $# -eq 2

DIRNAME=$1
if [ ! -d "${DIRNAME}" ] || [ ! -r "${DIRNAME}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

if [ $# -eq 1 ]; then
    # all broken symlinks
    find "${DIRNAME}" -type l -printf "%Y %p\n" 2> /dev/null | egrep "^[NL] " | cut -d " " -f 2-
    exit 0
fi

# we have 2 params
NUMBER="$(echo $2 | egrep "^[+-]?[0-9]+$")"

if [ -z "${NUMBER}" ]; then
    echo "The second paramenter must be an integer!"
    exit 3
fi

while read CUR_LINE; do
    if [ "$(echo "${CUR_LINE}" | cut -d " " -f 1)" -ge "${NUMBER}" ]; then
        echo "${CUR_LINE}" | cut -d " " -f 2-
    fi
done < <(find "${DIRNAME}" -type f -printf "%n %p\n" 2> /dev/null)

exit 0
