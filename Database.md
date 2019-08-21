# Database Questions
> 数据库面试题总结

---
## Q：MySQL ACID

- Atomicity（原子性）

- Consistency（一致性）

- Isolation（隔离性）

- Durability（持久性）-> 事务完成后，更新被保存，则不能回滚。

## Q：MySQL不可重复读 & 幻读

- 不可重复读（侧重修改）-> 锁行

- 幻读（侧重新增/删除）-> 锁表

## Q：MySQL中的锁（如：表锁，行锁，共享锁，排它锁，间隙锁）

- 排它锁（写锁，X锁）

会阻塞其他事务的读和协，属于行锁。

- 共享锁（读锁，S锁）

会阻塞其他事务修改表数据，属于行锁。

- 间隙锁

在记录之间的锁。

（1）使用唯一索引来搜索唯一行的语句不需要用间隙锁。

（2）当没有建立索引或建立的是非唯一索引时，会有间隙锁（且，当有多个查询条件时，必有间隙锁）。

- 意向锁（如IS，intention shared locks）

意向锁协议：事务想获取某行的S/X锁，必须现获取IS/IX锁，意向锁之间不互斥，可并行。

- 插入意向锁（Gap locks的一种）

针对insert的锁，多个事务在同一个索引下，当插入位置不同时，不会互相阻塞。

- 临键锁（Next-key locks）

封锁记录本身+索引区间，从而避免幻读。

幻读：某次select操作得到的结果数据无法支持后续的业务操作。

- One more thing

select * from table for update 会加读锁。

select * fron table 则是快照读（Snapshot Read），不会加锁。


**按照数据粒度分：**

-  行锁（记录锁）

开销大，加锁慢，会出现死锁，粒度最小，锁冲突概率低，并发度最高。

- 表锁

开销小，加锁快，不会死锁，粒度最大，锁冲突概率高，并发度低。

- 页锁（MySQL特有）

各项指标处于行锁和表锁之间，会死锁。

## Q：MySQL事务隔离级别

1. 读未提交（read-uncommitted）

可读取未提交的数据（脏读，幻读，不可重复度）。

2. 不可重复读/读提交（read-committed）

不可重复读，幻读。

3. 可重复读（repeatable-read）

MySQL默认的方式，使用MVCC（Multiversion Currency Control）机制，存在幻读。

4. 可串行化（serializable）

Notice: 隔离级别越高，越能保证完整性和一致性，但并发性能越差。

## Q：MySQL索引的实现机制

首先，大部分数据库以及文件系统都采用B/B+树来实现索引。

- **B Tree**

1. 每个非叶子节点最多由m-1个key和m个指针组成。（key升序排序）。

2. 所有叶子节点都在同一层。

- **B+ Tree （MySQL普遍使用）**

与B Tree的区别：

1. 有k个子节点必然会有k个key。

2. 非叶节点仅具有索引功能，与记录有关的信息全部存放在叶子节点中。

3. 所有叶节点构成一个有序链表，可按照key的次序遍历全部记录。

**B+ Tree的优缺点：**

优点：

更省空间（因为非叶节点仅作向导），有更好的空间局部性，更高的缓存命中率，有利于遍历，区间查找，搜索。

缺点：

不像B Tree那样，有些经常访问的数据如果离根较近，则效率比较高。

## Q：死锁的产生，预防和解决

**四个必要条件：**

1. 互斥：一个资源每次只能被一个进程占有。

2. 请求与保持：请求其他资源时，进程会保持自己原先持有的资源。

3. 不可剥夺：资源只能由进程自己主动释放。

4. 循环等待：若干进程首尾相接循环等待资源。

**避免死锁的方法**

1. 破坏请求与保持

（1）一次性申请全部所需的资源。

（2）请求资源前，释放原先保持的资源。

2. 破坏不可抢占

（1）申请资源被拒绝的话，则释放自己持有的资源

（2）根据进程优先级，可以抢占资源。

3. 破坏循环等待

（1）对资源进行线性排序，申请资源时按递增顺序进行。

**解决死锁的方法**

（1）抢占资源

（2）终止/撤销进程

（3）银行家算法
南方
## Q：MySQL 端口号，如何修改

1. 查看：show global variables like 'port';（MySQL默认3306，SQLServer默认1433，Oracle默认1521，DB2默认5000）

2. 修改：编辑 /etc/my.cnf

## Q：SQL 语言总结

- 1. 数据查询语言（DQL）

SELECT，保留字：WHERE，ORDER BY，GROUP BY，HAVING。

- 2. 数据操作语言（DML，Manipulation）

INSERT，UPDATE，DELETE。

- 3. 事务处理语言（TPL）

用于确保被 DML 影响的表的所有行能够及时得到更新。 BEGIN TRANSACTION，COMMIT，ROLLBACK。

- 4. 数据控制语言（DCL）

GRANT，REVOKE，确定单个用户/用户组对数据库（或者列）的访问权限。

- 5. 数据定义语言（DDL）

