//server which send messages to client

#include <winsock2.h>
// #include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#define WSVERS MAKEWORD(2, 0)
void errexit(const char*, ...);  //错误信息打印函数

int main(int argc, char* argv[]) {
    struct sockaddr_in sock_address, to_address;
    SOCKET serversock;
    WSADATA wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0) {
        errexit("WSAStartup failed\n");
    }
    //create and bind serversock
    serversock = socket(AF_INET, SOCK_DGRAM, 0);            //获得serversock
    sock_address.sin_family = AF_INET;                      //设置地址家族
    sock_address.sin_port = htons(45632);                   //设置端口
    sock_address.sin_addr.s_addr = inet_addr("127.0.0.1");  //设置地址
    int flag = bind(serversock, (struct sockaddr*)&sock_address, sizeof(struct sockaddr));
    if (flag != 0) errexit("bind falied.\n");

    char buf[4096];

    //initiate to_address with client's address
    to_address.sin_family = AF_INET;
    to_address.sin_port = htons(11451);  //设置端口
    const char* addr = "127.0.0.1";
    to_address.sin_addr.s_addr = inet_addr(addr);  //设置地址

    int count = 0;
    while (++count <= 101) {
        char buf[4096];
        if (count <= 100)
            sprintf(buf, "Hello, I'm number %d of packet sent from %s", count, "127.0.0.1");
        else
            sprintf(buf, "end of messages");
        flag = sendto(serversock, buf, 4096, 0, (struct sockaddr*)&to_address, sizeof(struct sockaddr));
        if (flag == -1) {
            errexit("send falied, errno = %d\n", GetLastError());
        }
        printf("send message '%s' to %s\n", buf, addr);
    }

    (void)closesocket(serversock);

    return 0;
}

void errexit(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    WSACleanup();
    exit(1);
}
