#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: ./myscript.sh dirname string"
    exit 1
fi

DIR=$1
STR=$2

if [ ! -d "${DIR}" ] || [ ! -r "${DIR}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

temp=$(mktemp)
find "$DIR" -maxdepth 1 -type f -printf "%f\n"  2>/dev/null | egrep "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${STR}$" > "$temp"

ENDING="$(cat $temp | tr -cd "0-9\n.-" | cut -d '-' -f 2 | sort -t '.' -n -r -k 1,1 -k 2,2 -k 3,3 | head -n 1 )"

grep "$ENDING" < $temp

rm -- $temp

exit 0
