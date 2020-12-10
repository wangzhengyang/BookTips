[toc]

# IO类
|头文件|类型|
|-|-|
|`iostream`|`istream`，`wistream`从流读取数据     `ostream`，`wostream`向流写入数据                 `iostream`，`wiostream`读写流|
|`fstream`|`ifstream`，`wifstream`从文件读取数据  `ofstream`，`wofstream`向文件写入数据   `fstream`，`wfstream`读写文件|
|`sstream`|`istringstream`，`wistringstream`从`string`读取数据`ostringostream`，`wostringstream`向`string`写入数据`stringstream`，`wstringstream`读写`string`

## IO对象无拷贝或赋值
不能拷贝或对IO对象赋值
## 条件状态
|状态|描述|
|-|-|
|`strm::iostate`|`strm`是一种IO类型，`iostate`是一种及其相关的类型，提供了表达条件状态的完整功能|
|`strm::badbit`|指出流已崩溃|
|`strm::failbit`|指出一个IO操作失败|
|`strm::eofbit`|指出流到达了文件结束|
|`strm::goodbit`|指出流未处于错误状态，此值保证为零|
|s.eof()|若流s的eofbit置位，则返回true|
|s.fail()|若流s的failbit或badbit置位，则返回true|
|s.bad()|若流s的badbit置位，则返回true|
|s.good()|若流s处于有效状态，则返回true|
|s.clear()|将流s中所有条件状态位复位，将流的状态设置为有效，返回void|
|s.clear(flags)|根据给定的flags标志位，将流s中对应条件状态位复位，flags的类型为`strm::iostate`，返回void|
|s.setstate(flags)|根据给定的flags标志位，将流s中对应条件状态位置位，flags的类型为`strm::iostate`，返回void|
|s.rdstate()|返回流s的当前条件状态，返回类型为`strm::iostate`|
### 查询流的状态
IO库定义了一个与机器无关的`iostate`类型，它提供了表达流状态的完整功能

`badbit`表示系统级错误，如不可恢复的读写错误，通常情况下，一旦`badbit`被置位，流就无法再使用

在发生可恢复错误后，`failbit`被置位，这种问题是可以修正的，流还可以继续使用

如果到达文件结束位置，`eofbit`和`failbit`都会被置位

`goodbit`的值为0，表示流未发生错误

如果`badbit`、`failbit`和`eofbit`任一个被置位，则检测流状态的条件会失败

## 管理输出缓冲
导致缓冲刷新(即，数据真正写到输出设备或文件)的原因：
1. 程序正常结束，作为`main`函数的`return`操作的一部分，缓冲刷新被执行
2. 缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区
3. 可以使用操作符如`endl`来显式刷新缓冲区
4. 在每个输出操作之后，可以用操作符`unitbuf`设置流的内部状态，来清空缓冲区，默认情况下，对`cerr`是设置`unitbuf`的，因此写到`cerr`的内容都是立即刷新的
5. 一个输出流可能被关联到另一个流，在这种情况下，当读写被关联的流时，关联到的流的缓冲区会被刷新

### 刷新输出缓冲区
`endl`完成换行并刷新缓冲区
`flush`刷新缓冲区，但不输出任何额外的字符
`ends`向缓冲区插入一个空字符，然后刷新缓冲区

### `unitbuf`操作符
如果想在每次输出操作后都刷新缓冲区，使用`unitbuf`操作符，它告诉流在接下来的每次写操作之后都进行一次`flush`操作
`nounitbuf`操作符则重置流，使其恢复使用正常的系统管理的缓冲区刷新机制

### 关联输入和输出流
当一个输入流被关联到一个输出流时，任何试图从输入流读取数据的操作都会刷新关联的输出流，通过`tie`关联

# 文件输入输出
头文件`fstream`
|操作|描述|
|-|-|
|`fstream fstrm`|创建一个未绑定的文件流，`fstream`是头文件`fstream`中定义的一个类型|
|`fstream fstrm(s)`|创建一个`fstream`，并打开名为`s`的文件，`s`可以是`string`类型，或者是一个指向C风格字符串的指针，这些构造函数都是`explicit`的，默认的文件模式`mode`依赖于`fstream`的类型|
|`fstream fstrm(s, mode)`|与前一个构造函数类似，但按指定`mode`打开文件|
|fstrm.open(s)|打开名为`s`的文件，并将文件与`fstrm`绑定，`s`可以是一个`string`或一个指向C风格字符串的指针，默认的文件`mode`依赖于`fstream`的类型，返回`void`|
|fstrm.close()|关闭与`fstrm`绑定的文件，返回`void`|
|fstrm.is_open()|返回一个`bool`值，指出与`fstrm`关联的文件是否成功打开且尚未关闭|

当一个`fstream`对象被销毁时，`close`会自动被调用

## 文件模式
|模式|描述|
|-|-|
|in|以读方式打开|
|out|以写方式打开|
|app|每次写操作前均定位到文件末尾|
|ate|打开文件后立即定位到文件末尾|
|trunc|截断文件|
|binary|以二进制方式进行IO|

保留被`ofstream`打开的文件中已有数据的唯一方式是显式指定`app`或`in`模式

在每次打开文件时，都要设置文件模式，可能是显式地设置，也可能是隐式地设置，当程序未指定模式时，就使用默认值

# `string`流
头文件`sstream`
|操作|描述|
|-|-|
|`sstream strm`|`strm`是一个未绑定的`stringstream`对象，`sstream`是头文件`sstream`中定义的一个类型|
|`sstream strm(s)`|`strm`是一个`sstream`对象，保存`string s`的一个拷贝，此构造函数是`explicit`的|
|strm.str()|返回`strm`所保存的`string`的拷贝|
|strm.str(s)|将`string s`拷贝到`strm`中，返回`void`|



