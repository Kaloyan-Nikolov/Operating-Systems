#!/bin/bash

temp=$(mktemp)
cat | egrep "^[-+]?[0-9]+$" | sort -n > "$temp" 

if [ "$(cat "$temp" | wc -l )" -eq 0 ];then
    echo
    echo "Please enter one number at least!"
    exit 1
fi

BIGGEST="$(cat "$temp" | tail -n 1)"
SMALLEST="$(cat "$temp" | head -n 1)"

if [ $BIGGEST -eq $SMALLEST ];then
    echo
    echo $BIGGEST
else
    ABS_SMALLEST="$( echo $SMALLEST | sed -E "s/-//")"
    ABS_BIGGEST="$( echo $BIGGEST | sed -E "s/-//")"
    if [ $ABS_SMALLEST -eq $ABS_BIGGEST ];then
        echo
        echo $BIGGEST
        echo $SMALLEST
    elif [ $ABS_SMALLEST -lt $ABS_BIGGEST ]; then
        echo
        echo $BIGGEST
    else
        echo
        echo $SMALLEST
    fi
fi

rm -- "$temp"

exit 0
