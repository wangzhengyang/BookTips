[toc]

# 定义抽象数据类型
## 引入`this`
成员函数通过一个名为`this`的额外的隐式参数来访问调用它的那个对象
## 引入`const`成员函数
`const`的作用是修改隐式`this`指针的类型，默认情况下，`this`的类型是指向类类型非常量版本的常量指针
## 构造函数
如果类没有显示地定义构造函数，那么编译器会隐式地定义一个默认构造函数，合成的默认构造函数将按照如下规则初始化类的数据成员：
1. 如果存在类内的初始值，用它来初始化成员
2. 否则，默认初始化该成员

### `=default`的含义
如果需要默认的行为，可以通过在参数列表后面写上`=default`来要求编译器生成构造函数
### 构造函数初始值列表
为创建的对象的一个或几个数据成员赋初值

## 拷贝、赋值和析构
如果不主动定义这些操作，则编译器将会合成它们，一般来说，编译器生成的版本将对对象的每个成员执行拷贝、赋值和销毁操作

# 访问控制和封装
1. 定义在`public`说明符之后的成员在整个程序内可被访问，`public`成员定义类的接口
2. 定义在`private`说明符之后的成员可以被类的成员函数访问，但是不能被使用该类的代码访问，`private`部分封装了(即隐藏了)类的实现细节

### 使用class或struct关键字
类可以在它的第一个访问说明符之前定义成员，对这种成员的访问权限依赖于类定义的方式，如果使用`struct`关键字，则定义在第一个访问符之前的成员是`public`的；相反，如果使用`class`关键字，则这些成员是`private`的

## 友元
类可以允许其他类或者函数访问它的非公有成员，方式是令其他类或者函数称为它的**友元**(friend)

# 类的其他特性
## 类成员
### 定义一个类型成员
使用`typedef`定义一个类型成员
用来定义类型的成员必须先定义后使用
### 令成员作为内联函数
定义在类内部的成员函数时自动`inline`的
### 可变数据成员
即使在一个`const`成员函数内，希望修改类的某个数据成员，可以通过在变量的声明中加入`mutable`关键字做到
## 返回***this**的成员函数
### 从`const`成员函数返回*this
一个`const`成员函数如果以引用的形式返回*this，那么它的返回类型将是常量引用

# 构造函数
## 构造函数初始值列表
如果没有在构造函数的初始值列表中显式地初始化成员，则该成员将在构造函数体之前执行默认初始化
### 构造函数的初始值有时必不可少
如果成员是`const`或者是引用的话，必须将其初始化，类似的，当成员属于某种类类型且该类没有定义默认构造函数时，必须将这个成员初始化

在很多类中，初始化和赋值的区别事关底层效率问题：前者直接初始化数据成员，后者则先初始化再赋值
### 成员初始化顺序
成员的初始化顺序与它们在类定义中的出现顺序一致
最好令构造函数初始值的顺序与成员声明的顺序保持一致，而且如果可能的话，尽量避免使用某些成员初始化其他成员
### 默认实参和构造函数
如果一个构造函数为所有参数都提供了默认实参，则它实际上也定义了默认构造函数

## 委托构造函数
委托构造函数使用它所属类的其他构造函数执行它自己的初始化过程
## 默认构造函数的作用
默认初始化在以下情况下发生：
1. 当在块作用域内不使用任何初始值定义一个非静态变量或者数组时
2. 当一个类本身含有类类型的成员且使用合成的默认构造函数时
3. 当类类型的成员没有在构造函数初始值列表中显式地初始化时
值初始化再以下情况下发生：
1. 在数组初始化的过程中如果提供的初始值数量小于数组的大小时
2. 当不使用初始值定义一个局部静态变量时
3. 当通过书写形如T()的表达式显式地请求值初始化时，其中T是类型名

类必须包含一个默认构造函数以便在上述情况下使用

## 隐式的类类型转换
### 只允许一步类类型转换
编译器只会自动地执行一步类型转换
### 抑制构造函数定义的隐式转换
将构造函数声明为`explicit`加以阻止

关键字`explicit`只对一个实参的构造函数有效，需要多个实参的构造函数不能用于执行隐式转换，所以无需将这些构造函数指定为`explicit`

只能在类内声明构造函数时使用`explicit`关键字，在类外部定义时不应重复

### `explicit`构造函数只能用于直接初始化
发生隐式转换的一种情况是当执行拷贝形式的初始化时(使用=)，此时，只能使用直接初始化而不能使用`explicit`构造函数

### 为转换显示地使用构造函数
尽管编译器不会将`explicit`的构造函数用于隐式转换过程，但是可以使用这样的构造函数显式地强制进行转换，使用`static_cast`执行显式转换

### 标准库中含有显式构造函数的类
1. 接受一个单参数的`const char*`的string构造函数不是`explicit`的
2. 接受一个容量参数的`vector`构造函数是`explicit`的

## 聚合类
当一个类满足如下条件，则认为它是聚合的：
1. 所有成员都是`public`的
2. 没有定义任何构造函数
3. 没有类内初始值
4. 没有基类，也没有`virtual`函数

使用一个花括号括起来的成员初始值列表，并用它初始化聚合类的数据成员

# 类的静态成员
### 声明静态成员
在成员的声明之前加上关键字`static`使得其与类关联起来，静态成员可以是`public`的或`private`的，静态数据成员的类型可以是常量、引用、指针、类类型等

类的静态成员存在于任何对象之外，对象中不包含任何与静态数据成员有关的数据

类似的，静态成员函数也不与任何对象绑定在一起，它们不包含`this`指针，作为结果，静态成员函数不能声明成`const`的，而且也不能再`static`函数体内使用`this`指针，这一限制即适用于`this`的显式使用，也对调用非静态成员的隐式使用有效

### 使用类的静态成员
1. 使用作用域运算符直接访问静态成员
2. 使用类对象、引用或指针来访问静态成员

### 定义静态成员
在类的外部定义静态成员时，不能重复`static`关键字，该关键字只出现在类内部

### 静态成员的类内初始化
通常情况下，类的静态成员不应该在类的内部初始化，然而，可以为静态成员提供`const`整数类型的类内初始值，不过要求静态成员必须是字面值常量类型的`constexpr`