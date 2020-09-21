Задача 4

Напишете серия от команди, които изтриват:

а) всички файлове в текущата директория и нейните директни поддиректории, които са с нулева дължина;

Решение 1:

```sh
find . -maxdepth 2 -type f -size 0 2>/dev/null -exec rm {} \;
```

Решение 2:

```sh
find . -maxdepth 2 -type f -size 0 -print0 2>/dev/null | xargs -0 -I {} rm {}
```

б) петте най-голями файла в home директорията на текущия потребител и нейните поддиректории.

Решение:

```sh
find "/home/$(whoami)/" -type f -printf "%s %p\n" 2> /dev/null | sort -t " " -rnk1 | head -n 5 | cut -d " " -f 2- | xargs -I {} rm {}
```
