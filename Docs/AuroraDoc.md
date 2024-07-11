# Предисловие
Документация, которую вы держите в руках, предназначена для версии MVP 0.2. Она включает в себя базовые возможности и синтаксис языка, а также примеры кода. Тем не менее, следует учитывать, что многое еще может измениться, улучшиться или быть добавлено в будущих версиях.
# Ведение
## Язык Aurora
Язык Aurora - это строго типизированный, компилируемый язык программирования высокого уровня. Он предназначен для написания высокоэффективных приложений, которые компилируются в специальный байт-код, исполняемый виртуальной машиной Aurora Environment (AE). Основные характеристики языка Aurora включают его строгую типизацию, компиляцию и поддержку разнообразных типов данных и конструкций управления.
## О Aurora Environment
Aurora Environment (AE) - это виртуальная машина, исполняющая байт-код Aurora Byte Code (Aurora BC). Она поддерживает 64-разрядную адресацию памяти, что позволяет работать с большими объемами данных и создавать мощные приложения.
## Почему стоит использовать Aurora?
1. Высокая производительность: Компиляция в байт-код позволяет программам Aurora работать быстро и эффективно.
2. Безопасность типов: Строгая типизация предотвращает множество ошибок времени выполнения.
3. Простота запуска: Для выполнения программ достаточно установить AE на ваше устройство.
4. Модульность и повторное использование кода: Поддержка классов и интерфейсов помогает структурировать код и использовать его повторно.
# Основы языка
## Структура программы
Каждая программа на Aurora начинается с первой строки кода. Организация и правила написания структурированы таким образом, чтобы код был легко читаем и поддавался анализу.
## Ключевые слова
Вот основные ключевые слова, используемые в Aurora:
- **const**: Объявление константы, значение которой не может измениться.
- **void**: Указание на отсутствие возвращаемого значения функции.
- **char**: Символьный тип данных (8 бит).
- **byte**, **int**, **long**, **float**, **double**: Типы данных для работы с числами.
- **class**, **interface**: Ключевые слова для объявления классов и интерфейсов.
- **for**, **while**, **do**: Циклические операторы.
## Блоки кода
Блоки кода в Aurora отделяются табуляцией или пробелами, что делает код легко читабельным и структурированным. Пример функции:
``` Aurora
void myFunc() 
    int result = 10 
    return result
```
## Типы данных
Имена переменных могу содержать буквы, цифры и нижнее подчеркивание 
Не должно начинаться с цифры и нижнего подчеркивания.
### Простые типы
- **char** - 8 бит, символ.
- **short** - 16 бит, целое число.
- **int** - 32 бит, целое число.
- **long** - 64 бит, целое число.
- **float** - Число с плавающей точкой одинарной точности.
- **double** - Число с плавающей точкой двойной точности.

Эти типы могут автоматически преобразовываться в типы большего размера. Например:
```Aurora
char c = 'A' 
int i = c // Преобразование char в int происходит автоматически
```
Линия автоматического преобразования.

char->short->int->long->float->double
### Пользовательские типы
В данный момент нет поддержки пользовательских типлв
### Ключевое слово const
Ключевое слово const используется для объявления неизменяемых переменных
```Aurora
const int maxSize = 100
maxSize = 200 // Ошибка! Нельзя изменить значение константы
```
## Операторы
Поддерживаемые операции:

- Арифметические: `+`, `-`, `/`, `*`
- Сравнения: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Присваивания: `+=`, `-=`, `*=`, `/=`
### Оператор " , "
Этот оператор объединяет два выражения в одно. Это полезно для одновременного выполнения нескольких операций:
```Aurora
int a = 5, b = 10, c = a + b
```
### Оператор " ... "
Оператор ... ничего не делает, используется для заполнения:
```Aurora
while condition == true
    ...
```
## ## Комментарии
Комментарии пишутся через // и не учитываются при компиляции:
```Aurora
// Это комментарий
int a = 10 // Это также комментарий
```
# Управления программой
## Условные операторы
### Оператор if
Оператор if выполняет блок кода, если условие верно:
```Aurora
if condition
	// Ваш код здесь
```
### Оператор else
Оператор else выполняет блок кода, если условие в if ложно:
```Aurora
if condition
	...
else
	// Ваш код здесь
```
### Конструкция else if
Конструкция else if позволяет проверять дополнительные условия, если предыдущее условие ложно
```Aurora
if (condition)
	...
else if (condition)
	// Ваш код здесь 
else
	...

```

## Циклы
### While
Цикл while выполняет блок кода, пока условие истинно:
```Aurora
while  a < b
	// Your code here
```
### Do while
Цикл do while выполняет блок кода хотя бы один раз, а затем повторяет его, пока условие истинно:
```Aurora
do
	// Ваш код здесь
while a < b
```
### For
Цикл for имеет три части: инициализация, условие и оператор. Пример:
```Aurora
for (int i = 0; i < 10; i++)
    // Ваш код здесь
```
- **init**: Выполняется один раз при входе в цикл.
- **condition**: Проверяется перед каждой итерацией.
- **operator**: Выполняется после каждой итерации.
## Операторы управления

### Break
Прерывает выполнение цикла:
```Aurora
for (int i = 0; i < 10; i++)
    if (i == 5)
        break
```
### Continue
Пропускает текущую итерацию и переходит к следующей:
```Aurora
for (int i = 0; i < 10; i++)
    if (i % 2 == 0)
        continue
    // Этот код будет выполнен только для нечетных i
### Return
```
Завершает выполнение функции и возвращает значение, если оно указано:
```Aurora
int sum(int a, int b)
    return a + b
```
# Функции

## Определение функции

Функции объявляются с указанием возвращаемого типа и списка аргументов:
```Aurora
ТИП_ВОЗВРАЩАЕМОГО_ЗНАЧЕНИЯ ИМЯ(АРГУМЕНТЫ)
    ТЕЛО_ФУНКЦИИ
```
Пример:
```Aurora
int sum(int a, int b)
    return a + b
```
Имена функций могут содержать буквы, цифры и нижнее подчеркивание, но не должны начинаться с цифры или подчеркивания.
Функции доступны везде в коде:
Например:
```Aurora
int return10()
	return 10
```
Объявленная функция видна везде в коде
```Aurora
void myFunc1()
    // Ваш код здесь
    return

myFunc1() // Верно
int result = myFunc2(10) // Тоже верно

int myFunc2(int value)
    int result = value * 2
    return result
```
У функции обязательно должно быть тело:
```Aurora
void myFunc1() // Ошибка!

int i = 1
```
## Рекурсия
Функции могут вызывать себя, позволяя реализовать рекурсию:
```Aurora
int factorial(int n)
    if (n <= 1)
        return 1
    else
        return n * factorial(n - 1)
```

```
class B : A
    init(sperma s)
    int i
    const j = 0
private:
    int j
    void myFunc()
       ...
protected:
    double jopa

```