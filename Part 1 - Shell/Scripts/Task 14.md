Задача 14

Напишете shell скрипт, който получава задължителен първи позиционен параметър - директория и
незадължителен втори - име на файл. Скриптъп трябва да намира в подадената директория и нейните поддиректории
всички symlinks и да извежда (при подаден аргумент файл - добавяйки към файла, а ако не е - на стандартния изход)
за тях следната информация:

- ако destination-a съществува - името на symlink-а -> името на destination-а
- броя symlinks, чиито destination не съществува

Примерен изход:

```
lbaz -> /foo/bar/baz
lqux -> ../../../qux
lquux -> /foo/quux
Broken symlinks: 34
```
