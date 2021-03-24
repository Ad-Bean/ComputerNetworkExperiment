#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
//
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA data;
    int state = WSAStartup(MAKEWORD(2, 2), &data);
    if (state != 0) {
        printf("initial error\n");
        return 0;
    }
    SOCKET word = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (word == INVALID_SOCKET) {
        printf("socket error\n");
        return 0;
    }
    struct sockaddr_in my_socket;

    my_socket.sin_family = AF_INET;
    my_socket.sin_port = htons(10000);
    //my_socket.sin_addr.s_addr = inet_addr("49.232.4.77"); // 加IP地址
    // my_socket.sin_addr.s_addr = inet_addr("172.26.82.33");
    my_socket.sin_addr.s_addr = inet_addr("127.0.0.1");

    int size;
    printf("please enter the number of your package\n");
    scanf("%d", &size);
    for (int i = 0; i < size; ++i) {
        char buf[32], num[10];
        strcpy(buf, "this is package");
        itoa(i, num, 10);
        strcat(buf, num);
        int cc = sendto(word, buf, 32, 0, (SOCKADDR *)&my_socket, sizeof(my_socket));
        if (cc == SOCKET_ERROR) {
            printf("send error\n");
            return 0;
        }
        Sleep(1);
    }
    char ending[32];
    char num[50];
    strcpy(ending, "aaaaaaa");
    itoa(size, num, 10);
    strcat(ending, num);
    sendto(word, ending, 32, 0, (SOCKADDR *)&my_socket, sizeof(my_socket));
    closesocket(word);
    WSACleanup();
}