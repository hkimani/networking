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

//For creating threads
#include <pthread.h>

#define PORT 9051

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

// Handles the authentication process - Login, Account creation
int authenticate(int client_socket){
    char auth_prompt[] = "Welcome\n "
                         "choose an option (press 1 or 2): \n"
                         "1.  Login \n"
                         "2. Create New Account \n";
    char client_reply[255];

    if(client_socket){
        //send the authentication prompt
        send(client_socket, auth_prompt, strlen(auth_prompt), 0);
        recv(client_socket, client_reply, 255, 0);

        char username_prompt[] = "Enter your Username: ";
        char pass_prompt[] = "Enter your password: ";
        char pass2_prompt[] = "Confirm your password: ";
        char username[30], password[30], password2[30];

        if (strncmp("1", client_reply, 1) == 0) { // Log In
            // Get Username

            send(client_socket, username_prompt, strlen(username_prompt), 0);
            recv(client_socket, username, 30, 0);

            // Get the Password

            send(client_socket, pass_prompt, strlen(pass_prompt), 0);
            recv(client_socket, password, 30, 0);
            return 1;
        }
        else if (strncmp("2", client_reply, 1) == 0) { // Log In
            // Get Username
            send(client_socket, username_prompt, strlen(username_prompt), 0);
            recv(client_socket, username, 30, 0);

            // Get the Password
            send(client_socket, pass_prompt, strlen(pass_prompt), 0);
            recv(client_socket, password, 30, 0);
            // Get the Password confirmation
            send(client_socket, pass2_prompt, strlen(pass2_prompt), 0);
            recv(client_socket, password2, 30, 0);
            return 1;
        }
        else{
            printf("Unknown Option Selected \n");
        }

    }

    return 0;
}

//Function to handle client connections (communications with server)
void* handleClient(void* pclient_socket){

    //type cast the client socket to an integer pointer
    int client_socket = *((int *)pclient_socket);
    free(pclient_socket); //free the pointer

    // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";
    char response[256] = "Connection success";

    int authenticated = authenticate(client_socket);

    if (authenticated){ // If log in details are correct, continue with the client handling

       //  send the authenticated message to allow the client to proceed
        char auth_msg[50];
        strcpy(auth_msg, "AUTHENTICATED");
        send(client_socket, auth_msg, strlen(auth_msg), 0);


        while (client_socket){
            recv(client_socket, message, 255, 0);
            printf("Response: %s\n", message);

            // If the server receives "DISCONNECT", it terminates connection with the client
            if(strncmp(message, "DISCONNECT", 10) == 0){
                strcpy(new_message, "DISCONNECT SUCCESSFUL");
                send(client_socket, new_message, strlen(new_message), 0);

                //  break out of the function, close the client socket and back to the server's waiting state
                break;

            }
            // strcpy(new_message, "You have reached the server");
            printf("Enter a message: ");
            fgets(new_message, 255, stdin);
            send(client_socket, new_message, strlen(new_message), 0);

        }

    }


    close(client_socket);
    printf("[SERVER] Terminated connection \n");

    return NULL;
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


    for(;;){ // The server listens eternally
        printf("[Server] Waiting for connections \n");
        // Save client socket
        int client_socket = accept(fd, (struct sockaddr *)NULL, NULL);
        if (client_socket)
            printf("[SERVER] Client %d Connected successfully\n", client_socket);

        // Do stuff with the connection
        pthread_t thread;
        //creating a pointer to arguments to be passed to the thread (pthread only takes pointers)
        int* pclient_socket = malloc(sizeof(int));
        *pclient_socket = client_socket;  // the pointer points to the client socket

        pthread_create(&thread, NULL, handleClient, pclient_socket);
       // pthread_join(thread);
    }

    // // Close socket
    // close(fd);

    return 0;
}