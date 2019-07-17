//server_functions header
#ifndef _SERVER_FUNCTIONS_H_
#define _SERVER_FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void send_confirmation(int *client_sockfd);
void get_address(int client_sockfd, char *buffer);

#endif//#ifndef _SERVER_FUNCTIONS_H_