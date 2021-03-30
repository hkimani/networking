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

// Client structure
struct client
{
    char name[256];
    int sid;
    bool online;
};

int main()
{

    // Socket variable
    struct sockaddr_in server;
    struct sockaddr_in client; //destination address




    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    //int conn;

    // Create socket and return Socket Id
    fd = socket(AF_INET, SOCK_DGRAM, 0);



    // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";
    char response[256] = "Connection success";

    bool connected[2] = {false, false};

    server.sin_family = AF_INET;         // Address family. IPv4 Internet protocols
    server.sin_port = htons(9056);       // Listener port
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    

    
    

    // Check for errors
    if (!(fd > 0))
    {
        printf("Connection failed. Error occurred \n");
    }

    // Assigns identifier to the socket
    bind(fd, (struct sockaddr *)&server, sizeof(server));

    // Listen for connections from clients. Max: 10
    //listen(fd, 10);

    // Save client socket
    //int client_socket = accept(fd, (struct sockaddr *)NULL, NULL);
    // Connection handling
    int size = sizeof(client);
    while (1)
    {
        recvfrom(fd, message, 256, 0,(struct sockaddr*)&client,&size);
        printf("Response from client: %s  \n", message);
        printf("Server: Enter a message: ");
        fgets(new_message, 100, stdin);
        sendto(fd, new_message, 256, 0,(struct sockaddr*)&client,sizeof(client));
    }

    // // Close socket
    close(fd);

    return 0;
}