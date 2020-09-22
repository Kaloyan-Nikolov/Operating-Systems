#!/bin/bash
  
if [ $# -ne 2 ]; then
    echo "Usage: ./myscript.sh dirname string"
    exit 1
fi

DIRNAME=$1
STR=$2

if [ ! -d "${DIRNAME}" ] || [ ! -r "${DIRNAME}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

find "${DIRNAME}" -maxdepth 1 -type f -printf "%f\n"  2> /dev/null \
| egrep "vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${STR}" | sed -E "s/-/ /g" | sed -E "s/\./ /g" \
| sort -t ' ' -rnk 2,2 -k 3,3 -k 4,4 | head -n 1 | sed "s/vmlinux /vmlinux-/" \
| sed "s/ ${STR}/-${STR}/" | sed "s/ /./g" 

exit 0
