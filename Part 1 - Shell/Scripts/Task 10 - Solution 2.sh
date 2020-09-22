#!/bin/bash
  
ALL_USERS_HOME="$(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ')"
LAST_FROM_ALL=0
while read USER HOMEE ;do

    if [ ! -d "$HOMEE" ]; then
        continue
    fi

    if [ ! -r "$HOMEE" ]; then
        continue
    fi

    LAST_MODIF_FILE="$(find "$HOMEE" -type f ! -name ".*" -printf "%T@ %f\n" 2>/dev/null | sort -t ' ' -nk1 | tail -n 1 )"
    if [ -z "$LAST_MODIF_FILE" ]; then
        continue
    fi

    LAST_FILE_TIME="$(echo $LAST_MODIF_FILE | awk -F "." '{print $1}')"

    if [ $LAST_FILE_TIME -gt $LAST_FROM_ALL ]; then
        ANSWER_USER=$USER
        ANSWER_FILE="$(echo $LAST_MODIF_FILE | awk '{print $2}')"
    fi

done < <(echo "$ALL_USERS_HOME")

echo " user: $ANSWER_USER file: $ANSWER_FILE"

exit 0
