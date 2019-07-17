#include "server_functions.h"

void send_confirmation(int *client_sockfd)
{
    char buffer[27] = "Server message of the day\n";
    char ip_buffer[20];

    send(*client_sockfd, buffer, sizeof(buffer), 0);

    get_address(*client_sockfd, ip_buffer);

    printf("Client: [%s] connected\n", ip_buffer);
}

void get_address(int client_sockfd, char *buffer)
{
    struct sockaddr_in addr;
    
    socklen_t addr_size = sizeof(struct sockaddr_in);

    getpeername(client_sockfd, (struct sockaddr *)&addr, &addr_size);

    strcpy(buffer, inet_ntoa(addr.sin_addr));
}