/*
    **compile**
    gcc UDPServer.c -lwsock32 -o UDPServer 
    **execute**
    ./UDPServer
*/
#include <stdio.h>
#include <winsock2.h>

#define true 1
#define PORT 8000                   // The port on which to listen for incoming data
#define SIZE 1024                   // Max length of buffer
#pragma comment(lib, "ws2_32.lib")  // Winsock Library

int main() {
    SOCKET udpSocket;                   // windows socket
    WSADATA winSocketApi;               // windows socket api
    struct sockaddr_in server, client;  // socket address
    char buffer[SIZE];
    int clientLen = sizeof(client);

    // Prepare the sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Initializing winsock
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &winSocketApi) != 0) {
        printf("Initializeing failed, with error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Creating socket
    printf("Creating Socket...\n");
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Bind
    printf("Binding...\n");
    if (bind(udpSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    int total = 0;
    printf("Listening...\n");
    printf("Enter buffer size: ");
    int bufferLen = 1024;
    scanf("%d", &bufferLen);
    while (true) {
        // printf("Waiting for packet...\n");
        // receive packets, which is a blocking call
        // the third parameter of the sendto function is the len of the packet
        int receiveLen = recvfrom(udpSocket, buffer, bufferLen, 0, (struct sockaddr *)&client, &clientLen);
        // fflush(stdout);
        // memset(buffer, '\0', bufferLen);
        if (receiveLen == SOCKET_ERROR) {
            printf("recvfrom() failed with error code: %d\n", WSAGetLastError());
            continue;
        }

        // if (strcmp("quit", buffer) == 0) {
        //     printf("total: %d\nsuccess: %d\nfailed: %d\n", total, success, failed);
        //     break;
        // }

        printf("Receiving packets from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Packet[%d] received:\n%s\n", ++total, buffer);

        // Reply the client with the same data
        // if (sendto(udpSocket, buffer, receiveLen, 0, (struct sockaddr *)&client, clientLen) == SOCKET_ERROR) {
        //     printf("sendto() failed with error code: %d\n", WSAGetLastError());
        //     // exit(EXIT_FAILURE);
        //     // failed++, total++;
        // }
    }
    closesocket(udpSocket);
    WSACleanup();
    return 0;
}