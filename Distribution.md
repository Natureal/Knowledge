# Distribution Questions
> 分布式相关面试题（Docker，Hadoop，Spark，Kubernetes，Kafka，etc）

---

### Doker底层实现隔离的机制
		- 一个正在运行的Docker容器，实际上就是启用了多个Linux Namespace的进程，而这个进程能够使用的资源量，由Cgroups配置的限制。
		- 容器技术中非常重要的概念：容器时一个“单进程”模型。敏捷和高性能时容器比较于虚拟机最大的优势，在Paas（Platform-as-a-Service）上大行其道。但其缺点就是隔离得不彻底。
		- 多个容器之间共享宿主的操作系统内核。所以不能在windows宿主上运行Linux容器，低版本的Linux宿主上也不能运行高版本的Linux容器。而且，有很多资源和对象是不能被Namespace化的，最典型的例子：时间。
		- 容器本身的设计：希望容器和应用能够同生命周期。
		- Cgroups对资源的限制有很多不完善的地方，典型的就行 /proc 文件系统的问题：
		在容器里执行top命令，会显示宿主的CPU和内存数据，而不是当前容器的数据。原因：/proc文件系统并不知道用户通过Cgroups给这个容器做了什么资源限制，即：它不了解Cgroups限制的存在。
- **隔离性Summary**
		通过Namespace来进行隔离，Cgroups进行资源限制，Capability进行权限限制
1. Namespace隔离
		（1）UTS命名空间负责主机名与域名的隔离，让容器有自己的主机名和域名，可被看做一个独立的网络节点
		（2）IPC命名空间负责信号量，消息队列和共享内存的隔离，