//
//  SeperateClient.c
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
    int client_socket;
    char buf[BUF_SIZE];
    struct sockaddr_in server_address;
    
    FILE *readfp;
    FILE *writefp;
    
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));
    
    connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    
    readfp = fdopen(client_socket, "r");
    writefp = fdopen(client_socket, "w");
    
    while (1) {
        if(fgets(buf, sizeof(buf), readfp) == NULL) {
            break;
        }
        
        fputs(buf, stdout);
        fflush(stdout);
    }
    
    fputs("FROM CLIENT: Thank you\n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    
    return 0;
}

