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

#define PORT 9051 //port definition


     struct sockaddr_in client; //destination address
     // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";
    char response[256] = "Connection success";
    int size = sizeof(client);

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
void *handleClient(void *pclient_socket )
{
          // struct sockaddr_in client;

           //type cast the client socket to an integer pointer
       int client_socket = *((int *)pclient_socket);
    free(pclient_socket); //free the pointer

    //type cast the client socket to an integer pointer
    //int fd = *((int *)pfd);
    //free(pfd); //free the pointer

    
         // Create socket and return Socket Id
    
    while (1)
    {
       // printf("[Server] Waiting for connections \n");

        recvfrom(client_socket, message, 255, 0,(struct sockaddr*)&client,&size);
        printf("Response from client in handle sec: %s\n", message);

        // If the server receives "DISCONNECT", it terminates connection with the client
        // if(strncmp(message, "DISCONNECT", 10) == 0){
        //     strcpy(new_message, "DISCONNECT SUCCESSFUL");
        //     sendto(client_socket, new_message, strlen(new_message), 0,(struct sockaddr*)&client,sizeof(client));

        //     //  break out of the function, close the client socket and back to the server's waiting state
        //     break;

        // }
        // strcpy(new_message, "You have reached the server");
        printf("Enter a message to send to client: ");
        fgets(new_message, 255, stdin);
        sendto(client_socket, new_message, strlen(new_message), 0,(struct sockaddr*)&client,sizeof(client));

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
   // int conn;

    bool connected[2] = {false, false};

    server.sin_family = AF_INET;         // Address family. IPv4 Internet protocols
    server.sin_port = htons(PORT);       // Listener port
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    // Create socket and return Socket Id
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Check for errors
    if (fd < 0)
        error("Connection failed. Error occurred \n");


    // Assigns identifier to the socket, prints an error if the binding fails
    
    if(bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
        error("Socket Binding Failed");
    else{
        printf("[Server]: Server starting....\n"); // server is starting 
       // printf("[Server] Waiting for connections \n");
       // printf("[SERVER] Client %d Connected successfully\n", fd);

    }

    int client_count= 0; //initialising counter variable
    


    // Listen for connections from clients. Max: 3
    //listen(fd, 10);


    while(1){ // The serverwaits for connectionsenternally
      printf("[Server] Waiting for connections \n"); 
       int client_socket = recvfrom(fd, message, 256, 0,(struct sockaddr *)&client,&size); 


         // Save client socket
        if (client_socket){
            printf("[Server] listening at port %d \n", PORT); //port specification server 
            printf("[SERVER] Client %d Connected successfully\n", client_count);
            ++client_count;
         printf("[Server] Waiting for connections \n");

        recvfrom(client_socket, message, 255, 0,(struct sockaddr*)&client,&size);
        printf("Response from client: %s\n", message);

         strcpy(new_message, "You have reached the server");
        
        sendto(client_socket, new_message, strlen(new_message), 0,(struct sockaddr*)&client,sizeof(client));


        


        // Do stuff with the connection
        pthread_t thread;
        //creating a pointer to arguments to be passed to the thread (pthread only takes pointers)
       int* pclient_socket = malloc(sizeof(int));
       *pclient_socket = fd;  // the pointer points to the client socket connection

        pthread_create(&thread, NULL, handleClient, pclient_socket);
       // pthread_join(thread);
   }

    }

    // // Close socket
     //close(fd);

    return 0;
}