//
//  calculate_client.c
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

int main(int argc, char *argv[]) {
    int client_socket;
    int result, operand_count;
    char operandArray[BUF_SIZE];
    struct sockaddr_in server_address;
    
    if(argc!=3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1) {
        error_handling("socket() error");
    }
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));
    
    if(connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    } else {
        puts("Connected......");
    }
    
    fputs("Operand count: ", stdout);
    scanf("%d", &operand_count);
    operandArray[0] = operand_count;
    
    for(int i=0; i<operand_count; i++) {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&operandArray[i*4+1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &operandArray[operand_count*4+1]);
    
    write(client_socket, operandArray, operand_count*4+2);
    read(client_socket, &result, 4);
    
    printf("Operation result: %d", result);
    
    close(client_socket);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

