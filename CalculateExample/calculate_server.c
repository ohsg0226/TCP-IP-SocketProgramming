//
//  calculate_server.c
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
int calculate(int opnum, int opnds[], char op);

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    int receiveCount, result;
    char operandInfo[BUF_SIZE];
    struct sockaddr_in server_adress, client_address;
    socklen_t client_address_size;
    
    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        error_handling("socket() error");
    }
    
    memset(&server_adress, 0, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adress.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*) &server_adress, sizeof(server_adress)) == -1) {
        error_handling("bind() error");
    }
    
    if(listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }
    
    client_address_size = sizeof(client_address);

    for(int i=0; i<5; i++) {
        int operandCount = 0;
        client_socket = (int)accept(server_socket, (struct sockaddr*) &client_address, &client_address_size);
        read(client_socket, &operandCount, 1);
        
        int recieveLength = 0;
        while(operandCount*4+1 > recieveLength) {
            receiveCount = (int)read(client_socket, &operandInfo[recieveLength], BUF_SIZE-1);
            recieveLength += receiveCount;
        }
        result = calculate(operandCount, (int*)operandInfo, operandInfo[recieveLength-1]);
        write(client_socket, (char*)&result, sizeof(result));
        close(client_socket);
    }
    close(server_socket);
    return 0;
}

int calculate(int opnum, int opnds[], char op) {
    int result=opnds[0], i;
    
    switch(op)
    {
    case '+':
        for(i=1; i<opnum; i++) result+=opnds[i];
        break;
    case '-':
        for(i=1; i<opnum; i++) result-=opnds[i];
        break;
    case '*':
        for(i=1; i<opnum; i++) result*=opnds[i];
        break;
    }
    return result;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

