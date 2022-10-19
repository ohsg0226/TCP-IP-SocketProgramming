//
//  UDPEcho_client.c
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
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t address_size;
    
    struct sockaddr_in server_address, from_address;
    
    if(argc != 3) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    //UDP 소켓 생성
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    if(sock == -1) {
        error_handling("socket() error");
    }
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));
    
    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        
        sendto(sock,
               message,
               strlen(message),
               0,
               (struct sockaddr*)&server_address,
               sizeof(server_address));
        address_size = sizeof(from_address);
        str_len = (int)recvfrom(sock,
                                message,
                                BUF_SIZE,
                                0,
                                (struct sockaddr*)&from_address,
                                &address_size);
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
