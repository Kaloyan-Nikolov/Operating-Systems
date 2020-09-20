#!/bin/bash
  
if [ $# -ne 2 ]; then
    echo "Usage: $0 <filename1> <filename2>"
    exit 1
fi

if [ ! -f $1 ] || [ ! -r $1 ] || [ ! -f $2 ] || [ ! -r $2 ];then
    echo "Please enter readable files!"
    exit 2
fi

if [ "$(cat "$1" | wc -l)" -gt "$(cat "$2" | wc -l)" ]; then
    LONGER_FILE="${1}"
else
    LONGER_FILE="${2}"
fi

AUTHOR="$(cat "${LONGER_FILE}" | head -n 1 | cut -d ' ' -f 2)"
cat "${LONGER_FILE}" | cut -d ' ' -f 4- | sort > "${AUTHOR}.songs"

exit 0
