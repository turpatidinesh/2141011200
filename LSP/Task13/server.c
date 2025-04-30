#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Server listening on port %d...\n", PORT);

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    printf("Server received: %s\n", buffer);

    strcpy(buffer, "Hello from Server");
    sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&clientaddr, len);

    close(sockfd);
    return 0;
}
