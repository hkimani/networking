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

#define PORT 9051
#define LENGTH 2048

//function to trim the input
void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

int main()
{
    // Socket variable
    struct sockaddr_in server;

    // File descriptor. (Socket Id)
    int fd;

    // Message store for messages from server
    char message[LENGTH];
    char response[LENGTH];

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(server);

    // Client connects to server
    int connection_status = connect(fd, (struct sockaddr *)&server, len); //This connects the client to the server.

    // check for error with the connection
    if (connection_status == -1)
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }
    else
    {
        printf("Enter DISCONNECT to terminate connection\n");
        // recv(fd, &response, sizeof(response), 0);
        // printf("--%s--", response);
        // printf("\n");
    }

    for (;;){

       //get the request from the client
       printf("Enter: \" w\" \"filename\" \" size\" for a write request and  \" r \" \"filename\" \" size\" for a read request \n");
       fgets(message, LENGTH, stdin);
       str_trim_lf(message, LENGTH);
       if(send(fd, message, strlen(message), 0) < 0)
            printf("ERROR: Send to Server failed");

        // Receive response from server (Read or write instructions)
        bzero(message, 0);
        if(recv(fd, message, LENGTH, 0) < 0){
            printf("ERROR: Receive from server failed");
        }
        printf("%s\n", message);
    }

    close(fd);

    return 0;
}