#include <cstdlib>
#include <cstdio>
#include <winsock2.h>
#include <time.h>

#define WSVERS MAKEWORD(2, 2)

const int LEN = 10240;

char buf[LEN];

int flags, from, fromlen;

int main(int argc, char * argv[]){
    WSADATA wsadata;
    if (WSAStartup(WSVERS, &wsadata) != 0){
        printf("ERROR in Startup");
        return -1;
    }
    struct sockaddr_in* address;
    address = (struct sockaddr_in * )malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(10949);
    (address->sin_addr).s_addr = INADDR_ANY;
    SOCKET qwq = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int statue = bind(qwq, (struct sockaddr*)address, sizeof(struct sockaddr));
    if (statue){
        printf("ERROR in bind\n");
        int rt = WSAGetLastError();
        printf("%d %d\n", statue, rt);
        return -1;
    }
    printf("Server is running.....\n");
    int cnt = 0;
    struct sockaddr from;
    fromlen = sizeof(from);
    while(1){
        statue = recvfrom(qwq, buf, LEN, 0, &from, &fromlen);
        // printf("%d\n", statue);
        if (statue >= 0){
            ++ cnt;
            printf("Recive %d packages\n", cnt);
            printf("the content is :%s\n", buf);
        }
    }
	closesocket(qwq);
	WSACleanup();
    return 0;
}
