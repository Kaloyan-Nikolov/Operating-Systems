#!/bin/bash
  
N=10

if [ $# -eq 0 ]; then
    echo "Usage: ./myscript.sh [-n N] FILE..."
    exit 1
fi

if [ "$1" = "-n" ]; then
    N="$2"
    shift 2
fi

ANSWER=$(mktemp)
for CUR_FILE in "$@" ; do
    while read LINE; do
        echo "${LINE}" | sed -E "s/([0-9]+-[0-9]+-[0-9]+ [0-9]+:[0-9]+:[0-9]+ )/\1${CUR_FILE} /" >> "${ANSWER}"
    done < <(cat "$CUR_FILE" | sort -t "-" -nk1 | tail -n "${N}")
    # this is the same as:
    # cat "$CUR_FILE" | sort -t "-" -nk1 | tail -n "${N}" | sed -E "s/([0-9]+-[0-9]+-[0-9]+ [0-9]+:[0-9]+:[0-9]+ )/\1${CUR_FILE} /" >> "${ANSWER}"
done

cat "${ANSWER}" | sort -t "-" -nk1

rm -- "${ANSWER}"

exit 0
