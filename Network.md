# Network


## Q：访问一个网页的全过程

参考1：https://blog.csdn.net/weibo1230123/article/details/82899205

1. **DNS解析：将域名转换成对应的IP地址**

（1）搜索浏览器自身缓存的DNS记录

（2）浏览器缓存没找到/记录已经过期，则搜索hosts文件（Linux下在/etc/hosts）

（3）如果hosts文件中没有/已经过期，则向DNS服务器发送解析请求

（4）如果DNS服务器也搜不到，则开始递归+迭代解析

-> 这里的递归+迭代解析，假设要解析 mail.google.com：

首先会向全球只有13台的根域服务器发出请求，显然，仅凭13台服务器不可能把全球所有IP都记录下来。所以根域服务器记录的是com域服务器的IP、cn域服务器的IP、org域服务器的IP。如果我们要查找.com结尾的域名，那么我们可以到com域服务器去进一步解析。根域服务器告诉我们com域服务器的IP。

接着我们的域名解析服务器会向com域服务器发出请求。根域服务器并没有mail.google.com的IP，但是却有google.com域服务器的IP。我们的域名解析服务器会向google.com域服务器发出请求。如此重复，直到获得mail.google.com的IP地址。

为什么是递归：问题由一开始的本机要解析 mail.google.com 变成域名解析服务器要解析 mail.google.com，这是递归。
为什么是迭代：问题由向根域服务器发出请求变成向com域服务器发出请求再变成向google.com域发出请求，这是迭代。

（5）获取域名对应的IP后，递归返回给浏览器

2. **Client发起TCP连接请求**

浏览器会选择一个大于1024的本机端口向目标IP地址的80端口发起TCP连接请求。经过标准的TCP握手流程，建立TCP连接。

3. **Client发起HTTP请求**

按照HTTP协议标准发送一个索要网页的请求。

4. **Server处理后返回响应报文**

（1）可能会与负载均衡，如Nginx服务器（一款面向性能设计的HTTP服务器）。

如果我们的平台配备了负载均衡的话，前一步DNS解析获得的IP地址应该是我们Nginx负载均衡服务器的IP地址。所以，我们的浏览器将我们的网页请求发送到了Nginx负载均衡服务器上。

Nginx根据我们设定的分配算法和规则，选择一台后端的真实Web服务器，与之建立TCP连接、并转发我们浏览器发出去的网页请求。

Web服务器收到请求，产生响应，并将网页发送给Nginx负载均衡服务器。

Nginx负载均衡服务器将网页传递给filters链处理，之后发回给我们的浏览器.

5. **浏览器渲染**

（1）浏览器根据页面内容，生成DOM Tree。根据CSS内容，生成CSS Rule Tree(规则树)。调用JS执行引擎执行JS代码。

（2）根据DOM Tree和CSS Rule Tree生成Render Tree(呈现树)。

（3）根据Render Tree渲染网页。

在浏览器解析页面内容的时候，发现页面引用了其他未加载的image、css文件、js文件等静态内容，可能会访问CDN。

（4）网页静态资源加载：

什么是CDN？如果我在广州访问杭州的淘宝网，跨省的通信必然造成延迟。如果淘宝网能在广东建立一个服务器，静态资源我可以直接从就近的广东服务器获取，必然能提高整个网站的打开速度，这就是CDN。CDN叫内容分发网络，是依靠部署在各地的边缘服务器，使用户就近获取所需内容，降低网络拥塞，提高用户访问响应速度。

接下来的流程就是浏览器根据url加载该url下的图片内容。本质上是浏览器重新开始第一部分的流程，所以这里不再重复阐述。区别只是负责均衡服务器后端的服务器不再是应用服务器，而是提供静态资源的服务器。


## Q：HTTP 请求报文

参考1：https://blog.csdn.net/a_fool_program_ape/article/details/81748544

1. **组成**

（1）请求行（request line）

（2）请求头部（header）

（3）空行（CR LF）

（4）请求数据部分

2. **请求行**

由 请求方法，URL，HTTP协议 三个字段组成，用空格隔开。

（example：GET /index.html HTTP/1.1）

![](imgs/http_request_method.png)

3. **请求头部**

