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

int main()
{
    // Socket variable
    struct sockaddr_in server;

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    int conn;

    // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(9051);
    server.sin_addr.s_addr = INADDR_ANY;

    // Client connects to server
    int connection_status = connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.
    
    // check for error with the connection
    if (connection_status == -1)
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }


    while (1)
    {
        printf("Enter a message: ");
        fgets(message, 100, stdin);
        send(fd, message, strlen(message), 0);
        recv(fd, new_message, sizeof(new_message), 0);
        printf("Response: %s\n", new_message);
        //An extra breaking condition can be added here (to terminate the while loop)
    }

    close(fd);

    return 0;
}
