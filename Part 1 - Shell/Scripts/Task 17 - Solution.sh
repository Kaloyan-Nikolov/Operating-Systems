#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <dirname>"
    exit 1
fi

DIRNAME=$1

if [ ! -d "${DIRNAME}" ] || [ ! -r "${DIRNAME}" ]; then
    echo "Please enter readable directory!"
    exit 2
fi

LINES_FRIEND=$(mktemp)

while read CUR_FRIEND; do
    #SOLUTION 1:
    LINES="$(find "${DIRNAME}" -mindepth 4 -maxdepth 4 -type f 2> /dev/null  | egrep "/${CUR_FRIEND}/"| xargs -I {} cat {} | wc -l)"

    #SOLUTION 2:
    #LINES="$(find "${DIRNAME}" -mindepth 4 -maxdepth 4 -type f 2> /dev/null | egrep "/${CUR_FRIEND}/"| xargs -n 1 -I {} wc -l {} | awk '{sum+=$1}END{print sum}')"
    
    echo "$LINES $CUR_FRIEND" >> ${LINES_FRIEND}
done < <(find "${DIRNAME}" -mindepth 3 -maxdepth 3 -type d -printf "%f\n" 2> /dev/null | sort | uniq)

cat ${LINES_FRIEND} | sort -rnk1 | head -n 3

rm -- "${LINES_FRIEND}"

exit 0
