//
//  MulticastClient.c
//  SocketProgramming
//
//  Created by 오승기 on 2022/10/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int receive_socket;
    int str_len;
    char buf[BUF_SIZE];
    
    struct sockaddr_in address;
    struct ip_mreq join_address;
    
    if(argc != 3) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    receive_socket = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[2]));
    
    if(bind(receive_socket, (struct sockaddr*) &address, sizeof(address)) == -1) {
        error_handling("bind() error");
    }
    
    join_address.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_address.imr_interface.s_addr = htonl(INADDR_ANY);
    
    setsockopt(receive_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_address, sizeof(join_address));
    
    while (1) {
        str_len = (int)recvfrom(receive_socket, buf, BUF_SIZE-1, 0, NULL, 0);
        if(str_len<0) { break; }
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    
    close(receive_socket);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
