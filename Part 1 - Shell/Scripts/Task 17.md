Задача 17

Нека съществува програма за моментна комуникация, която записва логове на разговорите в следния формат:
- има определена директория за логове - LOGDIR;
- в нея има директорийна структура от следния вид: LOGDIR/протокол/акаунт/приятел
като на всяко ниво може да има няколко екземпляра от съответния вид, т.е. няколко директории *протокол*, 
във всяка от тях може да има няколко директории *акаунт*, и във всяка от тях - няколко директории *приятел*;
- във всяка от директориите *приятел* може да има файлове с имена от вида yyyy-mm-dd-hh-mm-ss.txt - 
година-месец-ден-час-минути-секунди, спрямо това кога е започнал даден разговор;
- всеки такъв файл представлява лог на даден разговор със съответния приятел, като всяка разменена реплика
между вас е на отделен ред;
- даден идентификатор *приятел* може да се среща няколко пъти в структурата - през различни ваши акаунти
може да сте водили разговори със същия приятел.

Напишете скрипт, който приема задължителен позиционен аргумент - име на лог директория (LOGDIR).
Скриптът трябва да извежда тримата приятели, с които имате най-много редове комуникация глобално
(без значение протокол и аканут), и колко реда имате с всеки от тях.