CREATE（创建表），DROP（删除表），ALTER（在已有表中添加、修改、删除列）。

- 6. 指针控制语言（CCL）

DECLARE CURSOR，FETCH INTO，UPDATE WHERE CURRENT。

## Q：SQL 数据类型

1. 字符型（VARCHAR / CHAR）

VARCHAR 会自适应长度

2. 文本型（TEXT）

适合存储大串的字符

3. 数值型（INT，NUMERIC（小数），MONEY（专门用来存储钱数））

4. 逻辑型（BIT）

只能取 0 或 1

5. 日期型（DATETIME，SMALLDATETIME）

DATETIME：1753.1.1 -> 9999.12.31 （毫秒级）

SMALLDATETIME：1900.1.1 -> 2079.6.6 （秒级）

## Q：为什么平衡二叉树不适合作为索引

因为索引是存在于磁盘中的，索引通常很大，因此无法一次将全部索引加载到内存中，每次只能从磁盘中读取一个页。平衡树在实现上是通过数组，其逻辑上相近的节点在物理距离上可能差距很远，导致其局部性很差。

## Q：为什么 B tree 适合用来作为索引

因为 B 树充分利用了磁盘预读功能和局部性原理，也就是说 B 树是为了索引才被发明出来的。

（解释：由于磁盘顺序读取的效率很高，不需要寻道，只需要很少的旋转时间，因此对于有局部性的程序来说，预读可以提高 IO 效率。）

B 树的每个节点可以存储多个关键字，如果将他设置为磁盘页的大小，则可以充分利用磁盘预读的功能（每次读取一个完整的节点），同时，B 树的深度也比平衡二叉树小，因此需要进行的磁盘 IO 次数就相对对少，其更多的是在内存中进行数据查找，效率很高。

B 树的查询次数不比平衡二叉树少，但是磁盘 IO 次数少。

## Q：为什么 B+ tree 比 B tree 更适合作为索引

B树：有序数组 + 平衡 K 叉树

B+树：有序数组链表 + 平衡 K 叉树

B+树的 value 全部存放在叶子节点中，非叶子节点用来做索引，同时叶子节点串接成一个链表（提高范围询问性能，**这是 B+ 优于 B 树的最大原因**）。

但是 B 树的总体高度比 B+ 树矮，因此单个查询有一丢丢的优势。因此许多基于频率的搜索是基于 B 树，越频繁被查询的 value 越往根上走。

总结：B 和 B+ 树，反复被查询的节点基本都在内存中，很少出现磁盘 IO。

## Q：MySQL 为什么用 B+ tree 而不用 RB tree

B+ 树的深度比红黑树小，不会造成频繁的磁盘 IO。

B+ 树的节点大小可以被设计为1页，这样依次 IO 可以完全读入一个节点。

## Q：AVL 树和红黑树

AVL 对平衡性的要求更高，插入、删除所需要的旋转更多。适用于插入和删除比较少，但查找多的情况。

红黑树是一种弱平衡树，其深入可能会比 AVL 深（但不超过 2log(n)），但其平衡性要求较低，需要的旋转、改色操作次数较少。所以对于插入、删除较多的情况，可以采用红黑树。

红黑树应用场景：

（1）STL 中的 map 和 set。

（2）Linux CFS 进程调度，用红黑树管理进程控制块 PCB。

（3）Linux 进程虚拟内存区域也有红黑树作为索引。（实际上为红黑线索树）

（4）EPOLL 底层用红黑树管理 fd。

（5）Nginx 中用红黑树管理 timer。

（6）Java 中 TreeMap 底层实现为红黑树。


## Q：SQL 常见考题

给出一个表 student，三列：name, course, score。

1. 查询每门课都大于 80 分的学生姓名

```
SELECT name
FROM student
GROUP BY name
HAVING MIN(score) > 80
```

方法2：用 not in 容斥一下

```
SELECT DISTINCT name
FROM student
WHERE name NOT IN
  (SELECT DISTINCT name
   FROM student
   WHERE socre <= 80);

```

2. 查询每门课都大于 80 分，且至少选修 3 门课的学生姓名

```
SELECT name
FROM student
GROUP BY name
HAVING MIN(score) > 80
AND COUNT(course) >= 3
```

3. 查询出 ”张” 姓学生中平均成绩大于 75 分的学生信息

SQL 通配符：（1）% 替代 0 或多个字符。（2）- 替代一个字符。（3）[charlist] 字符列表中任意单个字符。（4）[^charlist] 或 [!charlist] 不在字符列表中的任意单个字符。

```
SELECT *
FROM student
WHERE name in
  (SELECT name FROM student
   WHERE name like '张%'
   GROUP BY name
   HAVING avg(score) > 75);

```

方法2：用 REGEXP 正则匹配

（正则式里 ^ ：匹配名字的开始。）

```
SELECT *
FROM student
WHERE name in
  (SELECT name FROM student
    WHERE name REGEXP '^[张李]'
    GROUP BY name
    HAVING avg(score) > 75)
```
