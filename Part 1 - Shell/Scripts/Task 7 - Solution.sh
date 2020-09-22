#!/bin/bash

if [ $(id -u) -ne 0 ]; then
   echo "You need root permissions!"
   exit 1
fi

while read USR HOMEDIR; do
    if [ -z $HOMEDIR ] || [ ! -d $HOMEDIR ]; then
        echo "User: $USR does not have a home directory!"
        continue
    fi

    if sudo -u $USR [ ! -w $HOMEDIR ]; then
        echo "User: $USR cannot write in the home directory!"
    fi
done < <(cat /etc/passwd | cut -d: -f1,6 | tr : ' ')

exit 0