由键/值对组成，每行一对，用冒号隔开。典型的常用请求头部：

User-Agent：产生请求的客户端类型。

Accept：客户端可识别的内容类型列表。

Host：请求的主机名，允许多个域名同处一个IP地址，即虚拟主机。

Accept-Charset： 通知服务端可以发送的编码格式

Accept-Encoding： 客户端使用的编码环境和编码方式

Accept-Language： 客户端语言环境

Authorization：授权信息，一般用于存放授权之后的信息

Connection：表示是否需要持久连接

Content-Length：表示请求数据正文的长度

Cookie：这个与服务器端做交互的，存储保持会话的重要信息

Referer：表示该次请求的来源，一般用于做防盗链


## Q：HTTP响应报文

参考1：https://blog.csdn.net/a_fool_program_ape/article/details/81748544

1. **组成**

（1）状态行（status line）

（2）响应头部

（3）空行

（4）响应数据部分

2. **状态行**

由 HTTP版本，状态码，解释状态码的简单短语 组成。

(HTTP_version, status_code, reason_phrase, CRLF)

•• 1xx：指示信息--表示请求已接收，继续处理。

•• 2xx：成功--表示请求已被成功接收、理解、接受。

•• 3xx：重定向--要完成请求必须进行更进一步的操作。

•• 4xx：客户端错误--请求有语法错误或请求无法实现。

•• 5xx：服务器端错误--服务器未能实现合法的请求。

常见状态代码、状态描述的说明如下。

•• 200 OK：客户端请求成功。

•• 301 Moved Permanently：客户端啊，你要请求的资源已经永久的搬家了，我把他的新地址放到了Location头部域中了；

••302 Moved Temporarily：客户端啊，你要请求的资源临时有事去别的地方了，我把他的位置放到了Location头部域中了，你可以先去那里找他，不过他应该是会回到他自己的家的；

••304 Not Modified：客户端啊，你要请求的资源自从上次你请求之后，就再也没有改动过，我想你是应该早就有这个资源了，所以在响应报文的数据部分我也没有再放这个资源。

•• 400 Bad Request：客户端请求有语法错误，不能被服务器所理解。

•• 401 Unauthorized：请求未经授权，这个状态代码必须和WWW-Authenticate报头域一起使用。

•• 403 Forbidden：服务器端顺利收到了客户端的请求，但是因为某些理由，服务器端拒绝为他提供服务

•• 404 Not Found：请求资源不存在，八成是资源地址写错了；举个例子：输入了错误的URL。

•• 500 Internal Server Error：服务器发生不可预期的错误。

•• 502 Bad Gateway：客户端你好，我是请求报文的代理服务器，持有资源的那个服务器在给我发送资源时出问题了；

•• 503 Server Unavailable：服务器当前不能处理客户端的请求，一段时间后可能恢复正常

3. **响应头部**

常见的：

Server：服务器应用程序软件的名称和版本

Content-Type：响应正文的类型（图片/二进制串）

（如：Content-Type:text/html;charset=ISO-8859-1）

Content-Length：响应正文的长度

Content-Charset：响应正文使用的编码

Content-Encoding：响应正文使用的数据压缩格式

Content-Language：响应正文使用的语言

4. **例子**

（1）HTTP请求报文例子

GET

```
GET /search?hl=zh-CN&source=hp&q=domety&aq=f&oq= HTTP/1.1
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint,
application/msword, application/x-silverlight, application/x-shockwave-flash, */*
Referer: <a href="http://www.google.cn/">http://www.google.cn/</a>
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)
Host: <a href="http://www.google.cn">www.google.cn</a>
Connection: Keep-Alive
Cookie: PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM=1261551917:S=ybYcq2wpfefs4V9g;
NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_yMfO2M4QMRKcH1g0iQv9u-2hfBW7bUFwVh7pGaRUb0RnHcJU37y-
FxlRugatx63JLv7CWMD6UB_O_r
```

POST

```
POST /search HTTP/1.1
Accept: image/gif, image/x-xbitmap
Connection: Keep-Alive

username=zhangsan&password=123
```


（2）HTTP响应报文例子

