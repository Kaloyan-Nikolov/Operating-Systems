Задача 18

Напишете скрипт, който приема 2 позиционни аргумента - име на текстови файл и директория. 
Директорията не трябва да съдържа обекти, а текстовият файл е стенограма и всеки ред е в следния формат:

ИМЕ ФАМИЛИЯ (уточнения) : РЕПЛИКА

където:

- ИМЕ ФАМИЛИЯ присъстват задължително;
- ИМЕ и ФАМИЛИЯ се състоят само от малки/главни латински букви и тирета;
- (уточнения) не е задължително да присъстват;
- двоеточието ':' присъства задължително;
- РЕПЛИКА не съдържа знаци за нов ред;
- в стринга преди двоеточието ':' задължително има поне 1 интервал между ИМЕ и ФАМИЛИЯ;
- наличието на други интервали където и да е на реда е недефинирано.

Примерен входен файл:

```
John Lennon (The Beatles): Time you enjoy wasting, was not wasted.
Roger Waters: I'm in competition with myself and I'm losing.
John Lennon:Reality leaves a lot to the imagination.
Leonard Cohen:There is a crack in everything, that's how the light gets in.
```

Скриптът трябва да:

- създава текстов файл dict.txt в посочената директория, който на всеки ред да съдържа:
ИМЕ ФАМИЛИЯ;НОМЕР където:
  - ИМЕ ФАМИЛИЯ е уникален участник в стенограмата (без да се отчитат уточненията);
  - НОМЕР е уникален номер на участник, избран от вас.

- създава файл НОМЕР.txt в посочената директория, който съдържа всички (и само) редовете на дадения участник.
