#!/bin/bash

temp=$(mktemp)
cat | egrep "^[-+]?[0-9]+$" > "$temp"

if [ "$(cat "$temp" | wc -l )" -eq 0 ];then
    echo
    echo "Please enter one integer at least"
    exit 1
fi

LARGEST_SUM=0
CUR_RESULT=0

while read NUM; do
    SUM=0
    NUM_COPY="$NUM"
    NUM="$( echo $NUM | sed -E "s/-//" )"
    while [ "$NUM" -ne 0 ]; do
        REMAINDER=$(expr $NUM % 10 )
        SUM=$( expr $SUM + $REMAINDER )
        NUM=$( expr $NUM / 10 )
    done

    if [ $SUM -gt $LARGEST_SUM ];then
        LARGEST_SUM=$SUM
        CUR_RESULT=$NUM_COPY
    elif [ $SUM -eq $LARGEST_SUM ] && [ $NUM_COPY -lt $CUR_RESULT ];then
        CUR_RESULT=$NUM_COPY
    fi

done < <(cat "$temp")

echo "answer: $CUR_RESULT"

rm -- "$temp"

exit 0
