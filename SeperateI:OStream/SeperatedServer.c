//
//  SeperatedServer.c
//  SocketProgramming
//
//  Created by 오승기 on 2022/10/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, const char * argv[]) {
    int server_socket, client_socket;
    FILE *readfp;
    FILE *writefp;
    
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size;
    char buf[BUF_SIZE];
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    
    client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_size);
    
    readfp = fdopen(client_socket, "r");
    writefp = fdopen(dup(client_socket), "w");
    
    fputs("FROM SERVER: HI CLIENT\n", writefp);
    fflush(writefp);
    
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);
    
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    
    return 0;
}
