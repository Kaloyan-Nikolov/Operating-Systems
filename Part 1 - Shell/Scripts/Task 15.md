Задача 15

Напишете скрипт, който получава 2 задължителни позиционни параметъра - директория и низ. 
Сред файловете в директорията би могло да има такива, чиито имена имат структурата vmlinuz-x.y.z-arch, където:

- vmlinuz е константен низ;
- тиретата "-" и точките "." присъстват задължително;
- х е число, version;
- у е число, major version;
- z е число, minor version;
- наредената тройка x.y.z формира глобалната версия на ядрото;
- arch е низ, архитектура (платформа), за която е съответното ядро.

Скриптът трябва да извежда само името на файла, намиращ се в подадената директория (но не и нейните поддиректории), който:
- спазва гореописаната структура;
- е от съответната архитектура спрямо параметъра - низ, подаден на скрипта;
- има най-голяма глобална версия.

Пример:

Съдържание на ./kern/:

```
vmlinuz-3.4.113-amd64
vmlinuz-4.11.12-amd64
vmlinuz-4.12.4-amd64
vmlinuz-4.19.1-i386
```

Извикване и изход:

```
$ ./task1.sh ./kern/ amd64
vmlinuz-4.12.4-amd64
```

**Забележка: Не е позволено използването на sort -V, --version-sort (natural sort of (version) numbers within text) !**