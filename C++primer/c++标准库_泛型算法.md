[toc]

大多数算法定义在`algorithm`头文件
一组数值泛型算法定义在`numeric`头文件

# 算法如何工作
1. 迭代器令算法不依赖于容器
2. 算法依赖于元素类型的操作

泛型算法本身不会执行容器的操作，它们只会运行于迭代器之上，执行迭代器的操作，所以算法永远不会改变底层容器的大小，算法可能改变容器中保存的元素的值，也可能在容器内移动元素，但永远不会直接添加或删除元素

# 泛型算法类型
## 只读算法

1. 查找某个值

`find(vec.cbegin(), vec.cend(), val)`

2. 求和

`numeric`头文件

`accumulate(vec.cbegin(), vec.cend(), init)`

前两个指出需要求和的元素的范围，第三个参数是和的初值

`accumulate`的第三个参数的类型决定函数中使用哪个加法元素符以及返回值的类型

对于只读取而不改变元素的算法，通常最好使用`cbegin()`和`cend()`

3. 确定两个序列是否保存相同的值

`equal(vec1.cbegin(), vec1.cend(), vec2.cbegin())`

可以比较两个不同类型的容器中的元素，而且元素类型也不必一样，只要能用`==`来比较两个元素类型即可

但是`equal`基于一个假设：它假定第二个序列至少与第一个序列一样长，此算法要处理第一个序列中的每个元素，它假定每个元素在第二个序列中都有一个与之对应的元素

那些只接受一个单一迭代器来表示第二个序列的算法，都假定第二个序列至少与第一个序列一样长

## 写容器元素算法

1. 填充

`fill(vec.begin(), vec.end(), val)`
`fill_n(vec.begin(), size, val)`

`fill_n`必须假定向目的位置迭代器写入数据的算法时，目的位置足够大，能容纳要写入的元素

2. 拷贝

`copy(vec.cbegin(), vec.cend(), dst)`

前两个表示一个输入范围，第三个表示目的序列的起始位置

传递给`copy`的目的序列至少要包含与输入序列一样多的元素

`copy`返回的是其目的位置迭代器(递增后)的值

3. 替换

`replace(vec.begin(), vec.end(), val1, val2)`

将范围内的val1的值替换成val2的值

`replace_copy(vec1.cbegin(), vec1.cend(), back_inserter(vec2), val1, val2)`
将范围内的val1的值替换成val2的值装入到新的容器中

## `back_inserter`插入迭代器
一种向容器中添加元素的迭代器，通常情况下，当通过一个迭代器向容器元素赋值时，值被赋予迭代器指向的元素，而当通过一个插入迭代器赋值时，一个与赋值号右侧值相等的元素被添加到容器中

`iterator`头文件

`back_iterator`接受一个指向容器的引用，返回一个与该容器绑定的插入迭代器，当通过此迭代器赋值时，赋值运算符会调用`push_back`将一个具有给定值的元素添加到容器中

## 重排容器元素算法

1. 排序

`sort(vec.begin(), vec.end())`

2. 相邻重复项"消除"

`unique(vec.begin(), vec.end())`

将相邻的重复项"消除"，并返回一个指向不重复值范围末尾的迭代器

如果需要删除元素，需要通过容器来进行删除操作

# 定制操作
很多算法会比较输入序列中的元素，默认情况下，算法使用元素类型的<或==运算符完成比较，但是标准库也为这些算法定义额外的版本，允许提供自定义的操作来替代默认运算符

## 向算法传递函数
### 谓词
谓词是一个可调用的表达式，其返回结果是一个能用作条件的值，标准库算法所使用的谓词分为两类：**一元谓词**(unary predicate，只接受单一参数)和**二元谓词**(binary predicate，接受两个参数)

接受谓词参数的算符对输入序列中的元素调用谓词，因此，元素类型必须能转换为谓词的参数类型

### 排序算法
```c++
bool isShorter(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}
```
`stable_sort(vec.begin(), vec.end(), isShorter)`

`stable_sort`维持相等元素的原有顺序

## lambda表达式
可以向一个算法传递任何类别的`可调用对象`，对于一个对象或一个表达式，如果可以对其使用调用运算符，则称它为可调用的

