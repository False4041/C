# C++

Лабораторные по дисциплине «Алгоритмы и структуры данных». Вариант 12.

Зорькин Арсений Станиславович, ПИЖ-б-о-25-2 (1).  
Институт информационных технологий и кибербезопасности, кафедра «Программная инженерия».  
Северо-Кавказский федеральный университет, 2026.

В каждой папке: `main.cpp`, отчёт `README.md`, титульный лист `title.pdf` и пример результата. Для интерактивного ввода добавлен `input.txt`.
Для работ с уровнями выбран продвинутый вариант. В остальных выполнены вариант 12 и общие задания. Во всех 17 методичках есть вариант 12.

| № | Тема | Код | Титульник |
| --- | --- | --- | --- |
| [01](lab-01/README.md) | Композиция и агрегация классов. Статические члены | [main.cpp](lab-01/main.cpp) | [PDF](lab-01/title.pdf) |
| [02](lab-02/README.md) | Анализ сложности алгоритмов. Шаблоны классов | [main.cpp](lab-02/main.cpp) | [PDF](lab-02/title.pdf) |
| [03](lab-03/README.md) | Реализация динамического массива (аналог vector). Итераторы | [main.cpp](lab-03/main.cpp) | [PDF](lab-03/title.pdf) |
| [04](lab-04/README.md) | Односвязный и двусвязный списки | [main.cpp](lab-04/main.cpp) | [PDF](lab-04/title.pdf) |
| [05](lab-05/README.md) | Стек, очередь, дек, приоритетная очередь | [main.cpp](lab-05/main.cpp) | [PDF](lab-05/title.pdf) |
| [06](lab-06/README.md) | Рекурсивные алгоритмы. Простые сортировки | [main.cpp](lab-06/main.cpp) | [PDF](lab-06/title.pdf) |
| [07](lab-07/README.md) | Быстрая сортировка, сортировка слиянием, пирамидальная сортировка | [main.cpp](lab-07/main.cpp) | [PDF](lab-07/title.pdf) |
| [08](lab-08/README.md) | Двоичный поиск. Хеш-таблицы | [main.cpp](lab-08/main.cpp) | [PDF](lab-08/title.pdf) |
| [09](lab-09/README.md) | Бинарное дерево. Обходы. BST | [main.cpp](lab-09/main.cpp) | [PDF](lab-09/title.pdf) |
| [10](lab-10/README.md) | AVL-дерево. Префиксное дерево (Trie) | [main.cpp](lab-10/main.cpp) | [PDF](lab-10/title.pdf) |
| [11](lab-11/README.md) | Представление графов. DFS и BFS | [main.cpp](lab-11/main.cpp) | [PDF](lab-11/title.pdf) |
| [12](lab-12/README.md) | Алгоритм Дейкстры. Алгоритм Флойда-Уоршелла | [main.cpp](lab-12/main.cpp) | [PDF](lab-12/title.pdf) |
| [13](lab-13/README.md) | Алгоритмы Прима и Краскала. Минимальное остовное дерево | [main.cpp](lab-13/main.cpp) | [PDF](lab-13/title.pdf) |
| [14](lab-14/README.md) | Динамическое программирование. Задача о рюкзаке. Наибольшая общая подпоследовательность (LCS) | [main.cpp](lab-14/main.cpp) | [PDF](lab-14/title.pdf) |
| [15](lab-15/README.md) | Алгоритм Кнута-Морриса-Пратта. STL (обзор) | [main.cpp](lab-15/main.cpp) | [PDF](lab-15/title.pdf) |
| [16](lab-16/README.md) | Проектирование иерархии классов. Наследование и полиморфизм | [main.cpp](lab-16/main.cpp) | [PDF](lab-16/title.pdf) |
| [17](lab-17/README.md) | Реализация мини-проекта с использованием алгоритмов и структур данных | [main.cpp](lab-17/main.cpp) | [PDF](lab-17/title.pdf) |

## Сборка

Каждая лабораторная — отдельная программа. Откройте нужную папку и соберите её `main.cpp`. Используется только стандартная библиотека C++17.

GCC / MinGW:

```sh
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o main
./main
```

MSVC, Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /utf-8 /W4 /O2 main.cpp
main.exe
```

Если есть input.txt, в cmd: `main.exe < input.txt`. В PowerShell: `Get-Content input.txt | .\main.exe`.
Программы 16 и 17 записывают файлы в текущую папку.

[Проверки](CHECKS.md) · [Ссылки для сдачи](SUBMISSION.md)
