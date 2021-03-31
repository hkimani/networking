// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// For creating an endpoint for communication
#include <sys/types.h>
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs: convert values between host and network byte order.
// AF_INET (Adress Family)
#include <netinet/in.h>

#include <fcntl.h>  // for open
#include <unistd.h> // for close



int main()
{

    // Socket variable
    struct sockaddr_in server; //server address structure
    struct sockaddr_in client; //destination address structure

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    //int conn;

    // Create socket and return Socket Id
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Message store for messages from server
    char message[256] = ""; // buffer for receiving messages from client
    char new_message[256] = "You have reached the server"; //message sent by server
    //char response[256] = "Connection success";

    //bool connected[2] = {false, false};

    server.sin_family = AF_INET;         // Address family. IPv4 Internet protocols
    server.sin_port = htons(9056);       // Listener port
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // Check for errors if socket is not 1
    if (!(fd > 0))
    {
        printf("Connection failed. Error occurred \n");
    }

    // Binds server address to the socket
    bind(fd, (struct sockaddr *)&server, sizeof(server));

    // Listen for connections from clients. Max: 10
    //listen(fd, 10);

    // Save client socket
    //int client_socket = accept(fd, (struct sockaddr *)NULL, NULL);
    // Connection handling
    int size = sizeof(client); //size of client address

    recvfrom(fd, message, 256, 0, (struct sockaddr *)&client, &size); //function that recieves meessages from client
    printf("Response from client: %s  \n", message);
    sendto(fd, new_message, 256, 0, (struct sockaddr *)&client, sizeof(client)); //function that sends messages to client

    // // Close socket
    close(fd);

    return 0;
}