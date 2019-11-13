## Linux Shell 命令备忘录

Referrence：[Linux Tools Quick Tutorial](https://linuxtools-rst.readthedocs.io/zh_CN/latest/)

- 搜索文件夹下包含某字符串的所有文件

  - grep -R "search_words" DIR

  （如果搜索当前目录下包含 "my" 的文件：grep -R "my" .）

- 搜索文件夹下名字中包含某字符串的所有文件

  - find DIR -name "string"

  （如果查找目标文件夹中是否有obj文件: find ./ -name '*.o'）

- 每秒刷新 top

  - top -d 1

- 查看文件内容

  - cat -n FILENAME（显示文件，加上行号）

  - head -n FILENAME（显示文件前 n 行）

  - tail -n FILENAME（显示文件倒数 n 行）

  - diff FILE1 FILE2（比较两个文件的区别）

- 修改文件（夹）权限

  u 表示拥有者，g 表示同 group 者，o 表示其他人，a 表示所有人。

  - chmod ugo+r a.sh（\+ 表示增加权限）

  - chmod -R a+rw *（-R 表示递归地设置文件夹内的所有文件权限）

  - chmod 777 FILE（等价于  chmod u=rwx,g=rwx,o=rwx file 或  chmod a=rwx file）

- 修改文件（夹）拥有者

  通常需要 root 权限

  - chown tom:users file FILE1 FILE2（把两个文件的拥有者改为 users 下的 tom）

  - chown -R James:users  *（设置当前目录下与子目录下的所有文件的拥有者为 users 群中的 James）

- 列出文件夹内容

  ls 出来最高位解释：

  - d（目录），-（文件），s（套接字），p（（命令）管道文件），l（符号链接文件），b（面向块的设备文件），c（面向字符的设备文件）。

  - ls -tlsa（-t（按照修改时间排序），l（用列表方式表示），s（显示文件大小），a（显示所有，包括隐藏文件）

  - -S（按照大小排序）

- 给文件增加别名（创建符号链接/硬链接）

  - ln cc ccAgain :硬连接；删除一个，将仍能找到；
  - ln -s cc ccTo :符号链接(软链接)；删除源，另一个无法使用；（后面一个ccTo 为新建的文件）


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

15. Google test & gprof

---


### 学习 path

1. 牛客网 Linux C/C++ 学习路线：https://www.nowcoder.com/discuss/193598?type=2&order=0&pos=24&page=2


### 收集的面经

1. 2018 C++开发面试题集合：https://blog.csdn.net/damage233/article/details/81116115
2. 蚂蚁金服C++研发一面：https://blog.csdn.net/weixin_34161064/article/details/87084619
3. 阿里+腾讯C++全部面：https://cloud.tencent.com/developer/article/1112267
3. 整理的C++面试题：https://blog.csdn.net/ljzcome/article/details/574158
4. 牛客2018 BAT华为网易C++：https://www.nowcoder.com/discuss/103939?type=0&order=0&pos=11&page=0
5. BAT面试模块总结：https://mp.weixin.qq.com/s?__biz=MzU2MTkwMTE4Nw==&mid=2247486817&amp;idx=1&amp;sn=d16ee32eb1cb422f39222af29d1a835e&source=41#wechat_redirect
6. 腾讯后台开发实习生要求：https://mp.weixin.qq.com/s?__biz=MzU2MTkwMTE4Nw==&mid=2247486790&amp;idx=1&amp;sn=74c324da3cd3c8da62d261a87bd93ae9&source=41#wechat_redirect
7. 高性能服务器2018总结：https://blog.csdn.net/analogous_love/article/details/86483266
8. 数据库题集：https://blog.csdn.net/si444555666777/article/details/82111355


## 面经总结

#### 1. 腾讯上海IEG天美T1工作室（游戏服务端开发）一面 8月2日

时间：1h

找了学长内推了一波，第二天就找我聊聊。OK... 聊聊，结果直接面了1小时。

（1）为什么出国读研？现在做的方向？

（2）问第一个项目（K8S Spark 分布式系统）

- （i）讲讲 K8S？

- （ii）为什么用 Kafka 作为输入？如何构造的流式输入？

- （iii）说说 Spark？怎么运行在 K8S 上？

- （iv）做性能测试了没有？（没有，笑哭）

（3）问第二个项目（HTTP 服务器）

- （i）为什么做这个项目？是学校的项目吗？（个人的...）

- （ii）为什么不考虑调第三方库？

- （iii）讲讲 Reactor 模式？

- （iv）讲讲 TCP/IP 的 CLOSE_WAIT 和 TIME_WAIT？

- （v）你怎么处理 TIME_WAIT 问题？地址重用可能有什么问题？所以该不该重用？（该不该重用没答好）

- （vi）UDP 了解吗？ 有没有 TCP 粘包问题？（这里聊了游戏中 UDP 和 TCP 应用区别）

- （vii）EPOLL 的 ET 模式你怎么理解？ 和 LT 的性能差异是多少？

- （viii）如何将一个连接绑定到一个工作线程？

- （ix）EPOLLONESHOT 的实现原理？

- （x）压力测试结果？

- （xi）为什么你的并发量大了吞吐量反而小了？（我：xxxx）

- （xii）CPU 跑满了吗？

- （xiii）定时器怎么实现的？

（4）问点 C++ 吧，多态怎么理解的？

（5）虚函数实现机制？

（6）子类指针赋值给父类指针，发生了什么？

（7）两个父类中有同名虚函数，子类重写了这个函数，发生了什么？调用这个函数时从哪个表调用？

（8）子类继承两个基类，其对象里有几个虚函数表？

（9）问个算法题吧，定时器怎么管理的，啥数据结构？

（10）想在哪里工作？你还有啥问题？

“聊聊” 结束，感觉的确没那么严肃，像是简历面... 之后面试官让我找个时间去现场。

#### 2. 阿里云（基础平台研发 C++） 一面 8月4日

时间：2h19min

找的牛客网的老哥内推的，感觉真的很靠谱。内推开放后很快就面试了。可以说是我经历最长的一面...

面试官人很好，很有耐心，我有不会他也给了我解答。

（1）聊简历，将实习项目，本科和现在做的研究（大概半小时，每一条都问的挺细致的）

（2）聊竞赛经历（起码半小时）

（3）给了一个场景题，考查整体的设计，debug 以及分析思维。在全国性的 CDN 网络中，某一天所有用户的平均建连延迟上升了几百ms，如何查出问题来源？假设数据流是日志打点信息。（这一块聊了很久，感觉也有半小时，由于我没有接触过真实的 CDN 监控平台，所以就说了一些我自己的想法，也不知道答得如何... 笑哭）

（4）问基础（乱序了.. 没记住顺序）

- （i）进程虚拟内存分布说一下？

- （ii）sizeof 空类是多少，为什么？

- （iii）TCP 三次握手和四次挥手说一下？

- （iv）C++ 虚函数的底层实现机制？

- （v）指针和引用的区别？

- （vi）epoll 和 select 的区别？

- （vii）服务器中有很多处在 CLOSE_WAIT 状态的连接，原因是什么？

- （viii）在构造函数中 new 多个成员变量，如何防止内存泄露？

- （ix）static 的作用？

- （x）malloc 和 new 的区别？

- （xi）进程间通信的方式？（8 大方式）

- （xii）Unix 域套接字？（PF_UNIX）

- （xiii）memmove 和 memcpy 的内存覆盖问题？（这个记反了，笑哭）

- 这一块我在回答的时候都尽量全面了，所以覆盖了一些其他可能会问的问题，比如智能指针、brk/sbrk、epoll 底层原理、select 底层原理、引用的底层实现、内核栈的分布、内核态用户态等。

（5）问算法

- （i）自己给出一棵双层红黑树，描述插入一个节点的细节，并给出最终树的形状。

（6）平时用的操作系统和编程语言

（7）意向的工作城市

（8）反问面试官问题

- （i）阿里对开源的态度是？（聊了聊 Tengine、Blink）

- （ii）团队规模、主要业务？

- （iii）现在需要什么样的人才？
