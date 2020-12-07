[toc]

# 基本内置类型
## 算数类型
|类型|含义|最小尺寸|
|-|-|-|
|bool|布尔类型|未定义|
|char|字符|8位|
|wchar_t|宽字符|16位|
|char16_t|Unicode字符|16位|
|char32_t|Unicode字符|32位|
|short|短整型|16位|
|int|整型|16位|
|long|长整型|32位|
|long long|长整型|64位|
|float|单精度浮点数|6位有效数字|
|double|双精度浮点数|10位有效数字|
|long double|扩展精度浮点数|10位有效数字|
C++语言规定一个int至少和一个short一样大，一个long至少和一个int一样大，一个long long至少和一个long一样大，其中，数据类型long long是在C++11中新定义的

## 类型转换
1. 把一个非布尔类型的算术值赋给布尔类型时，初始值为0则结果为false，否则结果为true
2. 把一个布尔值赋给非布尔类型时，初始值为false则结果为0，初始值为true则结果为1
3. 把一个浮点数赋给整数类型时，进行近似处理，结果值将仅保留浮点数中小数点之前的部分
4. 把一个整数赋给浮点类型时，小数部分记为0，如果该整数所占的空间超过了浮点类型的容量，精度可能有损失
5. 当赋给无符号类型一个超出它表示范围的值时，结果是初始值对无符号类型表示数值总数取模后的余数
6. 当赋给带符号类型一个超出它表示范围的值时，结果是未定义的(undefined)，此时，程序可能继续工作、可能崩溃，也可能生出垃圾数据

## 指定字面值类型
### 字符和字符串字面值
|前缀|含义|类型|
|-|-|-|
|u|Unicode 16字符|char16_t|
|U|Unicode 32字符|char32_t|
|L|宽字符|wchar_t|
|u8|UTF-8(仅用于字符串字面常量)|char|
### 整型字面值
|后缀|最小匹配类型|
|-|-|
|u or U| unsigned|
|l or L|long|
|ll or LL|long long|
### 浮点型字面值
|f or F|float|
|l or L|long double|

# 变量
|C++关键字|
|-|
|`alignas` `continue` `friend` `register` `true` `alignof` `decltype` `goto` `reinterpret_cast` `try` `asm` `default` `if` `return` `typedef` `auto` `delete` `inline` `short` `typeid` `bool` `do` `int` `signed` `typename` `break` `double` `long` `sizeof` `union` `case` `dynamic_cast` `mutable` `static` `unsigned` `catch` `else` `namespace` `static_assert` `virtual` `char16_t` `explicit` `noexcept` `struct` `void` `char32_t` `export` `nullptr` `switch` `volatile` `class` `extern` `operator` `template` `wchar_t` `const` `false` `private` `this` `while` `constexpr` `float` `protected` `thread_local` `const_cast` `for` `public` `throw`|

|C++操作符替代名|
|-|
|`and` `bitand` `compl` `not_eq` `or_eq` `xor_eq` `and_eq` `bitor` `not` `or` `xor`|

# 复合类型
## 引用
使用术语"引用"时，指的是"左值引用"

一般在初始化变量时，初始值会被拷贝到新建的对象中，然而定义引用时，程序把引用和它的初始值绑定(bind)在一起，而不是将初始值拷贝给引用，一旦初始化完成，引用将和它的初始值对象一直绑定在一起，因为无法令引用重新绑定到另外一个对象，因此引用必须初始化

# `const`限定符
默认情况下，`const`对象仅在文件内有效
编译器将在编译过程中把用到该变量的地方都替换成对应的值
如果想在多个文件之间共享`const`对象，必须在变量的定义之前添加`extern`关键字
## `const` 的引用
常量引用不能被用作修改它所绑定的对象

## `constexpr`和常量表达式
将变量声明为`constexpr`类型以便编译器来验证变量的值是否是一个常量表达式

# 处理类型
## 类型别名
使用`typedef` `using`来定义类型别名
## `auto`类型说明符
`auto`类型说明符，让编译器去分析表达式所属的类型
## `decltype`类型指示符
`decltype`的作用是选择并返回操作数的数据类型，在此过程中，编译器分析表达式并得到它的类型，却不实际计算表达式的值



