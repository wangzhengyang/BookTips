[toc]

# `socket`函数
执行网络I/O，一个进程必须要做的第一件事情就是调用`socket`函数，指定期望的通信协议类型(使用IPv4的TCP、使用IPv6的UDP、Unix域字节流协议等)
```c
#include <sys/socket.h>

int socket(int family, int type, int protocol); //返回：若成功则为非负描述符，若出错则为-1
```

|family|说明|
|-|-|
|AF_INET|IPv4协议|
|AF_INET6|IPv6协议|
|AF_LOCAL|Unix域协议|
|AF_ROUTE|路由套接字|
|AF_KEY|密钥套接字|

|type|说明|
|-|-|
|SOCK_STREAM|字节流套接字|
|SOCK_DGRAM|数据报套接字|
|SOCK_SEQPACKET|有序分组套接字|
|SOCK_RAW|原始套接字|

|protocol|说明|
|IPPROTO_CP|TCP传输协议|
|IPPROTO_UDP|UDP传输协议|
|IPPROTO_SCTP|SCTP传输协议|

`socket`函数在成功时返回一个小的非负整数值，它与文件描述符类型，把它称为套接字描述符，简称`sockfd`

# `connect`函数
TCP客户用`connect`函数来建立与TCP服务器的连接

```c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
```
客户在调用函数`connect`前不必非得调用`bind`函数，因为如果需要的话，内核会确定源IP地址，并选择一个临时端口作为源端口

如果是TCP套接字，调用`connect`函数会激发TCP的三路握手过程，而且仅在连接建立成功或出错才返回，其中出错返回可能有以下几种情况：
1. 若TCP客户没有收到SYN分节的响应，则返回ETIMEDOUT错误
2. 若对客户的SYN的响应是RST(表示复位)，则表明该服务器主机在指定的端口上没有进程在等待与之连接，客户一接收到RST就马上返回ECONNREFUSED错误
3. 若客户发出的SYN在中间的某个路由器上引发了ICMP错误，若在某个规定的时间后仍未收到响应，则把保存的消息(即ICMP错误)作为EHOSTUNREACH或ENETUNREACH错误返回给进程

# `bind`函数
把一个本地协议地址赋予一个套接字，对于网际网协议，协议地址是32位的IPv4地址或128位的IPv6地址与16位的TCP或UDP端口号的组合

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen); //返回：若成功则为0，若出错则为-1
```

# `listen`函数
仅由TCP服务器调用，它做两件事
1. 当`socket`函数创建一个套接字时，它被假设为一个主动套接字，也就是说，它是一个将调用`connect`发起连接的客户套接字，`listen`函数把一个未连接的套接字转换为一个被动套接字，指示内核应接受指向该套接字的连接请求，调用`listen`导致套接字从CLOSED状态转换到LISTEN状态
2. 第二个参数规定了内核应该为相应套接字排队的最大连接个数

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog); //返回：若成功则为0，若出错则为-1
```

内核为任何一个给定的监听套接字维护两个队列：
1. 未完成连接队列   每个这样SYN分节对应其中一项：已由某个客户发出并到达服务器，而服务器正在等待完成相应的TCP三路握手过程，这些套接字处于SYN_RCVD状态
2. 已完成连接队列   每个已完成TCP三路握手过程的客户对应其中一项，这些套接字处于ESTABLISHED状态

```c
void Listen(int fd, int backlog)
{
    char *ptr;
    if((ptr = getnev("LISTENQ)) != NULL){
        backlog = atoi(ptr);
    }

    if(listen(fd, backlog) < 0){
        err_sys("listen error");
    }
}

```

# `accept`函数
由TCP服务器调用，用于从已完成连接队列队头返回下一个已完成连接，如果已完成连接队列为空，那么进程被投入睡眠(假定套接字为默认的阻塞方式)
```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen); //返回：若成功则为非负描述符，若出错则为-1
```

# `fork`和`exec`函数
```c
#include <unistd.h>
pid_t fork(void); //返回：在子进程中为0，在父进程中为子进程ID，若出错则为-1
```

```c
#include <unistd.h>
int execl(const char *pathname, const char *arg0, ... /*(char*)0*/);
int execv(const char *pathname, char *const *argv[]);
int execle(const char *pathname, const char *arg0, ... /*(char*)0, char *const envp[]*/);
int execve(const char *pathname, char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg0, ... /*(char*)0*/);
int execvp(const char *filename, char *const argv[]);
```

# 并发服务器
```c
pid_t pid
int listenfd, connfd;
listenfd = Socket(...);
Bind(listenfd, ...);
Listen(listenfd, LISTENQ);
for(;;)
{
    connfd = Accept(listenfd, ...);
    if((pid = Fork()) == 0){
        Close(listenfd);
        doit(connfd);
        Close(connfd);
        exit(0);
    }
    Close(connfd);
}
```

# `close`函数
用来关闭套接字，并终止TCP连接
```c
#include <unistd.h>
int close(int sockfd); //返回：若成功则为0，若出错则为-1
```

# `getsockname`和`getpeername`函数
这两个函数或者返回与某个套接字关联的本地协议地址(`getsockname`)，或者返回与某个套接字关联的外地协议地址(`getpeername`)
```c
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);
```
