Задача 10

От всички файлове в home директорията на потребителя velin, изведете дълбочината на файл, който:
- има същия inode като този на най-скоро променения файл сред тях
- има минимална дълбочина

*Пояснение:* Под "дълбочина" се разбира дълбочината в дървото на файловата систева: 
например файлът /foo/bar/baz има дълбочина 3.

Решение 1:

```sh
find ~velin -samefile "$(find ~velin -type f -printf "%T@ %p\n" 2> /dev/null \
| sort -t " " -nk1 | tail -n 1 | cut -d " " -f 2)" 2> /dev/null | tr -cd "/\n" | awk '{print length}' | sort -n | head -n 1
```

Решение 2:

```sh
find ~velin -inum $(find ~velin -type f -printf "%T@ %i\n" 2> /dev/null | sort -n -k1 | tail -n 1 | cut -d ' ' -f 2) 2> /dev/null \
| tr -cd '/\n' | sort | head -1 | grep -o . | wc -l
```

Решение 3:

```sh
find / -type f -printf "%i %d %p\n" 2> /dev/null | egrep "^"$(find ~velin -type f -printf "%T@ %i\n" 2> /dev/null \
| sort -t ' ' -rnk1 | head -n 1 | cut -d ' ' -f 2- )" " | sort -t ' ' -nk2 | head -n 1 | awk '{print $2 }'
```
