#!/bin/bash
  
if [ $# -ne 1 ];then
    echo "Usage: $0 <dirname>"
    exit 1
fi

DIRNAME=$1
if [ ! -d "${DIRNAME}" ] || [ ! -r "${DIRNAME}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

# SOLUTION 1:
find "${DIRNAME}" -type l -printf "%Y %p\n" 2> /dev/null | egrep "^[NL]" | cut -d ' ' -f 2-

# SOLUTION 2:
#find "${DIRNAME}" -type l -print0 2> /dev/null | xargs -0 -I {} file {} | fgrep "broken" | awk -F ":" '{print $1}'

exit 0
