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
#define BUFFER_SIZE 2048

static unsigned int cli_count = 0;
static int uid = 10;

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

/* Add clients to queue */
void enqueue(client_t *cl){
    pthread_mutex_lock(&clients_mutex);

    for(int i=0; i < MAX_CLIENTS; ++i){
        if(!clients[i]){
            clients[i] = cl;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

/* Remove clients from queue */
void dequeue(int uid){
    pthread_mutex_lock(&clients_mutex);

    for(int i=0; i < MAX_CLIENTS; ++i){
        if(clients[i]){
            if(clients[i]->uid == uid){
                clients[i] = NULL;
                break;
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

//Function to clear the screen
void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

// function to trim input
void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
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
void *handleClient(void *arg){
    char buff_out[BUFFER_SIZE];
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

    bzero(buff_out, BUFFER_SIZE);

    while(1){
        if (leave_flag) {
            break;
        }

        int receive = recv(cli->sockfd, buff_out, BUFFER_SIZE, 0);
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

        bzero(buff_out, BUFFER_SIZE);
    }

    /* Delete client from queue and yield thread */
    close(cli->sockfd);
    dequeue(cli->uid);
    free(cli);
    cli_count--;
    pthread_detach(pthread_self());

    return NULL;
}

int main()
{

    // Socket variable
    struct sockaddr_in server;    //server
    struct sockaddr_in cli_addr;  //client

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


    // Listen for connections from clients. Max: 10
    listen(fd, 10);

    printf("=== WELCOME TO THE CHATROOM ===\n");

    pthread_t thread;

    for(;;){
        socklen_t clilen = sizeof(cli_addr);
        conn = accept(fd, (struct sockaddr*)&cli_addr, &clilen);

        /* Check if max clients is reached */
        if((cli_count + 1) == MAX_CLIENTS){
            printf("Max clients reached. Connection Rejected ");
            close(conn);
            continue;
        }

        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->address = cli_addr;
        cli->sockfd = conn;
        cli->uid = uid++;

        /* Add client to the queue and fork thread */
        enqueue(cli);
        pthread_create(&thread, NULL, &handleClient, (void*)cli);

    }

    return 0;
}


