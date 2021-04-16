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
#define MAX_CLIENTS 20


/* Client structure */
typedef struct{
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];  //Array of clients in chatroom

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;  // for mutual exclusion

// Function to print errors and exit
void error(const char *msg){
    perror(msg);
    exit(1);
}

// Function to broadcast messages -- work in progress
void send_message(char *msg, int uid){  // takes the msg to send, and the id of the sender
    // function to broadcast message
    pthread_mutex_lock(&clients_mutex);

    for(int i=0; i<MAX_CLIENTS; ++i){
        if(clients[i]){
            if(clients[i]->uid != uid){  // send to every client except self
                if(write(clients[i]->sockfd, msg, strlen(msg)) < 0){
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
    char buff_out[BUFFER_SZ];
    char name[32];
    int leave_flag = 0;

    cli_count++;
    client_t *cli = (client_t *)arg;

    // Name
    if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32-1){
        printf("Didn't enter the name.\n");
        leave_flag = 1;
    } else{
        strcpy(cli->name, name);
        sprintf(buff_out, "%s has joined\n", cli->name);
        printf("%s", buff_out);
        send_message(buff_out, cli->uid);
    }

    bzero(buff_out, BUFFER_SZ);

    while(1){
        if (leave_flag) {
            break;
        }

        int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
        if (receive > 0){
            if(strlen(buff_out) > 0){
                send_message(buff_out, cli->uid);

                str_trim_lf(buff_out, strlen(buff_out));
                printf("%s -> %s\n", buff_out, cli->name);
            }
        } else if (receive == 0 || strcmp(buff_out, "exit") == 0){
            sprintf(buff_out, "%s has left\n", cli->name);
            printf("%s", buff_out);
            send_message(buff_out, cli->uid);
            leave_flag = 1;
        } else {
            printf("ERROR: -1\n");
            leave_flag = 1;
        }

        bzero(buff_out, BUFFER_SZ);
    }

    /* Delete client from queue and yield thread */
    close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    cli_count--;
    pthread_detach(pthread_self());

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