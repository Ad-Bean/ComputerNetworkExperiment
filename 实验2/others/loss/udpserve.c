#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
//
#include <windows.h>
#pragma comment(lib, "ws2_32.lib");

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
    struct sockaddr_in from; /* sender address               */
    int fromsize = sizeof(from);
    //bzero(&my_socket,sizeof(my_socket));
    my_socket.sin_family = AF_INET;
    my_socket.sin_port = htons(10000);
    my_socket.sin_addr.s_addr = INADDR_ANY;
    if (bind(word, (LPSOCKADDR)&my_socket, sizeof(my_socket)) == SOCKET_ERROR) {
        printf("bind error\n");
        return 0;
    }
    char rev[32];
    int size = 0;
    char res[50];
    int result;
    while (1) {
        int cc = recvfrom(word, rev, 32, 0, (SOCKADDR *)&from, &fromsize);
        if (cc == SOCKET_ERROR) {
            printf("recvfrom() failed; %d\n", WSAGetLastError());
            break;
        } else if (cc == 0)
            break;
        else {
            //if(strlen(rev)>7)
            if (1) {
                char ans[50] = "aaaaaaa";
                char temp[32];
                strcpy(temp, rev);
                temp[7] = '\0';
                printf("%s\n%s\n", temp, ans);
                if (strcmp(ans, temp) == 0) {
                    int i = 7;
                    while (rev[i] != '\0') {
                        res[i - 7] = rev[i];
                        i++;
                    }
                    res[i] = '\0';
                    result = atoi(res);
                    printf("Pakage sended:%d\tPakage received:%d\n", result, size);
                    printf("The loss rate is %f\n", 1.0 - 1.0 * size / result);
                }
            }
            size++;
            rev[cc] = '\0';
            printf("%s\n", rev);
        }
        printf("%d", size);
    }
    closesocket(word);
    WSACleanup();
}