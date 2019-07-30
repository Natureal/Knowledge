## 1. 查找目录下包含某个字符串的文件

(1) grep -R "search_words" DIR

如果搜索当前目录下包含 "my" 的文件：grep -R "my" .

## 2. GDB 备忘录

1. 编译时加上 -g

2. start：开始运行，r 也可以

3. b 行号：添加断点 / b 行号 if 表达式：条件断点

4. i b：info breakpoints 列出当前断点

5. delete 编号：删除某个断点

6. l：从头列出代码 / l 行号：从某行开始列出代码。

7. watch 变量：变量改变就停下

8. i watchpoints：查看 watchpoints

9. run 运行程序，可简写为r

10. next 单步跟踪，函数调用当作一条简单语句执行，可简写为n

11. step 单步跟踪，函数调进入被调用函数体内，可简写为s

12. finish 退出进入的函数

13. until 在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体,可简写为u。

14. continue 继续运行程序，可简写为c
