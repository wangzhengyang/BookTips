[toc]
#创建shell
```bash
#!/bin/bash
#This script displays the date and who's logged on
```
`#`用作注释行
`#`后面的**惊叹号**告诉shell用哪个shell来运行脚本
#运行shell
1. 将shell脚本文件所处的目录添加到`PATH`环境变量中；
2. 在提示符中用绝对火相对文件路径来引用shell脚本文件
3. 赋予shell脚本执行文件的权限
# 显示消息
```bash
echo content    #output:content
echo "content"  #output:content
echo 'content'  #output:content
echo '"content"' #output:"content"
```
|选项|描述|
|-|-|
|-n|把文本字符串和命令输出显示在同一行|
```bash
echo -n "The time and date are: "
```
**需在字符串的两侧使用引号，保证要显示的字符串尾部有一个空格**
# 使用变量
## 环境变量
```bash
set
```
显示一份完整的当前环境变量列表
```bash
$variable
```
使用变量
##用户变量
```bash
variable=val
```
**在变量、等号和值之间不能出现空格**
##命令替换
1. 反引号字符(`)
2. $()格式
#重定向输入和输出
##输出重定向
```bash
command > outputfile    
command >> outputfile   #追加
```
##输入重定向
```bash
command < inputfile
command << marker
data
marker
#<<必须指定一个文本标记来划分输入数据的开始和结尾；任何字符串都可作为文本标记，但在数据的开始和结尾文本标记必须一致
```
# 管道
```bash
command1 | command2
```
#执行数学运算
##`expr`命令
```bash
expr operation
```
|操作符|描述|
|-|-|
|ARG1 \| ARG2|如果ARG1既不是null也不是零值，返回ARG1，否则返回ARG2|
|ARG1 & ARG2|如果没有参数是null或零值，返回ARG1；否则返回0|
|ARG1 < ARG2|如果ARG1小于ARG2，返回1；否则返回0|
|ARG1 <= ARG2|如果ARG1小于或等于ARG2，返回1；否则返回0|
|ARG1 = ARG2|如果ARG1等于ARG2，返回1；否则返回0|
|ARG1 != ARG2|如果ARG1不等于ARG2，返回1；否则返回0|
|ARG1 >= ARG2|如果ARG1大于或等于ARG2，返回1；否则返回0|
|ARG1 > ARG2|如果ARG1大于ARG2，返回1；否则返回0|
|ARG1 + ARG2|返回ARG1和ARG2的算术运算和|
|ARG1 - ARG2|返回ARG1和ARG2的算术运算差|
|ARG1 * ARG2|返回ARG1和ARG2的算术乘积|
|ARG1 / ARG2|返回ARG1被ARG2除的算数商|
|ARG1 % ARG2|返回ARG1被ARG2除的算术余数|
|STRING : REGEXP|如果REGEXP匹配到了STRING中的某个模式，返回该匹配模式|
|match STRING REGEXP|如果REGEXP匹配到了STRING中的某个模式，返回该匹配模式|
|substr STRING POS LENGTH|返回起始位置为POS(从1开始计数)、长度为LENGTH个字符的子字符串|
|index STRING CHARS|返回在STRING中找到CHARS字符串的位置；否则，返回0|
|length STRING|返回字符串STRING的数值长度|
|+ TOKEN|将TOKEN解释成字符串，即使是个关键字|
|(EXPRESSION)|返回EXPRESSION的值|
##使用方括号
```bash
$[ operation ]
```
## 浮点数