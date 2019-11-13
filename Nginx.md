## Nginx 小知识

- 查询nginx加载的模块: objs/ngx_module.c

- 守护进程化的实现: src/os/unix/ngx_daemon.c

- 内存池的实现: src/core/ngx_palloc.c

- cycle->conf_ctx的内容: 存储每个模块的信息，为 ngx_max_module 个 void* 指针

- NGX_CODE_MODULE类型的module: (1)ngx_core_module (2)ngx_errlog_module (3)ngx_events_module (4)ngx_http_module

- worker进程的事件循环: ngx_process_events_and_timers

## Nginx with RTMP module 空转时函数调用路径

```cpp
(gdb) bt
#0  ngx_rtmp_exec_child_dead (ev=0x556d30976f90) at ./nginx-rtmp-module/ngx_rtmp_exec_module.c:620
#1  0x0000556d2f64f890 in ngx_epoll_process_events (cycle=0x556d30940d60, timer=<optimized out>, flags=<o
ptimized out>) at src/event/modules/ngx_epoll_module.c:902
#2  0x0000556d2f64406f in ngx_process_events_and_timers (cycle=cycle@entry=0x556d30940d60) at src/event/n
gx_event.c:242
#3  0x0000556d2f64d3ef in ngx_worker_process_cycle (cycle=cycle@entry=0x556d30940d60, data=data@entry=0x0
) at src/os/unix/ngx_process_cycle.c:750
#4  0x0000556d2f64b66c in ngx_spawn_process (cycle=cycle@entry=0x556d30940d60, proc=proc@entry=0x556d2f64
d2ea <ngx_worker_process_cycle>, data=data@entry=0x0, name=name@entry=0x556d2f6fe31b "worker process", re
spawn=respawn@entry=-3) at src/os/unix/ngx_process.c:199
#5  0x0000556d2f64c9ce in ngx_start_worker_processes (cycle=cycle@entry=0x556d30940d60, n=1, type=type@en
try=-3) at src/os/unix/ngx_process_cycle.c:359
#6  0x0000556d2f64dd2c in ngx_master_process_cycle (cycle=0x556d30940d60) at src/os/unix/ngx_process_cycl
e.c:131
#7  0x0000556d2f623033 in main (argc=<optimized out>, argv=<optimized out>) at src/core/nginx.c:382

```

首先，让nginx加载nginx-rtmp-module，并让其空转。从调用栈，我们可以知道其函数的调用顺序:

- main: 启动nginx，并做一些初始化

  - ngx_master_process_cycle: master进程的主逻辑

    - ngx_start_worker_processes: 启动worker进程

      - ngx_spawn_process: master进程fork出worker进程的入口
      
        - ngx_worker_process_cycle: worker进程的主逻辑

ngx_process_events_and_timers: worker进程的事件循环入口

ngx_epoll_process_events: worker进程的事件循环

ngx_rtmp_exec_child_dead:
