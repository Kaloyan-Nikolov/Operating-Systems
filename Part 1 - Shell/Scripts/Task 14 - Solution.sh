#!/bin/bash
  
if [ $# -eq 0 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <dirname> [<filename>]"
    exit 1
fi

FILENAME=""
DIRNAME="${1}"
if [ ! -d "${DIRNAME}" ] || [ ! -r "${DIRNAME}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

if [ $# -eq 2 ];then
    FILENAME="${2}"
    if [ -f "${FILENAME}" ] && [ ! -w "${FILENAME}" ]; then
        echo "Please enter name of a new file or"
        echo "enter a writable file"
        exit 3
    fi
fi

if [ -z "${FILENAME}" ]; then
    find "${DIRNAME}" -type l -printf "%Y %p\n" 2> /dev/null | egrep "^[^NL] " | cut -d " " -f 2- | xargs -I {} stat -c "%N" {} | tr -d "'"

    RESULT="$(find "${DIRNAME}" -type l -print0 2> /dev/null | xargs -0 -I {} file {} | fgrep "broken" | wc -l)"
    echo "Broken symlinks: ${RESULT}"
    exit 0
fi

find "${DIRNAME}" -type l -printf "%Y %p\n" 2> /dev/null | egrep "^[^NL] " | cut -d ' ' -f 2-  | xargs -I {} stat -c "%N" {} | tr -d "'" >> "${FILENAME}"

RESULT="$(find "${DIRNAME}" -type l -print0 2> /dev/null | xargs -0 -I {} file {} | fgrep "broken" | wc -l )"
echo "Broken symlinks: ${RESULT}" >> "${FILENAME}"

exit 0
