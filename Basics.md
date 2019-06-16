# Basics Questions
> 计算机大基础面试问题

---

### Linux 进程间通信（IPC：Inter-Process Communication）大概8种

- **管道（PIPE）**

管道实际上是一段共享内存，创建者：管道服务器，连接到的：管道客户端。

（1）管道是半双工的，数据只能向一个方向流动。

（2）只能用于父进程或者兄弟进程之间（有亲缘关系），比如fork或者exec创建的进程。

（3）管道是一个文件（fd），但不属于某种文件系统，自立门户，仅存在于内存中。

（4）管道是由内核管理的一个缓冲区，它被设计成环状的数据结构，以便循环利用。

- **命名管道（FIFO）**

（1）是一种特殊类型的文件，在系统中以文件形式存在。

（2）允许没有亲缘关系的进程之间通信。

- **信号（Signal）** 

（1）unix中最为古老的IPC方式，用于进程间传递异步信号，比如键盘中断。

（2）用signal函数，或sigaction函数（更健壮）


- **信号量（Semaphore）XSI IPC**

（1）一种计数器，用于控制多个进程共享资源的访问。常常被用作锁。

（2） 信号量是特殊的变量，只取非负整数。

（3）只允许对值做两种操作：P（等待，wait）和V（信号，signal）。

（4）P相当于申请资源，信号量值>0，则-1，若信号量=0，则该进程挂起。

（5） V相当于释放资源，有进程等待就让它恢复，否则让信号量+1。

- **消息队列 XSI IPC**

（1）实际上时内核地址空间中的链表，它有消息队列标识符（queue ID）。

（2）每个节点是一个结构体msgbuf，发送msgsnd，接受msrcv，对每个数据都有最大长度限制。

（3）相比于命名管道，优势1：消息队列可以独立于发送/接受进程而存在。

（4）优势2：避免同步和阻塞的问题，不用进程自己来提供同步方法。

（5）优势3：接受程序可以有选择的接受数据（通过消息类型）。


- **共享内存（Share Memory）XSI IPC**

（1）在多个进程间共享一块内存区域，是IPC最快捷的方式。

（2）会被映射到进程的私有地址空间中，供直接读写。

（3）需要进程自己实现同步机制（比如用互斥锁，信号量）。

---
<font color=red>**Summary of XSI IPC**</font>

这里的信号量，消息队列和共享内存属于XSI IPC。

（1）这里的XSI IPC（System Interface and Headers）代表一种Unix标准，其依托标识符和键来实现。

（2）内核为每个XSI IPC分配一个结构体形式的IPC对象，有一个非负整数标识符（内部名称）。

（3）XSI IPC键值是其外部名称，用于多进程在同一个XSI IPC上汇合。

（4）这类IPC没有文件描述符，不能对它们用多路复用I/O，如select，poll。不能用ls查看，不能用rm删除不能用chmod，只能用ipcs和ipcrm来查看，删除。


---


- **内存映射（Memory Map）**

（1）内存映射文件，是由一个文件到一块内存的映射。（与虚拟内存有些类似）

（2）多个进程通过把同一个共享文件映射到自己的进程地址空间来实现多个进程间的通信。

（3）处理大文件时效率较高，直接从硬盘拷贝到用户空间，而不用经过内核空间的缓冲区。



- **套接字（Socket）**

（1）套接字既可以单机内不同进程间通信，也可以让跨网机器间通信。

（2）与管道的区别：将服务器与客户端明确区分出来。可实现一服务器，多客户端。




### Linux 启动流程
- **BIOS （Basic Input/Output System executes MBR）**

（1）开机，CPU查看ROM，通过JUMP命令启动BIOS。

（2）BIOS列出所有可用的启动设备（比如硬盘，U盘等）。

（3）通常从硬盘启动，硬盘包含MBR。

- **MBR（Master Boot Record executes GRUB）**

（1）硬盘中最初的512字节。

（2）最初的434～446字节包含primary boot loader。

（3）64字节分区表（partitio table）。

（4）6字节验证时间戳（validation timestamp）。

（5）启动GRUB

- **GRUB（GRand Unified Boot Loader executes Kernel）**

（1）根据/boot/grub/grub.conf文件，载入文件系统和其驱动。

（2）载入/boot下的内核文件。

- **Kernel（Kernel executes /sbin/init）**

（1）运行init进程。

- **Init Process（Init Process executes runlevel programs）**

（1）读取配置文件/etc/inittab。

（2）设置系统的运行级别（runlevel）。

（3）启动一些开启启动的程序（如daemon）。

- **Runlevel（Runlevel programs are executed from /etc/rc.d/rc*.d）**

0 - halt，系统停机状态。

1 - Single user mode，单用户工作状态，root权限，用于系统维护，禁止远程登录。

2 - Multiuser，多用户状态（无NFS）。

3 - Full multiuser mode，完全多用户状态（有NFS），登录后进入控制台命令行模式。

4 - unused，系统未使用，保留。

5 - X11，X11控制台，登陆后进入图形GUI模式。

6 - reboot，系统正常关闭并重启。



### Linux fork函数

Linux 实现进程的原理，分两个步骤：

（1) 在内存中复制父进程，得到子进程，此时子进程就是父进程上下文的简单克隆，内容完全一致。

（2) 设置子进程PID，PPID等和父进程不一样的内容。


### Linux 分页机制


