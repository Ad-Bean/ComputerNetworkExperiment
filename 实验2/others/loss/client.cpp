#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP_ADDR "192.168.43.146" //服务器IP地址
#define SERVER_PORT 2450                //服务器端口号
#define BUF_SIZE 1024
#define Num_Of_Message 1000000

int main(int argc, char *argv[])
{
    int rval, Length = 0;
    char sendbuf[BUF_SIZE];
    char revbuf[BUF_SIZE];

    WORD wVersionrequested;
    WSADATA wsaData;
    SOCKET ClientSock;
    struct sockaddr_in ServerAddr;

    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");

    /* 加载Winsock */
    wVersionrequested = MAKEWORD(2, 2);               //  使用2.0版的winsock
    if (WSAStartup(wVersionrequested, &wsaData) != 0) //完成Winsock服务初始化。
    {
        printf("Failed to load Winsock!\n");
        system("pause");
        return -1;
    }
    printf("Succeed to load Winsock!\n");

    /* 创建套接字 */
    ClientSock = socket(AF_INET, SOCK_DGRAM, 17); //  调用socket函数建立客户端socket，参数指明为UDP通信。
    if (ClientSock == INVALID_SOCKET)
    {
        printf("Failed to create socket!\n");
        system("pause");
        exit(1);
    }
    printf("Succeed to create socket!\n");

    /* 配置服务器IP、端口信息 */
    memset(&ServerAddr, 0, sizeof(struct sockaddr)); //每一个字节都用0来填充 //struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(SERVER_PORT);
    ServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR); //利用一个sockaddr_in结构体指定服务端的地址族、端口号与IP地址

    printf("\nNow you can have a task!\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
    printf("You can send data to server now: \n\n");

    // 配置socket及连接服务端部分完成

    // 开始发包

    int cnt = 0;

    while (cnt < Num_Of_Message)
    {
        Length = sizeof(struct sockaddr);
        /* 输入要发送的数据 */
        memset(sendbuf, 0, BUF_SIZE);
        // printf("--> ");
        // gets_s(sendbuf); //使用gets_s()能够获取空格
        strcpy(sendbuf, "Hello world!");

        /* 发送数据到服务器 */
        rval = sendto(ClientSock, sendbuf, strlen(sendbuf), 0, (SOCKADDR *)&ServerAddr, Length);
        if (rval <= 0)
        {
            printf("Write error or failed to send the message!\n");
            system("pause");
            exit(1);
        }
        else
        {
            // cout << "Successfully sending------" << rval << endl;
        }
        cnt++;
    }

    closesocket(ClientSock); //关闭套接字
    WSACleanup;              //停止Winsock

    system("pause");
    return 0;
}
