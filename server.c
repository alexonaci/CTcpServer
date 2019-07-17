#include "server_functions.h"

int main()
{
    int i, j, bytes_in, server_sockfd, client_sockfd, connection_status;
    char n_buffer[20];
    char buffer[512];
    char buffer2[512];

    //creating the server socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //TODO error check

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = inet_addr("0.0.0.0");

    //bind the socket to our specified IP address and port
    bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(server_sockfd, 5);

    printf("Listening...\n");

    //create fd set
    fd_set main_fd_set, copy_fd_set;

    //clear the set
    FD_ZERO(&main_fd_set);
    FD_ZERO(&copy_fd_set);

    //add server socket to set
    FD_SET(server_sockfd, &main_fd_set);

    int fdmax = server_sockfd;

    while(1)
    {
        copy_fd_set = main_fd_set;


        if(select(fdmax + 1, &copy_fd_set, NULL, NULL, NULL) == -1)
        {
            perror("select");
        }

        //cycle through file descriptors
        for(i = 0; i <= fdmax; i++)
        {
            if(FD_ISSET(i, &copy_fd_set))
            {
                if(i == server_sockfd)
                {
                    //accept a new connection
                    client_sockfd = accept(server_sockfd, NULL, NULL);

                    if(client_sockfd > fdmax)
                    {
                        fdmax = client_sockfd;
                    }

                    //add new client to list of connected clients
                    FD_SET(client_sockfd, &main_fd_set);
                    
                    //send a welcome message to the connected client
                    send_confirmation(&client_sockfd);

                    //TODO broadcast that a new client connected
                }
                else
                {
                    printf("DEBUG new message incoming from [%d]", i);

                    //accept a new message
                    bytes_in = recv(i, buffer, 512, 0);

                    if(bytes_in <= 0)
                    {
                        if(bytes_in == 0)
                        {
                            printf("Client connection dropped\n");
                            //TODO broadcast to all clients
                        }
                        else
                        {
                            perror("recv");
                        }

                        close(i);
                        FD_CLR(i, &main_fd_set);
                    }
                    else
                    {
                        buffer[bytes_in] = '\0';
                        printf("Client message: %s\n", buffer);

                        //broadcast message for everyone except sender
                        for(j = 0; j <= fdmax; j++)
                        {
                            if(FD_ISSET(j, &main_fd_set))
                            {
                                if(j != server_sockfd && j != i)
                                {
                                    //get address(j, ip_buffer);
                                    //sprintf(buffer2, "[%s] %s", ip_buffer, buffer);

                                    if(send(j, buffer, bytes_in, 0) == -1)
                                    {
                                        perror("send");
                                    }
                                }
                            }
                        }//for loop, broadcast message to users except sender
                    }
                }
            }            
        }//for loop, cycle through file descriptors

        usleep(1000);

    }//while(1)

    close(server_sockfd);
}