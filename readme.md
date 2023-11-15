# Проект "Детекция тропических циклонов"
## Зависимости:
python, gcc/clang, PIL, ultralytics
Данные зависимости можно удовлетворить, используя pip3.
## Файлы репозитория:
runs/ - предыдущие запуски тренировки модели.
datasets/ - датасеты, доступные yolo; datasets/custom/ имеет readme.md с описанием.
test.jpg - тестовое изображение.
demo.jpg - результат работы на тестовом изображении (человекочитаемый формат).
demo.txt - результат работы на тестовом изображении (формат задания).
solution.py - файл решения.
На вход при запуске решения следует указать имя файла, над котором решение должно произвести работу (например, test.jpg) или TRAIN для обучения.
Вывод работы скрипта solution.py будет находится в out.txt (в формате задания) и out.jpg (в человекопонимаемом формате).
Перед проведением обучения необходимо указать актуальные пути в файле datasets/custom/data.yaml .
