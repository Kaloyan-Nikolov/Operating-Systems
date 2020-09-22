Задача 22

Напишете скрипт, който получава 1 задължителен параметър - име на директория.
Скриптът трябва да намери и изведе на STDOUT за подадената директория и всяка 
нейна поддиректория по отделно името на файла с най-дълго име.

Забележка: В случай, че съществуват няколко файла с най-дълго име в директория, то те се сравняват лексикографски
и се извежда името на последния файл (foo.txt > bar.txt).

Примерна директорийна структура:

```
dir1
	dir2
		foo.txt
		barbar.txt
	dir3
		foobarbaz.txt
		dir4
			foobar.txt
			barfoo.txt
			qux.txt
textfile.txt
file.txt
info.txt
```

Примерен изход:

```
Longest filename in: ./dir1 is "textfile.txt"
Longest filename in: ./dir1/dir2 is "barbar.txt"
Longest filename in: ./dir1/dir3 is "foobarbaz.txt"
Longest filename in: ./dir1/dir3/dir4 is "foobar.txt"
```
