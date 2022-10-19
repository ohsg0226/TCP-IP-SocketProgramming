//
//  UDPEcho_server.c
//  SocketProgramming
//
//  Created by 오승기 on 2022/10/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, const char * argv[]) {
    int server_socket;
    char message[BUF_SIZE];
    int str_len;
    socklen_t client_address_size;
    
    struct sockaddr_in server_address, client_address;
    
    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    //UDP 소켓 생성
    server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    
    if(server_socket == -1) {
        error_handling("UDP Socket creatino error");
    }
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    
    while (1) {
        client_address_size = sizeof(client_address);
        str_len = (int)recvfrom(server_socket,
                                message,
                                BUF_SIZE,
                                0,
                                (struct sockaddr*)&client_address,
                                &client_address_size);
        
        sendto(server_socket, message, str_len, 0, (struct sockaddr*)&client_address, client_address_size);
    }
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
