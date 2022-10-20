//
//  MultiTaskingServer.c
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
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_childprocess(int signal);

int main(int argc, const char * argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    
    pid_t pid;
    struct sigaction act;
    socklen_t address_size;
    int str_len, state;
    char buf[BUF_SIZE];
    
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    act.sa_handler = read_childprocess;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        error_handling("Bind() error");
    }
    
    if(listen(server_socket, 5) == -1) {
        error_handling("Listen() error");
    }
    
    while (1) {
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*) &client_address, &address_size);
        
        if(client_socket == -1) {
            continue;
        } else {
            puts("new client connected....");
        }
        
        pid = fork();
        
        if(pid == -1) {
            close(client_socket);
            continue;
        } else if(pid == 0) {
            close(server_socket);
            while ((str_len = (int)read(client_socket, buf, BUF_SIZE)) != 0) {
                write(client_socket, buf, str_len);
            }
            close(client_socket);
            puts("client disconnected");
            return 0;
        } else {
            close(client_socket);
        }
    }
    
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
