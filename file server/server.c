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
#define BUFF_SIZE 2048


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
    char message[BUFF_SIZE];
    char response[BUFF_SIZE];
    char filename[50];
    char size[10];
    char operation[2];

    // while (client_socket)
    // {
        // Receive request from the client
        int receive = recv(client_socket, response, BUFF_SIZE, 0);
        if (receive < 0){
            perror("Server Recv Errror: ");
            return;
        }

        // Extract filename and size
        bzero(filename, sizeof(filename));
        bzero(size, sizeof(size));
        bzero(operation, sizeof(operation));
        for(int i=0; i<strlen(response); i++){
            if(response[i] == ' ' || response[i] == 10){
                break;
            }
            
        }

        //Request handling
        if (strncmp(response, "r", 1) == 0){ // READ FROM FILE
            printf("\n READ \n");
        }
        if (strncmp(response, "w", 1) == 0){  // WRITE TO FILE
            printf("WRITE");
        }
        printf(" End of is statements ");
        
    // }

    close(client_socket);
}

int main()
{

    // Socket variable
    struct sockaddr_in server;
    struct sockaddr_in cli_addr;

    // File descriptor. (Socket Id)
    int fd;

    // Connection file descriptor. (Connection Id)
    int conn;

    server.sin_family = AF_INET;         // Address family. IPv4 Internet protocols
    server.sin_port = htons(PORT);       // Listener port
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    // Create socket and return Socket Id
    fd = socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    // set socket options
    if (setsockopt(fd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0)
    {
        perror("ERROR: setting socking options failed");
        return EXIT_FAILURE;
    }

    // Check for errors
    if (fd < 0)
        error("Connection failed. Error occurred \n");

    // Assigns identifier to the socket, prints an error if the binding fails
    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        error("Socket Binding Failed");
        return EXIT_FAILURE;
    }

    // Listen for connections from clients. Max: 3
    if (listen(fd, 10) < 0)
    {
        perror("ERROR: Listening Failed");
        return EXIT_FAILURE;
    }

    socklen_t clilen = sizeof(cli_addr);
    printf("[Server] listening at port %d \n", PORT);
    // for (;;)
    // { // The server listens eternally

        
        // Save client socket
        conn = accept(fd, (struct sockaddr *)&cli_addr, &clilen);

        handleClient(conn); // handle clients (one at a time)
    // }

    // Close socket (unreachable though)
    close(fd);

    return 0;
}