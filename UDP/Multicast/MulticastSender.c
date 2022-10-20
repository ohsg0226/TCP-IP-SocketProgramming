//
//  MulticastSender.c
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
    int sender_socket;
    struct sockaddr_in multi_address;
    int time_live = TTL;
    FILE *fp;
    char buf[BUF_SIZE];
    
    if(argc != 3) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    sender_socket = socket(PF_INET, SOCK_DGRAM, 0);     //UDP연결
    memset(&multi_address, 0, sizeof(multi_address));
    multi_address.sin_family = AF_INET;
    multi_address.sin_addr.s_addr = inet_addr(argv[1]);
    multi_address.sin_port = htons(atoi(argv[2]));
    
    setsockopt(sender_socket, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live));
    
    if((fp = fopen("news.txt", "r")) == NULL) {
        error_handling("fopen() error");
    }
    
    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(sender_socket, buf, strlen(buf), 0, (struct sockaddr*) &multi_address, sizeof(multi_address));
        
        sleep(2);
    }
    close(fp);
    close(sender_socket);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
