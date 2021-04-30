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

#define PORT 9051 // The port

// Client structure
struct client
{
    char name[256];
    int sid;
    bool online;
};

// Function to print errors and exit
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//Function to handle client connections (communications with server)
void handleClient(int client_socket)
{

    // Message store for messages from server
    char message[64] = "Enter read or write for a request";
    char response[10];


    if (send(client_socket, message, strlen(message), 0) < 0)
    {
        printf("Error: Send");
    }
    else
    {
        // Receive request from the client
        recv(client_socket, &response, sizeof(response), 0);
        printf("---%s---", response);
        printf("\n");
        //Request handling
        if (strncmp(response, "read", 4) == 0)
        {
            printf("READ");
        }
        else if (strncmp(response, "write", 5) == 0)
        {
            printf("WRITE");
        }
        else
        {
            printf("Dangling else");
        }

        close(client_socket);
    }
}

int main()
{

    // Socket variable
    struct sockaddr_in server;

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    int conn;

    bool connected[2] = {false, false};

    server.sin_family = AF_INET;         // Address family. IPv4 Internet protocols
    server.sin_port = htons(PORT);       // Listener port
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // Create socket and return Socket Id
    fd = socket(AF_INET, SOCK_STREAM, 0);

    // int option = 1;
    // set socket options
    // if(setsockopt(fd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0){
    //     perror("ERROR: setting socking options failed");
    //     return EXIT_FAILURE;
    // }

    // Check for errors
    if (fd < 0)
        error("Connection failed. Error occurred \n");

    // Assigns identifier to the socket, prints an error if the binding fails
    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
        error("Socket Binding Failed");
    else
    {
        printf("[Server] listening at port %d \n", PORT);
    }

    // Listen for connections from clients. Max: 3
    listen(fd, 10);

    for (;;)
    { // The server listens eternally

        // Save client socket
        int client_socket = accept(fd, (struct sockaddr *)NULL, NULL);

        handleClient(client_socket); // handle clients (one at a time)
    }

    // Close socket (unreachable though)
    close(fd);

    return 0;
}