## RTMP

#### Message

- **Message Header**: 消息头部

  - Message Type: 消息类型

    - **Type of Messages**

      - Command Message: 控制信息

        - NetConnection: 建立连接和流

          - connect / call / close / createStream

        - NetStream: 对流的操作

          - play / play2 / deleteStream / closeStream

          - receiveAudio / receiveVideo

          - publish / seek / pause

      - Data Message: 数据信息

      - Shared Object Message

      - Audio Message: 音频数据

      - Video Message: 视频数据

      - Aggregate Message

      - User Control Message Events: 用户控制信息

        - Stream Begin/EOF/Dry/IsRecorded

        - SetBuffer Length

        - Ping Request/Response: Ping保活信息

  - Payload Length

  - Timestamp

  - Stream ID

- **Message Payload**: 有效负载


#### Chunk

- **Chunk Header**: 块头部

  - **Basic Header**: 1-3 byte(s), encodes the chunk stream ID and chunk type. Chunk type determines the format of the encoded message header. (fmt field) 包含了流ID和块类型，块类型决定了消息头部的格式。

    - fmt: identifies one of four format used by the 'chunk message header'.

    - cs id: chunk stream id

  - **Message Header**: one of four format selected by the 'fmt' field. 头部格式由块类型决定

    - **Type 0**: 11 bytes, MUST be used at the start of a chunk stream and whenever the stream timestamp goes backward. 必须用来块流的开始阶段。

      - timestamp + message length + message type id + message stream id

    - **Type 1**: 7 bytes, timestamp delta + message length + message type id. The stream ID is the same as the preceding chunk.

    - **Type 2**: 3 bytes, only timestamp delta. The stream ID and message length are the same as the preceding chunk.

    - **Type 3**: no message header. The stream ID, message length, and timestamp delta fields are the same as the preceding chunk for the same Chunk Stream ID. A single message should be split into one chunk of type 0 and other chunks of type 3.

  - **Extended Timestamp**: Used to encode timestamps or timestamp deltas that are greater than 16777215(0xFFFFFF)

- **Chunk Data**: 块数据

#### Protocol Control Messages: 协议控制信息

- Set Chunk Size: 32 bits, 31 bits for chunk size. : 设置块大小

- Abort Message: 32 bits for chunk stream id. : 中止信息

- Acknowledgement : 确认

- Window Acknowledgement Size : 窗口大小确认

- Set Peer Bandwidth : 设置带宽


#### C0 and S0: 1 byte

- **version**: 8 bits

#### C1 and S1: 1536 bytes

- **time**: 4 bytes

- **Zero**: 4 bytes

- **Random data**: 1528 bytes, sufficiently random to distinguish different handshake phases.

#### C2 and S2: 1536 bytes

- **time**: 4 bytes

- **time2**: 4 bytes

- **Random echo**: 1528 bytes, 和收到的S1/C1一致。


---

## ffmpeg 常用参数记录

- -f [fmt]：指定格式（音频或视频格式）

- -i [filename]：指定输入的文件名，在Linux中也能指定屏幕录制和摄像头

- -re：以本地帧频读数据，主要用于模拟捕获设备

- acodec [codec]：指定音频编码

- vcodec [codec]：指定视频编码

- ab [bitrate]：设置音频码率

- ar [freq]：设置音频采样率

- -stream_loop -1：循环推流

- 推流命令样例: ffmpeg -re -i sample.mp4 -vcodec copy -acodec copy -f flv "rtmp://localhost:1935/mytv"

- 拉流命令样例: ffplay -i "rtmp://localhost:1935/kaishi"

---

<!--
==================================================================================================================
-->

## Nginx 小知识

- 查询nginx加载的模块: objs/ngx_module.c

- NGX_CODE_MODULE类型的module:

  - ngx_core_module

  - ngx_errlog_module

  - ngx_events_module

  - ngx_http_module

- (void) ngx_process_events(cycle, timer, flags); 在前面加上void强制类型转换是了告诉编译器“我决定忽略这次调用的返回值，请继续对于其他忽略返回值的情况提出的警告。”，因为有些编译器和lint对于被丢弃的返回值会warning。