一个lambda表达式表示一个可调用的代码单元，可以将其理解为一个未命名的内联函数，与任何函数类型，一个lambda具有一个返回类型、一个参数列表和一个函数体
`[capture list](parameter list) -> return type{function body}`

`capture list`(捕获列表)是一个lambda所在函数中定义了局部变量的列表(通常为空)
`return type`、`paramter list`和`function body`与任何普通函数一样，分别表示返回类型、参数列表和函数体，但是，lambda必须使用尾置返回来指定返回类型

可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体
```c++
auto f = []{return 42;}
```

在lambda中忽略括号和参数列表等价于指定一个空参数列表

如果忽略返回类型，lambda根据函数体中的代码推断出返回类型，如果函数体只是一个return语句，则返回类型从返回的表达式的类型推断而来，否则，返回类型为void

### 向lambda传递参数
调用一个lambda时给定的实参被用来初始化lambda的形参，通常，实参和形参的类型必须匹配，而且lambda不支持默认参数

### 使用捕获列表
lambda通过将局部变量包含在其捕获列表中来指出将会使用这些变量，捕获列表指引lambda在其内部包含访问局部变量所需的信息

### `find_if`算法
```c++
auto wc = find_if(vec.begin(), vec.end(), [sz](const string &a){return a.size() > sz;})l
```

### `for_each`算法
```c++
for_each(wc, vec.end(), [](const string &s){cout << s << " ";});
```

捕获列表只用于局部非static变量，lambda可以直接使用局部static变量和在它所在函数之外声明的名字

## lambda捕获和返回
当定义一个lambda时，编译器生成一个与lambda对应的新的(未命名的)类类型，当向一个函数传递一个lambda时，同时定义了一个新类型和该类型的一个对象：传递的参数就是此编译器生成的类类型的未命名对象。类似的，当使用auto定义一个用lambda初始化的变量时，定义一个从lambda生成的类型的对象

默认情况下，从lambda生成的类都包含一个对应该lambda所捕获的变量的数据成员，类似任何普通类的数据成员，lambda的数据成员也在lambda对象创建时被初始化

### 值捕获
变量的捕获方式可以是值或引用，采用值捕获的前提是变量可以拷贝，与参数不同，被捕获的变量的值是在lambda创建时拷贝，而不是调用时拷贝

由于被捕获变量的值是在lambda创建时拷贝，因此随后对其修改不会影响到lambda内对应的值

### 引用捕获
以引用方式捕获的变量与其他任何类型的引用的行为类似，当在lambda函数体内使用此变量时，实际上使用的是引用绑定的对象

引用捕获与返回引用有着相同的问题和限制，如果采用引用方式捕获一个变量，就必须确保被引用的对象在lambda执行的时候是存在，因为lambda捕获的是局部变量

当以引用方式捕获一个变量时，必须保证在lambda执行时变量时存在的

### 隐式捕获
`&`告诉编译器采用捕获引用方式，`=`表示采用值捕获方式

当混合使用隐式捕获和显示捕获时，捕获列表中的第一个元素必须是一个`&`或`=`，此符号指定了默认捕获方式为引用或值

|捕获列表|描述|
|-|-|
|[]|空捕获列表，lambda不能使用所在函数中的变量，一个lambda只有捕获变量后才能使用它们|
|[names]|names是一个逗号分隔的名字列表，这些名字都是lambda所在函数的局部变量，默认情况下，捕获列表中的变量都被拷贝，名字前如果使用了`&`，则采用引用捕获|
|[&]|隐式捕获列表，采用引用捕获方式，lambda体中所使用的来自所在函数的实体都采用引用方式使用|
|[=]|隐式捕获列表，采用值捕获方式，lambda体将拷贝所使用的来自所在函数的实体的值|
|[&, identifier_list]|identifier_list是一个逗号分隔的列表，包含0个或多个来自所在函数的变量，这些变量采用值捕获方式，而任何隐式捕获的变量都采用引用方式捕获，identifier_list中的名字前面不能使用&|
|[=, identifier_list]|identifier_list中的变量都采用引用方式捕获，而任何隐式捕获的变量都采用值方式捕获，identifier_list中的名字不能包括this，且这些名字之前必须使用&|

### 可变lambda
默认情况下，对于一个值被拷贝的变量，lambda不会改变其值，如果希望能改变一个被捕获的变量的值，必须在参数列表首加上关键字`mutable`
```c++
auto f = [v1] () mutable{return ++v1;}
```

