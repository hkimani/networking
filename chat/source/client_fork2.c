#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
// Global variables

int sockfd = 0;
char name[32];

//function to trim the input
void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}


int main (){
    int clientsocket,ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientsocket = socket(AF_INET,SOCK_STREAM, 0);
    if (clientsocket < 0)
    {
        printf("Error in setting up socket . \n");
        exit(1);
    }
    memset(&serverAddr , '\0' ,sizeof(serverAddr));
    serverAddr.sin_family= AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(clientsocket, (struct sockaddr* )&serverAddr, sizeof(serverAddr));
    if (ret<0)
    {
        printf("Error in connect  . \n");
        exit(1);
    
    }
    printf("Connection to server  Success \n");

    while (1)
    {
        printf("Client: \t");
        scanf("%s",&buffer[0]);
        send(clientsocket, buffer,strlen(buffer),0);

        if (strcmp(buffer,"DISCONNECT") == 0)
        {
            close(clientsocket);
            printf("connection terminated \n");
            exit(1);
        }

        if (recv(clientsocket,buffer,1024,0)< 0)
        {
           printf("Error in recieve message . \n");
               
        }else
        {
            printf("Server: \t %s \n ",buffer);
        }
        
        
        
    }
    
    
    return 0;


    
}

