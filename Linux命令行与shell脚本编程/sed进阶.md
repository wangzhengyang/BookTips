[toc]

# 多行命令
sed编辑器包含三个可用来处理多行文本的特殊命令
`N`：将数据流中的下一行加进来创建一个多行组(multiline group)来处理
`D`：删除多行组中的一行
`P`：打印多行组中的一行
## next命令
### 单行的next命令
小写的`n`命令会告诉sed编辑器移动到数据流中的下一文本行，而不用重新回到命令的最开始再执行一遍；通常sed编辑器在移动到数据流中的下一文本行之前，会在当前行上执行完所有定义好的命令，单行next命令改变了这个流程
```bash
sed '/header/{n ; d}' data1.txt
```
### 合并文本行
单行next命令会将数据流中的下一文本行移动到sed编辑器的工作空间(称为模式空间)，多行版本的next命令(用大写N)会将下文本行添加到模式空间中已有的文本后
这样的作用是将数据流中的两个文本行合并到同一个模式空间中，文本行仍然用换行符分隔，但sed编辑器现在会将两行文本当成一行来处理
```bash
sed '/first/{N ; s/\n/ /}' data2.txt
sed 'N ; s/System.Adminstrator/Desktop User' data3.txt
sed 's/System Adminstrator/Desktop User/ ; N ; s/System\nAdminstrator/Desktop\nUser/' data4.txt
```
## 多行删除命令
sed编辑器提供了多行删除命令`D`,它只删除模式空间中的第一行，该命令会删除到换行符(含换行符)为止的所有字符
```bash
sed '/^$/{N ; /header/D}' data5.txt
```
## 多行打印命令
只打印多行模式空间中的第一行，这包括模式空间中直到换行符为止的所有字符
```bash
sed -n 'N ; /System\nAdminstrator/P' data3.txt
```
# 保持空间
模式空间(pattern space)是一块活跃的缓冲区，在sed编辑器执行命令时它会保持待检查的文本，但它并不是sed编辑器保存文本的唯一空间
sed编辑器有另一块称作保持空间(hold space)的缓冲区域，在处理模式空间中的某些行时，可以用保持空间来临时保存一些行
|命令|描述|
|-|-|
|h|将模式空间复制到保持空间|
|H|将模式空间附加到保持空间|
|g|将保持空间复制到模式空间|
|G|将保持空间附加到模式空间|
|x|交换模式空间和保持空间的内容|
通常，在使用`h`或`H`命令将字符串移动到保持空间后，最终还要用`g`、`G`或`x`命令将保存的字符串移回模式空间
```bash
sed -n '/first/{h ; p ; n ; p ; g ; p}' data2.txt
```
# 排除命令
感叹号命令(`!`)用来排除(negate)命令，也就是让原本会起作用的命令不起作用
```bash
sed -n '/header/!p' data.txt
```
```bash
sed '$!N ; s/System\nAdminstrator/Desktop\nUser/  ; s/System Adminstrator/Desktop User/' data4.txt
```
```bash
sed -n '{1!G ; h ; $p}' data2.txt 
```
# 改变流
## 分支
格式
```
[address]b [label]
```
`address`参数决定哪些行的数据会触发分支命令；`label`参数定义要跳转到的位置，如果没有加`label`参数，跳转命令会跳转到脚本的结尾
```bash
sed '{2,3b ; s/This is/Is this/ ; s/line./test?/}' data2.txt
```
要是不想直接跳到脚本的结尾，可以为分支命令定义一个要跳转到的标签，标签以冒号开始，最大可以是7个字符长度
```bash
sed '{/first/b jump1 ; s/This is the/No jump on/ ; :jump1 ; s/This is the/Jump here on/}' data.txt
```
循环查找
```bash
echo "This, is, a, test, to, remove, commas." | sed '{:start ; s/,//1p ; /,/b start}'
```
## 测试
格式
```
[address]t [label]
```
跟分支命令一样，在没有指定标签的情况下，如果测试成功，sed会跳转到脚本的结尾
```bash
sed '{s/first/matched/ ; t ; s/This is the/No match on/}' data.txt
```
如果替换命令成功匹配并替换了一个模式，测试命令就会跳转到指定的标签；如果替换命令未能匹配指定的模式，测试命令就不会跳转
```bash
echo "This, is, a, test, to, remove, commas." | sed '{:start ; s/,//1p ; t start}'
```
# 模式替换
在使用通配符时，很难知道到底哪些文本会匹配模式
## `&`符号
`&`符号可以用来代表替换命令中的匹配的模式。不管模式匹配的是什么样的文本，都可以在替代模式中使用`&`符号来使用这段文本
```bash
echo "This cat sleeps in his hat." | sed 's/.at/"&"/g'
```
## 替换单独的单词
用圆括号定义替换模式中的子模式，可以在替换模式中使用特殊字符来引用每个子模式；替代符号由反斜线和数字组成，数字表明子模式的位置，sed编辑器会给第一个子模式分配字符\1，给第二个子模式分配字符\2，以此类推
```bash
echo "The System Administrator manual" | sed 's/\(System\) Adminstrator/\1 User'
```
```bash
echo "1234567" | sed '{:start ; s/\(.*[0-9]\)\([0-9]\{3\}\)/\1,\2/; t start}'
```
# 脚本中使用`sed`
## 使用包装脚本
将sed编辑器命令放到shell包装脚本(wrapper)中，不用每次使用时都重新键入整个脚本，包装脚本充当着sed编辑器脚本和命令行之间的中间人角色
```bash
#!/bin/bash
sed -n '{1!G ; h ; $p}' $1
```
## 重定向`sed`输出
默认情况下，sed编辑器会将脚本的结果输出到STDOUT上，可以在脚本中用$()将sed编辑器的输出重定向到一个变量中，以备后用
```bash
#!/bin/bash
factorial=1
counter=1
number=$1
while [ $counter -le $number ]
do
    factorial=$[ $factorial * $counter ]
    counter=$[ $counter + 1 ]
done
result=$(echo $factorial | sed '{:start ; s/\(.*[0-9]\)\([0-9]\{3\}\)/\1,\2/ ; t start}')
```
# `sed`实用工具
## 加倍行间距
```bash
sed 'G' data.txt
```
这个脚本在数据流的最后一行后面也会添加一个空白行，使得文件的末尾也产生一个空白行，可以用排查符号(`!`)和尾行符号(`$`)来确保脚本不会将空白行加到数据流的最后一行后面
```bash
sed '$!G' data.txt
```
## 对可能含有空白行的文件加倍行间距
```bash
sed '/^$/d ; $!G' data.txt
```
## 给文件中的行编号
```bash
sed '=' data.txt
```
```bash
sed '=' data2.txt | sed '{N ; s/\n/ /}'
```
## 打印末尾行
```bash
sed '$p' data.txt
```
只显示最后10行
```bash
sed '{:start ; $q ; N ; 11,$D ; b start}' data.txt
```
## 删除行
1. 删除连续的空白行
   
```bash
sed '/./,/^$/!d' data.txt
```
2. 删除开头的空白行

```bash
sed '/./,$!d' data.txt
```
3. 删除结尾的空白行

```bash
sed '{:start ; /^\n*/{$d ; N ; b start}}'
```
## 删除HTML标签
```bash
sed 's/<[^>]*>//g' data.txt
```