#!/bin/bash

if [ $# -ne 0 ]; then
    echo "All entered arguments will be ignored!"
fi

MAX_SUM=0
SMALLEST_NUM=0
CNT=0
while read NUMBER; do
    CNT=1
    ABS_NUM="$(echo $NUMBER | sed -E "s/-//" )"
    CUR_SUM="$(echo $ABS_NUM | sed -E "s/(.)/\1+/g" | sed -E "s/\+$//" | bc )"
    if [ $CUR_SUM -gt $MAX_SUM ]; then
        MAX_SUM=$CUR_SUM
        SMALLEST_NUM=$NUMBER
    fi
    if [ $CUR_SUM -eq $MAX_SUM ] && [ $NUMBER -lt $SMALLEST_NUM ]; then
        SMALLEST_NUM=$NUMBER
    fi
done < <(cat |  egrep "^[+-]?[0-9]+$")

if [ $CNT -eq 1 ]; then
    echo "Smallest number with biggest sum of digets is: $SMALLEST_NUM"
else
    echo "There are not any integers entered!"
fi

exit 0