```
HTTP/1.1 200 OK
Date: Sat, 31 Dec 2005 23:59:59 GMT
Content-Type: text/html;charset=ISO-8859-1
Content-Length: 122

<html>
<head>
<title>Wrox Homepage</title>
</head>
<body>
<!-- body goes here -->
</body>
</html>

```

## Q：HTTP 常见问题

参考1：https://blog.csdn.net/a_fool_program_ape/article/details/81748544

1. **HTTP协议无状态**

HTTP是无状态的面向连接的协议，其对事务处理没有记忆能力。但是无状态不代表HTTP不能保持TCP连接。

从HTTP/1.1起，默认都开启了Keep-Alive，保持连接特性，简单地说，当一个网页打开完成后，客户端和服务器之间用于传输HTTP数据的TCP连接不会关闭。但有一个保持时间。

2. **HTTP与TCP的关系**

HTTP属于应用层协议，TCP属于传输层协议，IP属于网络层协议。

HTTP协议的长连接和短连接，实质上是TCP协议的长连接和短连接。

关于长连接：

参考1：https://www.cnblogs.com/gotodsp/p/6366163.html

```
我们再模拟一下长连接的情况：client向server发起连接，server接受client连接，双方建立连接。

client与server完成一次请求后，它们之间的连接并不会主动关闭，后续的读写操作会继续使用这个连接。
\\

TCP的保活功能主要为服务器应用提供。如果客户端已经消失而连接未断开，则会使得服务器上保留一个半开放的连接.
而服务器又在等待来自客户端的数据，此时服务器将永远等待客户端的数据。
保活功能就是试图在服务端器端检测到这种半开放的连接。
\\
如果一个给定的连接在两小时内没有任何动作，服务器就向客户发送一个探测报文段，根据客户端主机响应探测4个客户端状态：
\\
客户主机依然正常运行，且服务器可达。此时客户的TCP响应正常，服务器将保活定时器复位。

客户主机已经崩溃，并且关闭或者正在重新启动。上述情况下客户端都不能响应TCP。服务端将无法收到客户端对探测的响应。

服务器总共发送10个这样的探测，每个间隔75秒。若服务器没有收到任何一个响应，它就认为客户端已经关闭并终止连接。

客户端崩溃并已经重新启动。服务器将收到一个对其保活探测的响应，这个响应是一个复位，使得服务器终止这个连接。

客户机正常运行，但是服务器不可达。这种情况与第二种状态类似。
```

3. **GET与POST**

GET 参数追加在 URL 后，长度受限制，不安全。

POST 参数在请求报文的请求数据部分，长度比GET的限制长（在HTTP/1.1中无限制，但在服务器会限制），安全。


## Q：socket 是什么

参考1：https://www.jianshu.com/p/b68d2b26f5f4

Socket是应用层与TCP/IP协议族通信的中间软件抽象层，它是一组接口。在设计模式中，Socket其实就是一个门面模式，它把复杂的TCP/IP协议族隐藏在Socket接口后面，对用户来说，一组简单的接口就是全部，让Socket去组织数据，以符合指定的协议。

![](socket.png)

## Q：MIME

- **概念**

全称为：Multipurpose Internet Mail Extensions，多用途互联网邮件扩展。是一种应用广泛的电子邮件技术规范。（最初的目的是为了在发送电子邮件时附加多媒体数据。）

在HTTP中，MIME类型被定义在Content-type header中。

- **类型与格式**

MIME类型由两个部分组成：（1）数据的大类别 （2）具体的种类

常见的MIME类型：

（1）超文本标记语言文本 .html： text/html

（2）普通ASCII文本 .txt： text/plain

（3）RTF文本 .rtf： application/rtf

（4）GIF图像 .gif： image/gif

（5）JPEG格式的图片 .jpeg： image/jpeg

（6）MS的powerpint文件 .ppt： application/vnd.ms-powerpoint

（7）au声音文件 .au： audio/basic

（8）MPEG文件 .mpeg： video/mpeg

（9）AVI文件 .avi： video/x-msvideo

（10）GZIP文件 .gz： application/x-gzip

（11）TAR文件 .tar： application/x-tar

（12）ReadAudio音乐文件 .ra/.ram： audio/x-pn-realaudio

（13）MIDI音乐文件 .midi： audio/x-midi
