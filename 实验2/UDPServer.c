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
#define BUFFER 512                  // Max length of buffer
#pragma comment(lib, "ws2_32.lib")  // Winsock Library

int main() {
    SOCKET udpSocket;                   // windows socket
    WSADATA winSocketApi;               // windows socket api
    struct sockaddr_in server, client;  // socket address
    char buffer[BUFFER];

    int receiveLen;
    int sendLen = sizeof(client);

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Initialise winsock
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &winSocketApi) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
    // Create a socket
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");
    // Bind
    if (bind(udpSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done\n");
    int total = 0, success = 0, failed = 0;
    // Keep listening for data
    while (true) {
        printf("Waiting for data...\n");
        fflush(stdout);
        // Clear the buffer by filling null, it might have previously received data
        memset(buffer, '\0', BUFFER);
        // Try to receive some data, this is a blocking call
        if ((receiveLen = recvfrom(udpSocket, buffer, BUFFER, 0, (struct sockaddr *)&client, &sendLen)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        // Print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data[%d]: %s\n", ++success, buffer);
        total++;
        // Reply the client with the same data
        if (sendto(udpSocket, buffer, receiveLen, 0, (struct sockaddr *)&client, sendLen) == SOCKET_ERROR) {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            // exit(EXIT_FAILURE);
            failed++, total++;
        }
    }
    closesocket(udpSocket);
    WSACleanup();
    return 0;
}