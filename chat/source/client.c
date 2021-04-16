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

#include <pthread.h>

#define PORT 9051
#define LENGTH 2048

char name[30];  // Will store the client's username

// File descriptor. (Socket Id)
int fd;

void str_overwrite_stdout() {
    printf("%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void send_msg_handler() {
    char message[LENGTH] = {};
    char buffer[LENGTH + 32] = {};

    while(1) {
        str_overwrite_stdout();
        fgets(message, LENGTH, stdin);
        str_trim_lf(message, LENGTH);

        if (strcmp(message, "DISCONNECT") == 0) {
            break;
        } else {
            sprintf(buffer, "%s: %s\n", name, message);
            send(fd, buffer, strlen(buffer), 0);
        }

        bzero(message, LENGTH);
        bzero(buffer, LENGTH + 32);
    }
}

void recv_msg_handler() {
    char message[LENGTH] = {};
    while (1) {
        int receive = recv(fd, message, LENGTH, 0);
        if (receive > 0) {
            printf("%s", message);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else {
            // -1
        }
        memset(message, 0, sizeof(message));
    }
}

int main()
{
    // Socket variable
    struct sockaddr_in server;

    // Connection file descriptor. (Connection Id)
    int conn;

    // Message store for messages from server
    char message[256] = "";
    char new_message[256] = "";

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Client connects to server
    int connection_status = connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.
    
    // check for error with the connection
    if (connection_status == -1)
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }

    printf("Please enter your name: ");
    fgets(name, 30, stdin);
    str_trim_lf(name, strlen(name));
    // Send name
    send(fd, name, 32, 0);

    printf("=== WELCOME TO THE CHATROOM ===\n");

    // send message
    pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    // receive message
    pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    close(fd);

    return 0;

}
