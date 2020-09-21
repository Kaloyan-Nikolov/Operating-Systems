Задача 8

При подреждане в нарастващ ред на числовите потребителски идентификатори (UID) на
акаунтите, дефинирани в системата, 5-ят акаунт е от групата, запазена за акаунти от
специалност СИ. 

Изведете списък с имената (име и фамилия) и home директориите на всички акаунти от 
специалност СИ, подредени по факултетен номер.

Примерно съдържание на файла:

```
s61988:x:1219:504:Stoian Genchev,SI,2,5:/home/SI/s61988:/bin/bash
s81430:x:1234:503:Iordan Petkov, KN, k2, g7:/home/KN/s81430:/bin/bash
s61807:x:1248:504:Elica Venchova:/home/SI/s61807:/bin/bash
s62009:x:1254:504:Denitsa Dobreva, 2, 6:/home/SI/s62009:/bin/bash
s61756:x:1258:504:Katrin Kartuleva, SI, 4, 1:/home/SI/s61756:/bin/bash
s855287:x:1195:504:Vaska Kichukova,SI,2,5:/home/SI/s855287:/bin/bash
```

Примерен изход:

```
Katrin Kartuleva:/home/SI/s61756
Elica Venchova:/home/SI/s61807
Stoian Genchev:/home/SI/s61988
Denitsa Dobreva:/home/SI/s62009
Vaska Kichukova:/home/SI/s855287
```

Решение 1:

```sh
cat /etc/passwd | egrep "^[^:]+:[^:]+:[^:]+:$(cat /etc/passwd | cut -d ":" -f 3,4 \
| sort -t ":" -nk1 | head -n 5 | tail -n 1 | cut -d ":" -f 2):" | cut -d ":" -f 5,6 | sed -E "s/,.*:/:/" | sort -t "/" -k4
```

Решение 2:

```sh
cat /etc/passwd | egrep "^s[0-9]+:x:[0-9]+:"$(cat /etc/passwd | sort -t ":" -nk3 \
| head -n 5 | tail -n 1 | cut -d ":" -f 4)":" | awk 'BEGIN{ FS="[:,]"}{print $5":"$(NF-1)}' | sort -t "/" -k4
```
