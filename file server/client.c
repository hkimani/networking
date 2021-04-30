// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For creating an endpoint for communication
#include <sys/types.h>
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs - convert values between host and network byte order
#include <netinet/in.h>

#include <fcntl.h>  // for open
#include <unistd.h> // for close

#define PORT 9051

int main()
{
    // Socket variable
    struct sockaddr_in server;

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    int conn;

    // Message store for messages from server
    char message[256];
    char response[256];

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Client connects to server
    int connection_status = connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.

    // check for error with the connection
    if (connection_status == -1)
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }
    else
    {
        printf("Enter DISCONNECT to terminate connection\n");
        recv(fd, &response, sizeof(response), 0);
        printf("--%s--", response);
        printf("\n");
    }

    for (;;){

       //get the request from the client
       fgets(message, sizeof(message), stdin);
       if(send(fd, message, strlen(message), 0) < 0)
                printf("did not send message to server");

        //If the client receives a disconnect successful message, it terminates
        if (strncmp("DISCONNECT SUCCESSFUL", response, strlen(response)) == 0)
            break;

    }

    close(fd);

    return 0;
}