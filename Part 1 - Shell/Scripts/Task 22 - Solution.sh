#!/bin/bash

# find longest filename in every subdirectory and the directory itself

if [ $# -ne 1 ]; then
    echo "Usage: $0 <DIRNAME>"
    exit 1
fi

DIRNAME=$1
if [ ! -d ${DIRNAME} ] || [ ! -r ${DIRNAME} ]; then
    echo "Please enter readable dir!"
    exit 2
fi

FILENAMES=$(mktemp)
FILENAMES_LENGTH=$(mktemp)
while read CUR_DIRNAME; do
    find "${CUR_DIRNAME}" -maxdepth 1 -type f -printf "%f\n" 2> /dev/null > "${FILENAMES}"
    find "${CUR_DIRNAME}" -maxdepth 1 -type f -printf "%f\n" 2> /dev/null \
| awk '{print length}' > "${FILENAMES_LENGTH}"
    LONGEST_FILENAME="$(paste "${FILENAMES_LENGTH}" "${FILENAMES}" \
| tr '\t' ' ' | tr -s ' ' | sort -t ' ' -nk1 | tail -n 1 | cut -d ' ' -f 2-)"
    echo "Longest filename in: ${CUR_DIRNAME} is \"${LONGEST_FILENAME}\""
done < <(find "${DIRNAME}" -type d 2> /dev/null)

rm -- "${FILENAMES}"
rm -- "${FILENAMES_LENGTH}"

exit 0
