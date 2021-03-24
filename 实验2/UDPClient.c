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
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    char buffer[BUFFER];
    char message[BUFFER];

    //Initialise winsock
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &winSocketApi) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    // create socket
    if ((winSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("socket() failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //setup address structure
    memset((char *)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

    //start communication
    int count = 100;
    printf("Enter message: ");
    gets(message);
    printf("Sending message %d times...\n", count);
    while (count--) {
        // send the message
        if (sendto(winSocket, message, strlen(message), 0, (struct sockaddr *)&si_other, slen) == SOCKET_ERROR) {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        // receive a reply and print it
        // clear the buffer by filling null, it might have previously received data
        memset(buffer, '\0', BUFFER);
        //try to receive some data, this is a blocking call
        if (recvfrom(winSocket, buffer, BUFFER, 0, (struct sockaddr *)&si_other, &slen) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        puts(buffer);
    }
    closesocket(winSocket);
    WSACleanup();
    return 0;
}