### 指定lambda返回类型
默认情况下，如果一个lambda体包含return之外的任何语句，则编译器假定此lambda返回void，与其他返回void的函数相似，被推断返回voide的lambda不能返回值

`transform`函数
```c++
transform(vi.begin(), vi.end(), vi.begin(), [](int i){return i < 0? -i : i;});
```

```c++
transform(vi.begin(), vi.end(), vi.begin(), [](int i){if (i < 0) return -i;else return i;})
```
编译推断这个版本的lambda返回类型为void，但实际返回int

```c++
transform(vi.begin(), vi.end(), vi.begin(), [](int i )->int{if (i < 0) return -i;else return i;});
```

## 参数绑定
### 标准库`bind`函数
`functional`头文件
将`bind`函数看做一个通用的函数适配器，它接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表

一般形式
`auto newCallable = bind(callable, arg_list);`

`newCallable`本身是一个可调用对象，`arg_list`是一个逗号分隔的参数列表，对应给定的`callable`的参数，当调用`newCallable`时，`newCallable`会调用`callable`，并传递给它`arg_list`中的参数

`arg_list`中的参数可能包含形如`_n`的名字，其中n是一个整数，这些参数是“占位符”，表示`newCallable`的参数，它们占据传递给`newCallable`的参数的“位置”，数值`n`表示生产的可调用对象中参数的位置：`_1`为`newCallable`的第一个参数，`_2`为第二个参数，以此类推

```c++
auto wc = find_if(words.begin(), words.end(), bind(check_size, _1, sz));
```

### 使用placeholders名字
名字`_n`都定义在一个名为`placeholders`的命令空间中

`using std::placeholders::_1`

使用`using namespace std::placeholders`来使用`placeholders`，定义在`functional`头文件

### `bind`的参数
可以用`bind`绑定给定可调用对象中的参数或重新安排其顺序

### 用`bind`重排参数顺序
```c++
//按单词长度由短到长排序
sort(words.begin(), words.end(), isShorter);
//按单词长度由长到短排序
sort(words.begin(), words.end(), bind(isShorter, _2, _1));
```

### 绑定引用参数
默认情况下，`bind`的那些不是占位符的参数被拷贝到`bind`返回的可调用对象中，但是有时需要以引用方式传递

如果希望传递给`bind`一个对象而又不拷贝它，使用标准库`ref`函数
```c++
for_each(words.begin(), words.end(), bind(print, ref(os), _1, ' '));
```
函数`ref`返回一个对象，包含给定的引用，此对象是可以拷贝的，标准库还有一个`cref`函数，生成一个保存`const`引用的类型，函数`ref`和`cref`都定义在头文件`functional`中

# 再探迭代器
`iterator`头文件
1. **插入迭代器**(insert iterator)：这些迭代器被绑定到一个容器上，可用来向容器插入元素
2. **流迭代器**(stream iterator)：这些迭代器被绑定到输入或输出流上，可用来遍历所关联的IO流
3. **反向迭代器**(reverse iterator)：这些迭代器向后而不是向前移动，除了`forward_list`之外的标准库容器都有反向迭代器
4. **移动迭代器**(move iterator)：这些专用的迭代器不是拷贝其中的元素，而是移动它们

## 插入迭代器
插入器是一种迭代器适配器，它接受一个容器，生成一个迭代器，能实现向给定容器添加元素，当通过一个插入迭代器进行赋值时，该迭代器调用容器操作来向给定容器的指定位置插入一个元素
|操作|描述|
|-|-|
|it = t|在it指定的当前位置插入值t，假定c是it绑定的容器，依赖于插入迭代器的不同种类，此赋值会分别调用c.push_back(t)、c.push_front(t)或c.insert(t,p),其中p为传递给inserter的迭代器位置|
|*it,++it,it++|这些操作虽然存在，但不会对it做任何事情，每个操作都返回it|

1. **back_iterator**创建一个使用`push_back`的迭代器
2. **front_iterator**创建一个使用`push_front`的迭代器
3. **inserter**创建一个使用`insert`的迭代器，此函数接受第二个参数，这个参数必须是一个指向给定容器的迭代器，元素将被插入到给定迭代器所表示的元素之前




