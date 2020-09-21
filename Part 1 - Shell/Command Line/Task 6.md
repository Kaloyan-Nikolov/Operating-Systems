Задача 6

Намерете имената на топ 5 файловете в текущата директория с най-много hardlinks.

Решение:

```sh
find . -maxdepth 1 -type f -printf "%n %f\n" 2> /dev/null | sort -t " " -rnk1 | head -n 5 | cut -d ' ' -f 2-
```
