[toc]

# 文本处理
## sed编辑器
sed编辑器被称为流编辑器，会在编辑器处理数据之前基于预先提供的一组规则来编辑数据流
sed编辑器可以根据命令来处理数据流中的数据，这些命令要么从命令行中输入，要么存储在一个命令文本文件中
sed编辑器会执行下列操作：
1. 一次从输入中读取一行数据
2. 根据所提供的编辑器命令匹配数据
3. 按照命令修改流中的数据
4. 将新的数据输出到STDOUT

在流编辑器将所有命令与一行数据匹配完毕后，它会读取下一行数据并重复这个过程。在流编辑器处理完流中的所有数据行后，它就会终止
由于命令是按顺序逐行给出的，sed编辑器只需对数据流进行一遍处理就可以完成编辑操作

命令格式
```bash
sed options script file
```
|选项|描述|
|-|-|
|-e script|在处理输入时，将script中指定的命令添加到已有的命令中|
|-f file|在处理输入时，将file中指定的命令添加到已有的命令中|
|-n|不产生命令输出，使用print命令来完成输出|
script参数指定了应用于流数据上的单个命令。如果需要用多个命令，要么使用`-e`选项在命令行中指定，要么使用`-f`选项在单独的文件中指定
1. 在命令行定义编辑器命令

默认情况下，sed编辑器会将指定的命令应用到STDIN输入流上，从而可以直接将数据通过管道输入sed编辑器处理
```bash
echo "This is a test" | sed 's/test/big test'
```
`s`命令会用斜线间指定的第二个文本字符串来替换第一个文本字符串模式

2. 在命令行使用多个编辑器命令

要在sed命令行上执行多个命令时，只要用`-e`选项就可以
```bash
sed -e '/s/brown/green/; s/dog/cat/' data1.txt
```
两个命令都作用到文件中的每行数据上，命令之间必须用分号隔开，并且在命令末尾和分号之间不能有空格

3. 从文件中读取编辑器命令

如果有大量要处理的`sed`命令，将它们放进一个单独的文件中通常会更方便一些，`sed`命令中用`-f`选项来指定文件
```bash
# script1.sed
# s/brown/green/
# s/fox/elephant/
# s/dog/cat/

sed -f script1.sed data1.txt
```
在这种情况下，不用在每条命令后面放一个分号，sed编辑器知道每行都是一条单独的命令，跟在命令行输入命令一样，sed编辑器会从指定文件中读取命令，并将它们应用到数据文件中的每一行上

## `gawk`程序
可以做如下的事情：
1. 定义变量来保存数据；
2. 使用算术和字符串操作符来处理数据；
3. 使用结构化编程概率(比如if-then语句和循环)来为数据处理增加处理逻辑；
4. 通过提取数据文件中的数据元素，将其重新排列或格式化，生成格式化报告
`gawk`程序的报告生成能力通常用来从大文本文件中提取数据元素，并将它们格式化成可读的报告
最完美的例子是格式化日志文件，在日志文件中找到错误行会很难，`gawk`程序可以让你从日志文件中过滤出需要的数据元素，然后你可以将其格式化，使得重要的数据更易于阅读
1. `gawk`命令格式

```bash
gawk options program file
```
|选项|描述|
|-|-|
|-F fs|指定行汇总划分数据字段的字段分隔符|
|-f file|从指定的文件中读取程序|
|-v var=value|定义gawk程序中的一个变量及其默认值|
|-mf N|指定要处理的数据文件中的最大字段数|
|-mr N|指定数据文件中的最大数据行数|
|-W keyword|指定gawk的兼容模式或警告等级|

2. 从命令行读取程序脚本

`gawk`程序脚本用一对花括号来定义，你必须将脚本命令放到两个花括号({})中
```bash
gawk '{print "Hello world"}'
```
`gawk`需要从命令行或者文件接收输入，才能运行
使用Ctrl+D组合键会在bash中产生一个EOF字符，终止该`gawk`命令

3. 使用数据字段变量

`gawk`的主要特性之一是其处理文本文件中数据的能力。它会自动给一行中的每个数据元素分配一个变量。默认情况下，`gawk`会将如下变量分配给它在文本行中发现的数据字段：
(1)$0代表整个文本行；

(2)$1代表文本行中的第1个数据字段；

(3)$2代表文本行中的第2个数据字段；

(4)$n代表文本行中的第n个数据字段；
在文本行中，每个数据字段都是通过字段分隔符划分的，`gawk`在读取一行文本时，会用预定义的字段分隔符划分每个数据字段，`gawk`中默认的字段分隔符是任意的空白字符(例如空格或制表符)

