Задача 3

Файлове във вашата home директория съдържат информация за музикални албуми и имат специфична структура.
Началото на всеки ред е годината на издаване на албума, а непосредствено, след началото на всеки ред, следва името на изпълнителя на песента.
Имената на файловете се състоят от една дума, която съвпада с името на изпълнителя.

Примерно съдържание на файл с име "Bonnie":

2005г. Bonnie - "God Was in the Water" (Randall Bramblett, Davis Causey) - 5:17\
2005г. Bonnie - "Love on One Condition" (Jon Cleary) - 3:43\
2005г. Bonnie - "So Close" (Tony Arata, George Marinelli, Pete Wasner) - 3:22\
2005г. Bonnie - "Trinkets" (Emory Joseph) - 5:02\
2005г. Bonnie - "Crooked Crown" (David Batteau, Maia Sharp) - 3:49\
2005г. Bonnie - "Unnecessarily Mercenary" (Jon Cleary) - 3:51\
2005г. Bonnie - "I Will Not Be Broken" - "Deep Water" (John Capek, Marc Jordan) - 3:58

Да се напише скрипт, приемащ 2 параметъра, които са имената на такива файлове от вашата home директория.
Скриптът сравнява кой от двата файла има повече на брой редове, съдържащи неговото име (на файла).
За файлът победител изпълнете следните действия:

- извлечете съдържанието му, без годината на издаване на албума и без имено на изпълнителя;
- сортирайте лексикографски извлеченото съдържание и го запишете във файл с име **"изпълнител.songs"**

Примерен изходен файл (с име Bonnie.songs):

"Crooked Crown" (David Batteau, Maia Sharp) - 3:49\
"God Was in the Water" (Randall Bramblett, Davis Causey) - 5:17\
"I Will Not Be Broken" - "Deep Water" (John Capek, Marc Jordan) - 3:58\
"Love on One Condition" (Jon Cleary) - 3:43\
"So Close" (Tony Arata, George Marinelli, Pete Wasner) - 3:22\
"Trinkets" (Emory Joseph) - 5:02\
"Unnecessarily Mercenary" (Jon Cleary) - 3:51
