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