如果读取采用其他字段分隔符的文件，使用`-F`选项指定
```bash
gawk -F: `{print $1}` /etc/passwd
```
4. 在程序脚本中使用多个命令


`gawk`允许你将多条命令组合成一个正常的程序，要在命令行上的程序脚本中使用多条命令，只要在命令之间放个分号即可
```bash
echo "My name is Rich" | gawk '{$4="Christine"; print $0}'
```
5. 从文件中读取程序

`gawk`编辑器允许将程序存储到文件中，然后再在命令行中引用
```bash
# script2.gawk
# {print $1 "'s home directory is " $6}
gawk -F: -f script2.gawk /etc/passwd
```
多条指令，只要一条命令放一行即可，不需要用分号
```bash
# script3.gawk
# {
# text = "'s home directory is "
# print $1 text $6
# }
gawk -F: -f script3.gawk /etc/passwd
```
6. 在处理数据前运行脚本

`gawk`允许指定程序脚本何时运行，默认请看下，`gawk`会从输入中读取一行文本，然后针对该行的数据执行程序脚本，通过`BEGIN`关键字强制`gawk`在读取数据前执行`BEGIN`关键字后指定的程序脚本
```bash
gawk 'BEGIN {print "The data3 File Contents:"} {print $0}' data3.txt
``` 
7. 在处理数据后运行脚本
`END`关键字允许你指定一个程序脚本，`gawk`会在读完数据后执行它
```bash
# script4.gawk
# BEGIN{
# print "The latest list of users and shells"
# print " UserID \t Shell"
# print "--------- \t ----------"
# FS=":"   
# }
#
# {
# print $1 "     \t " $7   
# }
#
# END{
# print "This concludes the listing"   
# }

gawk -f script4.gawk /etc/passwd
```
# sed编辑器基础
## 更多的替换选项
1. 替换标记

默认情况下只替换每行中出现的第一处，要让替换命令能够替换一行中不同地方出现的文本必须使用替换标记，替换标记在替换命令字符串之后设置
```
s/pattern/replacement/flags
```
有4钟可用的替换标记：
(1) 数字，表明新文本将替换第几处模式匹配的地方
(2) g，表明新文本将会替换所有匹配的文本；
(3) p，表明原先行的内容要打印出来；
(4) w file，将替换的结果写到文件中；
`p`替换标记会打印与替换命令中指定的模式匹配的行，通常会和`-n`选项一起使用，`-n`选项将禁止sed编辑器输出，但`p`替换标记会输出修改过的行，将二者配合使用的效果就是只输出被替换命令修改过的行

2. 替换字符

在文本字符串中遇到一些不太方便在替换模式中使用的字符，由于正斜线通常用作字符串分隔符，所以如果它出现在模式文本中的话，必须用反斜线来转义
```bash
sed 's/\/bin\/bash/\/bin\/csh/' /etc/passwd
```
sed编辑器允许选择其他字符来作为替换命令中的字符串分隔符
```bash
sed 's!/bin/bash!/bin/csh!' /etc/passwd
```
## 使用地址
默认情况下，在sed编辑器中使用的命令会作用于文本数据的所有行。如果只想将命令作用于特定行或某些行，则必须用行寻址
(1)以数字形式表示行区间
(2)用文本模式来过滤出行
```bash
[address]command

address{
    command1
    command2
    command3
}
```
sed编辑器会将指定的每条命令作用到匹配指定地址的行上
1. 数字方式的行寻址

当使用数字方式的行寻址时，可以用行在文本流中的行位置来引用。sed编辑器会将文本流中的第一行编号为1，然后继续按顺序为接下来的行分配行号
在命令中指定的地址可以是单个行号，或是用起始行号、逗号以及结尾行号指定的一定区间范围内的行
```bash
sed '2s/dog/cat/' data1.txt
sed '2,3s/dog/cat/' data1.txt
sed '2,$s/dog/cat/' data1.txt #使用美元符表示到文件末尾
```
2. 使用文本模式过滤器

sed编辑器允许指定文本模式来过滤出命令要作用的行
```
/pattern/command
```
```bash
grep Samantha /etc/passwd
# output
# Samantha:x:502:502::/home/Samantha:/bin/bash
sed '/Samantha/s/bash/csh/' /etc/passwd
# output
# Samantha:x:502:502::/home/Samantha:/bin/csh
```
该命令只作用到匹配文本模式的行上。虽然使用固定文本模式能帮你过滤出特定的值，但其作用难免有限，sed编辑器在文本模式中采用了一种称为正则表达式的特性来帮助你创建匹配效果更好的模式
3. 命令组合

