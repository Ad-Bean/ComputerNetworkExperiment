//client which receive messages from server

#include <winsock2.h>
// #include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#define WSVERS MAKEWORD(2, 0)
void errexit(const char*, ...);  //错误信息打印函数

int main(int argc, char* argv[]) {
    struct sockaddr_in sock_address, from_address;
    SOCKET clientsock;
    WSADATA wsadata;

    if (WSAStartup(WSVERS, &wsadata) != 0) {
        errexit("WSAStartup failed\n");
    }

    //create and bind serversock
    clientsock = socket(AF_INET, SOCK_DGRAM, 0);            //获得serversock
    sock_address.sin_family = AF_INET;                      //设置地址家族
    sock_address.sin_port = htons(45632);                   //设置端口
    sock_address.sin_addr.s_addr = inet_addr("127.0.0.1");  //设置地址
    int flag = bind(clientsock, (struct sockaddr*)&sock_address, sizeof(struct sockaddr));
    if (flag != 0) errexit("bind falied.\n");

    char buf[4096];

    int count = 0;
    int fromlen;
    const char* endquotes = "end of messages";
    // while (true) {
    while (1) {
        flag = recvfrom(clientsock, buf, 4096, 0, NULL, NULL);
        if (flag == -1) {
            errexit("receive falied, errno = %d\n", GetLastError());
        }
        printf("%s\n", buf);
        if (strcmp(endquotes, buf) == 0) break;
        ++count;
        printf("count: %d\n", count);
    }
    printf("\n\ntotal 100, actually receive %d, lost %d\n", count, 100 - count);
    (void)closesocket(clientsock);

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
