[toc]

# TCP回射服务器程序：main函数

```c
#include "unp.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t chilen;
    struct sockaddr_in chiaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    for(;;){
        chilen = sizeof(chliaddr);
        connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
        if((childpid = Fork()) == 0){
            Close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
}
```
# TCP回射服务器程序：str_echo函数
```c
#include "unp.h"

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while((n = read(sockfd, buf, MAXLINE)) > 0){
        Writen(sockfd, buf, n);
    }

    if(n < 0 && errno == EINTR){
        goto again;
    }else if(n < 0){
        err_sys("str_echo: read error");
    }
}
```

# TCP回射客户程序：main函数
```c
#include "unp.h"

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    if(argc != 2){
        err_quit("usage: tcpcli <IPaddress>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    Connect(socket, (SA*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    exit(0);
```

# TCP回射客户程序：str_cli函数
```c
#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while(Fgets(sendline, MAXLINE, fp) != NULL){
        Writen(socketfd, sendline, strlen(sendline));
        if(Readline(sockfd, recvline, MAXLINE) == 0){
            err_quit("str_cli: server terminated prematurely);
        }
        Fputs(recvline, stdout);
    }
}
```