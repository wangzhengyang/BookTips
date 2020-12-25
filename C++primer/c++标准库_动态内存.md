[toc]

静态内存用来保存局部static对象、类static数据成员以及定义在任何函数之外的变量

栈内存用来保存定义在函数内的非static对象

除了静态内存和栈内存，每个程序还有一个内存池，称作**自由空间**或**堆**，程序用堆来存储**动态分配**

# 动态内存与智能指针
`new`，在动态内存中为对象分配空间并返回一个指向该对象的指针，可以选择对对象进行初始化

`delete`，接受一个动态对象的指针，销毁该对象，并释放与之关联的内存

`memory`头文件定义`shared_ptr`、`unique_ptr`以及`weak_ptr`

`shared_ptr`允许多个指针指向同一个对象

`unique_ptr`独占所指向的对象

`weak_ptr`是一种弱引用，指向`shared_ptr`所管理的对象

## `shared_ptr`类
智能指针的使用方式与普通指针类似，解引用一个智能指针返回它所指向的对象

|`shared_ptr`和`unique_ptr`支持操作|描述|
|-|-|
|shared_ptr\<T> sp unique_ptr\<T> up|空智能指针，可以指向类型为T的对象|
|p|将p用作一个条件判断，若p指向一个对象，则为true|
|*p|解引用p，获得它指向的对象|
|p->mem|等价于(*p).mem|
|p.get()|返回p中保存的指针，要小心使用，若智能指针释放了对象，返回的指针所指向的对象也就消失了|
|swap(p,q) p.swap(q)|交换p和q中的指针|

|`shared_ptr`独有操作|描述|
|-|-|
|make_shared<T>(args)|返回一个shared_ptr，指向一个动态分配的类型为T的对象，使用args初始化此对象|
|shared_ptr<T> p(q)|p是shared_ptr q的拷贝，此操作会递增q中的计数器，q中的指针必须能转换为T*|
|p=q|p和q都是shared_ptr，所保存的指针必须能相互转换，此操作会递减p的引用计数，递增q的引用计数，若p的引用计数为0，则将其管理的原内存释放|
|p.unique()|若p.use_count()为1，返回true;否则返回false|
|p.use_count()|返回与p共享对象的智能指针数量；可能很慢，主要用于调试|

### `make_shared`函数
`memory`头文件
最安全的分配和使用动态内存的方式是使用`make_shared`函数，此函数在动态内存中分配一个对象并初始化它

### `shared_ptr`自动销毁管理对象
通过特殊的成员函数——**析构函数**(destructor)完成销毁工作

## 直接管理内存
除非使用智能指针来管理内存，否则不要分配动态内存

## `shared_ptr`和`new`结合使用
|定义和改变`shared_ptr`的方法|描述|
|-|-|
|shared_ptr<T> p(q)|p管理内置指针q所指向的对象；q必须指向new分配的内存，且能够转换为T*类型|
|shared_ptr<T> p(u)|p从unique_ptr u那里接管了对象的所有权；将u置为空|
|shared_ptr<T> p(q,d)|p接管了内置指针q所指向的对象的所有权，q必须能转换为T*类型，p将使用可调用对象d来代替delete|
|shared_ptr<T> p(p2,d)|p是shared_ptr p2的拷贝，唯一的区别是p将用可调用对象d来代替delete|
|p.reset() p.reset(q) p.reset(q,d)|若p是唯一指向其对象的shared_ptr，reset会释放此对象，若传递了可选的参数内置指针q，会令p指向q，否则会将p置为空，若还传递了参数d，将会调用d而不是delete来释放q|

## 智能指针和异常
使用智能指针来确保在异常发生后资源能被正确地释放

### 智能指针和哑类
默认情况下，`shared_ptr`假定它们指向的是动态内存，当一个`shared_ptr`被销毁时，默认地对管理的指针进行`delete`操作，通过使用**删除器**完成释放工作

## `unique_ptr`
某个时刻只能有一个`unique_ptr`指向一个给定对象

初始化`unique_ptr`必须采用直接初始化形式

`unique_ptr`不支持普通的拷贝或赋值操作
|操作|描述|
|-|-|
|unique_ptr<T> u1 unique_ptr<T,D> u2|空unique_ptr,可以指向类型为T的对象，u1会使用delete来释放它的指针；u2会使用一个类型为D的可调用对象来释放它的指针|
|unique_ptr<T,D> u(d)|空unique_ptr，指向类型为T的对象，用类型为D的对象d代替delete|
|u=nullptr|释放u指向的对象，将u置为空|
|u.release()|u放弃对指针的控制权，返回指针，并将u置为空|
|u.reset() u.reset(q) u.rest(nullptr)|释放u指向的对象,如果提供了内置指针q，令u指向这个对象；否则将u置为空|

## `weak_ptr`
