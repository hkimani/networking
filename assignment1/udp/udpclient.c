// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// For creating an endpoint for communication
#include <sys/types.h>
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs - convert values between host and network byte order
#include <netinet/in.h>

#include <fcntl.h>  // for open
#include <unistd.h> // for close

int main()
{
    // Socket variable
    struct sockaddr_in server;

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    // int conn;
       
        // message store for messages from server
    char message[256] = "Hi server"; //message sent by client to server
    char new_message[256] = ""; //buffer for client to recieve messages
    char response[256] = "Connection success";

    fd = socket(AF_INET, SOCK_DGRAM, 0); //connect UDP socket

    server.sin_family = AF_INET; //address family
    server.sin_port = htons(9056); //listening port specification 
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // Client connects to server
    // int connection_status = connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.

    // check for error with the connection
    if (!(fd > 0))
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }
    int size = sizeof(server);

    sendto(fd, message, 256, 0, (struct sockaddr *)&server, sizeof(server)); //function that sends message to server address

    //send(fd, message, strlen(message), 0);
    recvfrom(fd, new_message, 256, 0, (struct sockaddr *)&server, &size);  //function that receives new message from server

    //recv(fd, new_message, sizeof(new_message), 0);
    printf("Response from Server: %s \n", new_message);
    //An extra breaking condition can be added here (to terminate the while loop)


    close(fd);

    return 0;
}
