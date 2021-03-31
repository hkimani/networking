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


#define PORT 9051  // The port

// Client structure
struct client
{
    char name[256];
    int sid;
    bool online;
};

// Function to print errors and exit
void error(const char *msg){
    perror(msg);
    exit(1);
}


//Function to handle client connections (communications with server)
void handleClient(int client_socket){

    // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";
    char response[256] = "Connection success";

    while (client_socket)
    {
        recv(client_socket, message, 255, 0);
        printf("Response: %s\n", message);

        // If the server receives "DISCONNECT", it terminates connection with the client
        if(strncmp(message, "DISCONNECT", 10) == 0){
            strcpy(new_message, "DISCONNECT SUCCESSFUL");
            send(client_socket, new_message, strlen(new_message), 0);

            // close the client socket and break out of the function, back to the server's waiting state
            break;

        }
        printf("Enter a message: ");
        // strcpy(new_message, "You have reached the server"); /** uncomment line & comment below line 4 clean demo **/
        fgets(new_message, 255, stdin);
        send(client_socket, new_message, strlen(new_message), 0);
    }

    close(client_socket);
    printf("[SERVER] Terminated connection \n");
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

    // Check for errors
    if (fd < 0)
        error("Connection failed. Error occurred \n");


    // Assigns identifier to the socket, prints an error if the binding fails
    if(bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
        error("Socket Binding Failed");
    else{
        printf("[Server]: Server starting....\n");
        printf("[Server] listening at port %d \n", PORT);
    }


    // Listen for connections from clients. Max: 3
    listen(fd, 10);

    while(1){ // The server listens eternally
        printf("[Server] Waiting for connections \n");
        // Save client socket
        int client_socket = accept(fd, (struct sockaddr *)NULL, NULL);
        if (client_socket)
            printf("[SERVER] Client %d Connected successfully\n", client_socket);

        handleClient(client_socket); // handle clients (one at a time)
    }


    // Close socket (unreachable though)
    close(fd);

    return 0;
}