需要在单行上执行多条命令，可以用花括号将多条命令组合在一起
```bash
sed '2{s/fox/elephant s/dog/cat}' data1.txt
```
## 删除行
`d`会删除匹配指定寻址模式的所有行，使用该命令时，如果忘记加入寻址模式的话，流中的所有文本行都会被删除
```bash
sed 'd' data1.txt # 删除所有文本行
sed '3d' data6.txt
sed '2,3d' data6.txt
sed '3,$d' data6.txt
sed `/number 1/d` data6.txt
```
也可以使用两个文本模式来删除某个区间内的行，指定的第一个模式会"打开"行删除功能，第二个模式会"关闭"行删除功能
```bash
sed '/1/,/3/d' data6.txt
```
特别小心，只要sed编辑器在数据流中匹配到了开始模式，删除功能就会打开
## 插入和附加文本
(1)插入(insert)命令(`i`)会在指定行前增加一个新行
(2)附加(append)命令(`a`)会在指定行后增加一个新行
它们不能在单个命令行上使用，必须指定是要将行插入还是附加到另一行
```bash
sed '[address]command\new line'
```
`new line`中的文本将会出现在sed编辑器输出中你指定的位置
当使用插入命令时，文本会出现在数据流文本的前面
```bash
echo "Test line 2" | sed 'i\Test Line 1'
# output
# Test line 1
# Test line 2
```
使用附加命令，文本会出现在数据流文本的后面
```bash
echo "Test Line 2" | sed 'a\Test Line 1'
# output
# Test Line 2
# Test Line 1
```
```bash
sed '3i\This is an inserted line.' data6.txt
sed '3a\This is an append line.' data6.txt
sed '$a\This is a new line of text.' data6.txt
sed '1i\This is one line of new text.\This is another line of new text.' data6.txt
```
## 修改行
修改(change)命令(`c`)允许修改数据流中整行文本的内容
```bash
sed '3c\This is a changed line of text.' data6.txt
sed '/number 3/c\This is a changed line of text.' data6.txt
sed '/number 1/c\This is a changed line of text.' data8.txt
sed '2,3c\This is a new line of text.' data6.txt
```
## 转换命令
转换(transform)命令(`y`)是唯一可以处理单个字符的sed编辑器命令
```bash
[address]y/inchars/outchars/
```
转换命令会对inchars和outchars值进行一对一的映射，inchars中的第一个字符会被转换为outchars中的第一个字符，第二个字符会被转换为outchars的第二个字符，依次类推；如果inchars和outchars的长度不同，则sed编辑器会产生一条错误信息
```bash
sed 'y/123/789/' data8.txt
```
转换命令是一个全局命令，它会将文本中找到的所有指定字符自动进行转换，而不考虑它们出现的位置
## 回顾打印
### 打印行
`p`命令通常用来打印包含匹配文本模式的行
在命令行上用`-n`选项，禁止输出其他行，只打印包含匹配文本模式的行
也可以快速打印数据流中的某些行
```bash
sed -n '/number 3/p' data6.txt
sed -n '2,3p' data6.txt
```
可以创建一个脚本在修改行之前显示该行
```bash
sed -n '/3/{p s/line/test/p}' data6.txt
```
### 打印行号
等号(=)命令会打印行在数据流中的当前行号，行号由数据流中的换行符决定
```bash
sed '=' data1.txt
```
在数据流中查找特定文本模式
```bash
sed -n '/number 4/{= p}' data6.txt
```
### 列出行
列出(list)命令(l)可以打印数据流中的文本和不可打印的ASCII字符
```bash
sed -n 'l' data10.txt
```
## 使用`sed`处理文件
### 写入文件
`w`命令用来向文件写入行
格式：
```bash
[address]w filename
```
```bash
sed '1,2w test.txt' data6.txt
```
```bash
sed -n '/Browncoat/w Browncoats.txt' data11.txt
```
### 从文件读取数据
读取(read)命令(r)允许将一个独立文件中的数据插入到数据流中
格式:
```bash
[address]r filename
```
```bash
sed '3r data12.txt' data6.txt
sed '/number 2/r data12.txt' data6.txt
```
通过占位文本LIST来放入任务名单的位置
```bash
sed '/LIST/{r data1.txt d}' notice.std
```
