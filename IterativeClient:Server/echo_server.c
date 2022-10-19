//
//  echo_server.c
//  SocketProgramming
//
//  Created by 오승기 on 2022/10/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, const char * argv[]) {
    int server_socket, client_socket;
    char message[BUF_SIZE];
    int str_len;
    
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size;
    
    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    if(server_socket == -1) {
        error_handling("socket() error");
    }
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    
    if(listen(server_socket, 5)==-1) {
        error_handling("listen() error");
    }
    
    client_address_size = sizeof(client_address);
    
    for(int i=0; i<5; i++) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if(client_socket == -1) {
            error_handling("accept() error");
        } else {
            printf("Connected client %d\n", i+1);
        }
        
        while ((str_len=(int)read(client_socket, message, BUF_SIZE))!=0) {
            write(client_socket, message, str_len);
            
            close(client_socket);
        }
    }
    close(server_socket);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

