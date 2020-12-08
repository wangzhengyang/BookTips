[toc]

# switch内部变量
如果在某处一个带有初值的变量位于作用域之外，在另一处该变量位于作用域之内，则从前一处跳转到后一处的行为是非法的

# try语句块和异常处理
1. **throw 表达式**(throw expression)，异常检测部分使用throw表达式来表示它遇到了无法处理的问题，throw **引发**(raise)了异常
2. **try 语句块**(try block)，异常处理部分使用try语句块处理异常，try语句块以关键字try开始，并以一个或多个**catch子句**(catch clause)结束，try语句块中代码抛出的异常通常会被某个catch子句处理，因为catch子句"处理"异常，所以它们也被称作**异常处理代码**(exception handler)
3. 一套**异常类**(exception class)，用于在throw表达式和相关的catch子句之间传递异常的具体信息

## throw 表达式
`runtime_error`是标准库异常类型的一种，定义在`stdexcept`头文件

## try 语句块
```c++
try{
    program-statement
}catch (exception-declaration){
    handler-statements
}catch (exception-declaration){
    handler-statements
}//...
```
如果没有找到任何匹配的catch子句，程序转到名为`terminate`的标准库函数，该函数的行为与系统有关，一般情况下，执行该函数将导致程序非正常退出

## 标准异常
1. `exception`头文件定义了最通用的异常类`exception`，它只报告异常的发生，不提供任何额外信息
2. `stdexcept`头文件定义了几种常见的异常类
3. `new`头文件定义了`bad_alloc`异常类型
4. `type_info`头文件定义了`bad_cast`异常类型

`<stdexcept>`定义的异常类
|类|描述|
|-|-|
|exception|最常见的问题|
|runtime_error|只有在运行时才能检测出的问题|
|range_error|运行时错误；生成的结果超出了有意义的值域范围|
|overflow_error|运行时错误；计算上溢|
|underflow_error|运行时错误；计算下溢|
|logic_error|程序逻辑错误|
|domain_error|逻辑错误；参数对应的结果值不存在|
|ivalid_argument|逻辑错误；无效参数|
|length_error|逻辑错误；试图创建一个超出该类型最大长度的对象|
|out_of_range|逻辑错误；使用一个超出有效范围的值|

只能以默认初始化的方式初始化`exception`、`bad_alloc`和`bad_cast`对象，不允许为这些对象提供初始值
其他异常类型的行为则恰好相反：应该使用`string`对象或者C风格字符串初始化这些类型的对象，但是不允许使用默认初始化的方式，当创建此类对象时，必须提供初始值，该初始值含有错误相关信息

异常类型只定义一个名为`what`的成员函数，该函数没有任何参数，返回值是一个指向C风格字符串的`const char*`，该字符串的目的是提供关于异常的一些文本信息

