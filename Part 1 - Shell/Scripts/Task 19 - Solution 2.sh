#!/bin/bash
  
if [ $# -ne 2 ];then
    echo "Usage: $0 <a.csv> <b.scv>"
    exit 1
fi

if [ ! -f "$1" ] ;then
    echo "Please enter readable file!"
    exit 2
fi

INPUT="$1"
RESULT="$2"
RES=$(mktemp)

#here we want to know what to include in RESULT
while read LINE; do
    CUTLINE="$( echo "$LINE" | cut -d ',' -f 2- )"
    if egrep -q ",$CUTLINE$" < <(cat "$RES" 2> /dev/null);then
        continue;
    else #it has to be added
        echo "$LINE" >> "$RES"
    fi

done < <( cat "$INPUT" | sort -t "," -nk1)

#now we know
echo -n > "$RESULT"
while read LINE; do
    if cat "$RES" 2> /dev/null | egrep -q "^$LINE$" ;then
        echo "$LINE" >> "$RESULT"
    fi
done < <( cat "$INPUT")

rm -- "$RES"

exit 0
