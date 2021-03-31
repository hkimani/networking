// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// For creating an endpoint for communication
#include <sys/types.h>
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs - convert values between host and network byte order
#include <netinet/in.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close


int main()
{
    // Specifying the adress for the socket.

    struct sockaddr_in server_address;

    
    // creating a socket that connects to server
    int server_socket;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
         
    // AF_INET: IPv4 Internet protocols
    // SOCK_STREAM: Reliable two way connection. (TCP vs UDP)

    // arg1: domain.
    // arg2: socket type (SOCK_DGRAM)
    // arg3: protocol (TCP vs UDP)

    // ... server address is a structure specifying the address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9006); // PORT. htons function converts the unsigned short integer hostshort from host byte order to network byte order.
    server_address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
      

    // Client connects to server
    //int connection_status = connect(server_socket, (struct sockaddr *) &server_address, sizeof (server_address));
    // arg1: network socket
    // arg2: address to which to which datagrams are sent by default. And the address from which datagrams are received.
    // arg3: Size of the structure: server address. Size of arg2

    // check for error with the connection
    if (!(server_socket > 0))
    {
        printf("There was an error making a connection to the remote socket \n\n");
        return -1;
    }  
    
    int size = sizeof(server_address);

    // Receive data from the server
    char server_response[256];

    // Client receives a message from the server
    recvfrom(server_socket, server_response, 256, 0, (struct sockaddr *)&server_address,&size);
    // param 1: socket
    // param 2: response storage (buffer)
    // param 3: size of the response storage (buffer)
    // param 4: flag

    // Output the data we get from the server
    printf("data");
    printf("The server sent the data: %s \n ",server_response);
        
    // Close the socket connection
    close(server_socket); 

    return 0;
}