- F_GETFD    取得与文件描述符fd联合的close-on-exec标志，类似FD_CLOEXEC。主要用于关闭worker的写端。

<!--
==================================================================================================================
-->

---

## 开源 Nginx with RTMP 核心数据结构

<details>
<summary>
<b> ngx_rtmp_core_main_conf_t : rtmp core 模块主配置 </b>
</summary>

```cpp
type = struct {
    ngx_array_t servers;
    ngx_array_t listen;
    ngx_array_t events[26];
    ngx_hash_t amf_hash;
    ngx_array_t amf_arrays;
    ngx_array_t amf;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_core_src_conf_t : rtmp core 模块服务器配置 </b>
</summary>

```cpp
type = struct ngx_rtmp_core_srv_conf_s {
    ngx_array_t applications;
    ngx_msec_t timeout;
    ngx_msec_t ping;
    ngx_msec_t ping_timeout;
    ngx_flag_t so_keepalive;
    ngx_int_t max_streams;
    ngx_uint_t ack_window;
    ngx_int_t chunk_size;
    ngx_pool_t *pool;
    ngx_chain_t *free;
    ngx_chain_t *free_hs;
    size_t max_message;
    ngx_flag_t play_time_fix;
    ngx_flag_t publish_time_fix;
    ngx_flag_t busy;
    size_t out_queue;
    size_t out_cork;
    ngx_msec_t buflen;
    ngx_rtmp_conf_ctx_t *ctx;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_core_app_conf_t : rtmp core 模块应用配置</b>
</summary>

```cpp
type = struct {
    ngx_array_t applications;
    ngx_str_t name;
    void **app_conf;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_relay_app_conf_t : rtmp relay 模块应用配置</b>
</summary>

```cpp
type = struct {
    ngx_array_t pulls;
    ngx_array_t pushes;
    ngx_array_t static_pulls;
    ngx_array_t static_events;
    ngx_log_t *log;
    ngx_uint_t nbuckets;
    ngx_msec_t buflen;
    ngx_flag_t session_relay;
    ngx_msec_t push_reconnect;
    ngx_msec_t pull_reconnect;
    ngx_rtmp_relay_ctx_t **ctx;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_relay_static_t </b>
</summary>

```cpp
type = struct {
    ngx_rtmp_conf_ctx_t cctx;
    ngx_rtmp_relay_target_t *target;
}
```
</details>



<details>
<summary>
<b> ngx_rtmp_relay_target_t </b>
</summary>

```
type = struct {
    ngx_url_t url;
    ngx_str_t app;
    ngx_str_t name;
    ngx_str_t tc_url;
    ngx_str_t page_url;
    ngx_str_t swf_url;
    ngx_str_t flash_ver;
    ngx_str_t play_path;
    ngx_int_t live;
    ngx_int_t start;
    ngx_int_t stop;
    void *tag;
    void *data;
    ngx_uint_t counter;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_listen_t </b>
</summary>

```cpp
type = struct {
    u_char sockaddr[112];
    socklen_t socklen;
    ngx_rtmp_conf_ctx_t *ctx;
    unsigned int bind : 1;
    unsigned int wildcard : 1;
    unsigned int ipv6only : 2;
    unsigned int so_keepalive : 2;
    unsigned int proxy_protocol : 1;
    int tcp_keepidle;
    int tcp_keepintvl;
    int tcp_keepcnt;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_exec_main_conf_t </b>
</summary>

```cpp
type = struct {
    ngx_array_t static_conf;
    ngx_array_t static_exec;
    ngx_msec_t respawn_timeout;
    ngx_int_t kill_signal;
    ngx_log_t *log;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_exec_t </b>
</summary>

```cpp
type = struct {
    ngx_rtmp_exec_conf_t *conf;
    ngx_log_t *log;
    ngx_rtmp_eval_t **eval;
    void *eval_ctx;
    unsigned int active : 1;
    unsigned int managed : 1;
    ngx_pid_t pid;
    ngx_pid_t *save_pid;
    int pipefd;
    ngx_connection_t dummy_conn;
    ngx_event_t read_evt;
    ngx_event_t write_evt;
    ngx_event_t respawn_evt;
    ngx_msec_t respawn_timeout;
    ngx_int_t kill_signal;
}
```
</details>

<details>
<summary>
<b> ngx_rtmp_session_t </b>
</summary>

```cpp
type = struct {
    uint32_t signature;
    ngx_event_t close;
    void **ctx;
    void **main_conf;
    void **srv_conf;
    void **app_conf;
    ngx_str_t *addr_text;
    int connected;
    ngx_queue_t posted_dry_events;
    uint32_t buflen;
    uint32_t ack_size;
    ngx_str_t app;
    ngx_str_t args;
    ngx_str_t flashver;
    ngx_str_t swf_url;
    ngx_str_t tc_url;
    uint32_t acodecs;
    uint32_t vcodecs;
    ngx_str_t page_url;
    ngx_buf_t *hs_buf;
    u_char *hs_digest;
    unsigned int hs_old : 1;
    ngx_uint_t hs_stage;
    ngx_msec_t epoch;
    ngx_msec_t peer_epoch;
    ngx_msec_t base_time;
    uint32_t current_time;
    ngx_event_t ping_evt;
    unsigned int ping_active : 1;
    unsigned int ping_reset : 1;
    unsigned int auto_pushed : 1;
    unsigned int relay : 1;
    unsigned int static_relay : 1;
    ngx_rtmp_stream_t *in_streams;
    uint32_t in_csid;
    ngx_uint_t in_chunk_size;
    ngx_pool_t *in_pool;
    uint32_t in_bytes;
    uint32_t in_last_ack;
    ngx_pool_t *in_old_pool;
    ngx_int_t in_chunk_size_changing;
    ngx_connection_t *connection;
    ngx_msec_t timeout;
    uint32_t out_bytes;
    size_t out_pos;
    size_t out_last;
    ngx_chain_t *out_chain;
    u_char *out_bpos;
    unsigned int out_buffer : 1;
    size_t out_queue;
    size_t out_cork;
    ngx_chain_t *out[];
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_header_t </b>
</summary>

```cpp
type = struct {
    uint32_t csid;
    uint32_t timestamp;
    uint32_t mlen;
    uint8_t type;
    uint32_t msid;
}
```
</details>


<details>
<summary>
<b> ngx_rtmp_stream_t </b>
</summary>

```cpp
type = struct {
    ngx_rtmp_header_t hdr;
    uint32_t dtime;
    uint32_t len;
    uint8_t ext;
    ngx_chain_t *in;
}
```
</details>

---

<!--
==================================================================================================================
-->

## Nginx-rtmp-module 配置指令详解

```
rtmp_auto_push on|off         #切换自动推送（多worker直播流）模式，默认为off
rtmp_auto_push_reconnect      #当worker挂掉时，自动推送连接的超时事件，默认为100ms
rtmp_socket_dir /var/sock     #设置用于流推送的UNIX域套接字目录，默认为/tmp

rtmp{
  server{
    listen 1935;         #监听端口
    timeout 60s;         #socket超时，默认60秒
    ping 3m;             #RTMP ping的间隔，RTMP ping是用来检查活动连接的协议功能，发送特殊包到对方并等待回复
    ping_timeout 30s;    #在ping_timeout内没收到回复就断连
    max_streams 32;      #RTMP流的最大数目，默认值为32
    ack_window 5000000;  #RTMP确认窗口的大小，默认值为5百万
    chunk_size 4096;     #流整合的最大块大小，默认值为4096.这个值越大，CPU负载越小，不能低于128
    max_message 1M;      #所有输入数据会先被分割为message，再进一步分割为chunk。message处理结束前都在内存中，默认值为1M。

    application myapp{
      live on;                                  #直播模式，一对多广播
      meta off;                                 #关闭发送元数据到客户端，默认为on
      interleave on;                            #开启交叉模式，音频和视频数据会在同一个RTMP chunk流中传输，默认为off
      wait_key on;                              #使视频流从一个关键帧开始，默认为off
      wait_video on;                            #在第一个视频帧发送前禁用音频，默认为off
      publish_notify on;                        #发送NetStream.Publish.Start和NetStream.Publish.Stop给用户，默认为off
      record all;                               #记录流到flv文件中，包括all（音频和视频）
      record audio keyframes;                   #只记录音频关键帧
      record_path /tmp/rec;                     #指定录制的flv文件存放的目录
      record_unique on;                         #添加时间戳到录制文件，默认为off。如果是off，同样的文件在每一次新录制发生时将被重写
      record_append on;                         #录制时将把新数据添加到老文件
      record_max_size 128K;                     #设置录制文件的最大值
      record_max_frames 2;                      #设置每个录制文件的视频帧的最大数量
      record_intervals 1s;                      #录制的时间间隔

      access_log off;                           #关闭访问日志，默认是on
      max_connections 100;                      #为rtmp设置最大连接数

      #allow [play|publish] address|subnet|all  #允许来自指定地址或者所有地址play/publish
      allow publish 127.0.0.1;                  #允许来自127.0.0.1的publish
      deny publish all;
      allow play 192.168.0.0/24;
      deny play all;

      #exec command arg*                        #定义每个流publish时要执行的带有参数的外部命令。
      #command应该是二进制可执行文件的完整路径，这里就可以用ffmpeg。
      #ffmpeg被假定为客户端连接到nginx-rtmp然后作为发布者输出转换流到nginx-rtmp。
      # $name - 流的名字
      # $app  - 应用名
      # $addr - 客户端地址
      # $flashver - 客户端flash版本
      # $swfurl - 客户端swf url
      # $tcurl  - 客户端tc url
      # $pageurl  - 客户端页面url
      # > file  - 截取输出
      # >> file - 附加输出
      # < file  - 输入
      exec ffmpeg -i rtmp://localhost/src/$name -vcodec libx264 -vprofile baseline -g 10 -s 300x200 -acodec libfaac -ar 44100 -ac 1 -f flv rtmp://localhost/hls/$name 2>>/var/log/ffmpeg-$name.log;

      play /var/flvs;                           #播放指定目录或者HTTP地址的flv或者mp4文件。
      play /var/local_mirror http://myserver.com/vod;               #逐个尝试

      pull rtmp://cdn.example.com/main/ch?id=12563 name=channel_a;  #创建pull中继，从远端服务器拉流，成为本地可用，仅当至少有一个播放器正在play本地流时发生。name为绑定到的中继的本地流名字。

      push url [key=value]*                     #push和pull的语法一样
      push_reconnect 1s;                        #在断开连接后，重新push的等待时间，默认为3秒
      on_connect http://example.com/my_auth;    #设置HTTP连接回调，当客户发送连接命令时，发送一个异步的HTTP请求到location /on_connection，处理将暂停到接收到HTTP结果代码，返回2XX码时，RTMP会话继续；返回3XX（重定向）时，RTMP会重定向到另一个从HTTP返回头里获取到的application；否则（其他状态码），丢弃连接。
      on_play http://localhost:8080/local_redirect; #设置HTTP播放回调，每当收到客户的play命令时，发送一个异步HTTP请求，处理会暂停挂起到接收到HTTP结果代码，2XX继续RTMP会话，3XX重定向（流名在HTTP返回头的Localtion中获取）

      recorder audio{                           #创建录制块，可以在单个application中创建多个recorder。
        record audio;                           #记录音频
        record_suffix .audio.flv;               #设置录制文件的后缀名，默认为 .flv
        record_notify on;                       #录制启动或停止时发送NetStream.Record.Start和NetStream.Record.Stop状态信息给发布者，默认为off
      }

    }
  }
}

http{
  server{
    location /stat{
      rtmp_stat all;                            #设置RTMP statistics处理程序，RTMP statistics是一个静态的XML文档
      rtmp_stat_stylesheet stat.xsl;            #在浏览器中作为XHTML页面查看这个文档
    }

    location /stat.xsl{
      root /path/to/stat/xsl/file;
    }

    location /control{                          #为当前HTTP location设置RTMP控制程序
      rtmp_control all;
    }

    location /on_connect{
      if($arg_flashver != "my_secret_flashver"){
        rewrite ^.*$ fallback? permanent;
      }
      return 200;
    }

    location /local_redirect{
      rewrite ^.*$ newname? permanent;
    }
  }
}
```

---

<!--
==================================================================================================================
-->


## Nginx 启动整体逻辑

#### master进程整体逻辑

- **main**: 由nginx.c启动

  - ngx_**daemon**: 守护进程化

    - fork: 父进程退出，子进程成为守护进程

  - ngx_**master_process_cycle**: master进程主逻辑

    - ngx_**start_worker_processes**: 启动worker进程

      - ngx_**spawn_process**:

        - fork: 产生worker进程

    - sig**suspend**: master挂机，等待信号

#### worker进程整体逻辑

- fork: 之后跟踪子进程，也就是worker进程

- ngx_**worker_process**_cycle: 在源码中的别名为proc

  - ngx_**worker_process_init**: worker进程初始化

    - 循环检查每个module是否有定义init_process函数，如果有则调用。

  - ngx_**setproctitle**: 将进程名由原先的master改成worker

  - ngx_process_**events_and_timers**: worker事件循环

    - ngx_**epoll**_process_events: worker进程的事件循环

---

<!--
==================================================================================================================
-->


## 开源 Nginx with RTMP module 与 Tengine-live 详细函调路径

**工具: cgdb**

- **main**: 启动nginx，并做初始化

  - ngx_**init_cycle**：
  **<font color='green'> [------------------------------------------------------ 整体初始化 -------------------------------------------------------------------]</font>**

    - ngx_**conf_parse**：
    **<font color='green'> [---------------------------------- 解析配置文件nginx\*.conf ----------------------------------------] </font>**

      - ngx_**conf_handler**
**<font color='green'> [--------------------- 发现rtmp配置，初始化其所有子模块 --------------------------] </font>**

        - ngx_**rtmp_block**

          - ngx_rtmp_{core|access|relay|...}\_create\_{main|srv|app}_conf

        - ngx_**rtmp_core_server**

          - ... \_create_..._conf .....

        - ngx_**rtmp_core_listen**

        - ngx_**rtmp_core_application**

          - ... \_create_..._conf .....

        - ngx_**rtmp_{core|codec|...}_merge**_{srv|app|...}_conf

        - ngx_**rtmp_relay_push_pull**：解析转发逻辑

        - ngx_**rtmp_..._postconfiguration**

          - <font color='green'> 挂载每个模块的next_play函数 </font>

        - ngx_**rtmp_add_{ports|addrs|...}**

  - ngx_**master_process**_cycle: master进程的主逻辑循环

    - ngx_**start_worker**_processes: 启动worker进程

      - ngx_**spawn_process**:
      **<font color=green> [-------------------------------------------------- fork出worker进程 ------------------------------------------------------] </font>**

        - ngx_**worker_process_cycle**: worker进程的主逻辑循环

          - ngx_**worker_process_init**：给worker初做始化

            - <font color='green'> 循环调用每个module的init_process函数（如果有） </font>

             - ngx_rtmp_**init_process**

             - ngx_rtmp_**{relay|exec|auto_push|stat}_init_process**

          - ngx_**process_events_and_timers**: worker进程的事件循环入口

            - ngx_**epoll**_process_events: worker进程的事件循环

              - **epoll_wait**(.., timer)
              **<font color=green> [------------------------------------ EPOLL事件循环 -----------------------------------] </font>**

              - ngx_**event_accept**：接受TCP连接

                - **<font color='green'> [------------------------------ RTMP 握手 ------------------------------]</font>**

                - ngx_rtmp_**init_connection**

                  - ngx_rtmp_**init_session**

                    - ngx_rtmp_**set_chunk_size**

                  - ngx_rtmp_**handshake**

                    - ngx_rtmp_**handshake_recv**

              - ngx_rtmp_**handshake_recv**：rtmp握手流程1, recv=1537

                - ngx_rtmp_**handshake_send**：回送握手，send=1537

              - ngx_rtmp_**handshake_recv**：rtmp握手流程2, recv=1536

                - ngx_rtmp_**handshake_done**：握手完成

                  - ngx_rtmp_**cycle**：**<font color=green> [------------ RTMP状态自动机 --------------] </font>**

                    - ngx_rtmp_**recv**：

> ### 以下对ngx_rtmp_recv详细展开

- ngx_rtmp_**recv**：

  - ngx_rtmp_**receive_message**：接收rtmp message
  **<font color='green'> [-------------------------------------------- 初始化 RTMP Connection ----------------------------------------------] </font>**

    - ngx_rtmp_**amf_message_handler**

      - ngx_rtmp_**amf_read**

      - ngx_rtmp_**cmd_connect_init**：初始化连接

        - **<font color='red'> Tengine-live做定制的域名解析 </font>**

          - <font color='red'> 解析rtmp_connect_t中的app字段，检查app是否带参数，形如[app]/name?arg=args </font>

          - <font color='red'> 解析四段式地址，URL: rtmp://ip/domain/app/stream </font>

        - ngx_rtmp_**notify_connect**：可能会有鉴权

          - ngx_rtmp_**cmd_connect**：建连

  - ngx_rtmp_**receive_message**：接收rtmp message
  **<font color='green'> [------------------------------------------------------- 开始推拉流 -------------------------------------------------------] </font>**

    - ngx_rtmp_**amf_message_handler**：

      - <font color='red'> ngx_rtmp_cmd_publish_init </font>

        - <font color='red'> ngx_rtmp_cmd_fcpublish </font>

      - ngx_rtmp_cmd_**create_stream_init**：首先创建流

        - <font color='red'> ngx_rtmp_ts_live_create_stream </font>

        - ngx_rtmp_**cmd_create_stream**

      - ngx_rtmp_**cmd_publish_init**：
      **<font color='green'> [--------------------- 若为publish推流，进入next_publish函数调用链 ---------------------] </font>**

        - <font color=red> ngx_rtmp_<b>buffer_switch_publish</b>: 上下行链路选择 </font>

        - ngx_rtmp_**dash_publish**: 判断dash推流

        - ngx_rtmp_**hls_publish**: 判断hls推流

        - ngx_rtmp_**log_publish**：根据推流名注册日志

        - ngx_rtmp_**notify_publish**：检查auto_push

          - <font color='red'> ngx_rtmp_<b>notify_send_remote_auth_publish</b> </font>

        - ngx_rtmp_**exec_publish**

        - <font color=red> ngx_rtmp_<b>ts_live_publish </b> </font>

        - <font color=red> ngx_rtmp_<b>flv_live_publish </b> </font>

        - <font color=red> ngx_rtmp_<b>combine_publish </b> </font>

        - ngx_rtmp_**relay_publish**：中继转发

          - ngx_rtmp_**relay_push**

            - ngx_rtmp_**relay_create**

              - **create_play_ctx**：

                - <font color=green> 本质为create_remote_ctx，与远端建连，远端play本地 </font>

                - ngx_rtmp_**relay_create_connection**

                    - ngx_rtmp_**init_session**

                    - ngx_rtmp_**client_handshake**

              - **create_publish_ctx**：

                - <font color=green> 本质为create_local_ctx，本地publish到远端 </font>

                - 在本地的relay_ctx中注册session，配置push_evt

        - ngx_rtmp_**live_publish**

          - ngx_rtmp_**live_join**: 注册为publisher

        - ngx_rtmp_**record_publish**

        - ngx_rtmp_**access_publish**

        - ngx_rtmp_**cmd_publish**

      - ngx_rtmp_**cmd_play_init**：
      **<font color='green'> [-------------------------- 若为play推流，进入next_play函数调用链 --------------------------] </font>**

        - ngx_rtmp_**log_play**

        - ngx_rtmp_**notify_play**

        - ngx_rtmp_**exec_play**

        - ngx_rtmp_**relay_play**

          - ngx_rtmp_**relay_pull**

            - ngx_rtmp_**relay_create**

              - **create_play_ctx**

                - <font color=green> 本质为create_local_ctx </font>

              - **create_publish_ctx**

                - <font color=green> 本质为create_remote_ctx，与远端建连 </font>

        - ngx_rtmp_**play_play**

        - ngx_rtmp_**live_play**

          - ngx_rtmp_**live_join**

            - ngx_rtmp_**live_get_stream**

              - <font color='green'> 计算流名的hash值 </font>

              - 根据hash在live_app_conf->streams中寻找对应的流

        - ngx_rtmp_**access_play**

        - ngx_rtmp_**cmd_play**

          - 调用链的终点，直接返回OK


  - ngx_rtmp_**receive_message**：接收rtmp message
  **<font color='green'> [------------------------------------ 处理控制信息 -------------------------------------] </font>**


      - ngx_rtmp_**protocol_message_handler**

        - ngx_rtmp_**set_chunk_size**



  - ngx_rtmp_**receive_message**：接收rtmp message
  **<font color='green'> [---------------- 处理用户信息 User Control Message Events --------------] </font>**

    - ngx_rtmp_**user_message_handler**

      - <font color='green'> ping心跳包处理 </font>

      - ngx_rtmp_**reset_ping**

        - ngx_**add_timer**

---

```sequence {theme='simple'}

Title: RTMP Communication Workflow

participant Client
participant Server


Note over Client,Server : Application Connect

Client -->> Server : Command Message (connect)

Server -->> Client : Window Acknowledgement Size

Server -->> Client : Set Peer Bandwidth

Client -->> Server : Window Acknowledgement Size

Server -->> Client : User Control Message (StreamBegin)

Server -->> Client : Command Message (result, connect response)

Note over Client,Server : Create Stream

Client -->> Server : Command Message (createStream)

Server -->> Client : Command Message (result, createStream response)

Note over Client,Server : Publish (after creating stream)

Client -->> Server : Command Message (Publish)

Server -->> Client : User Control (StreamBegin)

Client -->> Server : Data Message (Metadata)

Client -->> Server : Audio Data

Client -->> Server : SetChunkSize

Server -->> Client : Command Message (publish result)

Client -->> Server : Video Data

Client -->> Server : Keep sending messages until complete....

Note over Client,Server : Play (after creating stream)

Client -->> Server : Command Message (Play)

Server -->> Client : SetChunkSize

Server -->> Client : User Control (StreamIsRecorded)

Server -->> Client : User Control (StreamBegin)

Server -->> Client : Command Message (onStatus-play reset)

Server -->> Client : Command Message (onStatus-play start)

Server -->> Client : Audio Message

Server -->> Client : Video Message

Server -->> Client : Keep sending messages....

```

---

## Nginx HTTP module 整体逻辑

#### worker进程下

- ngx_**process_events_and_timers**
  - ngx_**epoll**_process_events
    - ngx_event_**accept**：接受TCP连接
      - ngx_**get_connection**：生成ngx_connection_t结构
      - ngx_http_**init_connection**：初始化connection结构
        - ngx_add_**timer**: 连接超时定时器,60s
    - ngx_**http_wait_request**_handler：等待请求
      - ngx_http_**create_request**：创建request结构
        - ngx_http_**alloc_request**：分配request结构
      - ngx_http_**process_request**_line：逐行处理request
        - ngx_http_**read_request_header**：读取request header
        - ngx_http_**parse_request**_line：逐行解析request
        - ngx_http_**process_request_uri**：处理request uri
        - ngx_http_**process_request_headers**：处理request headers
          - ngx_http_**parse_header**_line：逐行解析header字段
          - ngx_http_**process_request**：处理request
            - ngx_http_**handler**：http处理函数
              - ngx_http_**core_run_phases**: 核心逻辑，在此过程中将http响应报文发出
                - ngx_http_**core_rewrite_phase**
                  - ngx_http_**rewrite_handler**
                - ngx_http_**core_find_config_phase**
                - ngx_http_**core_post_rewrite_phase**
                - ngx_http_**core_generic_phase**
                - ngx_http_**core_access_phase**
                - ngx_http_**core_post_access_phase**
                - ngx_http_**core_content_phase**

---
