#!/bin/bash
  
if [ $# -ne 3 ]; then
    echo "Usage: ./myscript SRC DST STR"
    exit 1
fi

SRC=$1
DST=$2
STR=$3

if [ ! -d "${SRC}" ] || [ ! -r "${SRC}" ]; then
    echo "Please enter readable directory for SOURCE!"
    exit 2
fi

if [ ! -d "${DST}" ] || [ ! -w "${DST}" ]; then
    echo "Please enter writable directory for DESTINATION!"
    exit 3
fi

if [ "$(find "${DST}" -mindepth 1 | wc -l)" -ne 0 ]; then
    echo "Please enter empty directory as DESTINATION!"
    exit 4
fi

if [ $(id -u) -ne 0]; then
   echo "You need root permissions!"
   exit 5
fi

while read filename; do
    without_src="$(echo "${filename}" | sed -E "s/.*${SRC}//")"
    dirs_to_make="$(echo $without_src | rev | cut -d "/" -f 2- | rev)"
    mkdir -p "${DST}${dirs_to_make}"
    mv "$filename" "${DST}${without_src}"
done < <(find "${SRC}" -type f -name "*${STR}*" 2> /dev/null)

exit 0
