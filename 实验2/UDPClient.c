/*
    **compile**
    gcc UDPClient.c -lwsock32 -o UDPClient 
    **execute**
    ./UDPClient
*/
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock Library
#define true 1
#define SERVER "127.0.0.1"  // ip address of udp server
#define BUFFER 512          // Max length of buffer
#define PORT 8000           // The port on which to listen for incoming data

int main(void) {
    int winSocket;
    WSADATA winSocketApi;
    struct sockaddr_in server;

    int serverLen = sizeof(server);
    char buffer[BUFFER], message[BUFFER];

    // Initialize winsock
    printf("Initializing windows socket...\n");
    if (WSAStartup(MAKEWORD(2, 2), &winSocketApi) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    // printf("Initialised.\n");

    // Create socket
    if ((winSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("socket() failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Setup address structure
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);

    // start communication
    int count = 1000;
    printf("Enter message: ");
    // gets(message);
    scanf("%s", message);
    printf("Sending message %d times...\n", count);
    while (count--) {
        // sending the message
        if (sendto(winSocket, message, strlen(message), 0, (struct sockaddr *)&server, serverLen) == SOCKET_ERROR) {
            printf("sendto() failed with error code : %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        // receive a reply and print it
        memset(buffer, '\0', BUFFER);  // clear the buffer
        // try to receive data, which is a blocking call
        if (recvfrom(winSocket, buffer, BUFFER, 0, (struct sockaddr *)&server, &serverLen) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buffer);
    }
    closesocket(winSocket);
    WSACleanup();
    return 0;
}