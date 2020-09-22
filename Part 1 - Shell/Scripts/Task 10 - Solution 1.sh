#!/bin/bash
  
#for every homedir we find its owner and most recent file
#we find the most recent owner from all files

RECENT_FILES=$(mktemp)
CUR_ROW="";
while read CUR_USER CUR_HOME; do
    CUR_ROW="$(find "${CUR_HOME}" -type f ! -name ".*" -printf "%T@ file: %f\n" 2> /dev/null | sort -nk1 | tail -n 1)"
    if [ -n "$(echo ${CUR_ROW})" ]; then
        echo "${CUR_ROW} , and the home dir is: ${CUR_USER}" >> ${RECENT_FILES}
    fi
done < <(cat /etc/passwd | cut -d: -f1,6 | tr : ' ')

if [ -z "$(cat ${RECENT_FILES})" ]; then
    echo "No files found!"
else
    cat "${RECENT_FILES}" | sort -nk1 | tail -n 1 | cut -d ' ' -f 2-
fi

rm -- "${RECENT_FILES}"
exit 0
