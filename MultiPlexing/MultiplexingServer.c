//
//  MultiplexingServer.c
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
#include <sys/select.h>
#include <sys/wait.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_childprocess(int signal);

int main(int argc, const char * argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    struct timeval timeout;
    fd_set reads, copy_reads;
    
    socklen_t address_size;
    int fd_max, str_len, fd_num;
    char buf[BUF_SIZE];
    
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    if(listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }
    
    FD_ZERO(&reads);
    FD_SET(server_socket, &reads);
    fd_max = server_socket;
    
    while (1) {
        copy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fd_num=select(fd_max+1, &copy_reads, 0, 0, &timeout))==-1)
            break;
        
        if(fd_num==0)
            continue;

        for(int i=0; i<fd_max+1; i++) {
            if(FD_ISSET(i, &copy_reads)) {
                if(i == server_socket) {
                    address_size = sizeof(client_address);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &address_size);
                    FD_SET(client_socket, &reads);
                    if(fd_max < client_socket)
                        fd_max = client_socket;
                    printf("connected client: %d \n", client_socket);
                } else {
                    str_len = (int)read(i, buf, BUF_SIZE);
                    if(str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    } else {
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    
    close(server_socket);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childprocess(int signal) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d\n", pid);
}
