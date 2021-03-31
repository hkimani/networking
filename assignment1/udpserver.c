// Standard library
#include <stdio.h>
#include <stdlib.h>

// For creating an endpoint for communication
#include <sys/types.h>
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs: convert values between host and network byte order
#include <netinet/in.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close


int main() {

    // Define the server address and client which is of type: sockaddr_in
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    // Server response message
    char server_message[256] = "You have reached the server";


    // create socket
    int server_socket; //file descriptor
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);


    // AF_INET: IPv4 Internet protocols
    // SOCK_STREAM: Reliable two way connection (full-duplex byte streams). (TCP vs UDP)
    // arg1: domain.
    // arg2: socket type 
    // arg3: protocol (TCP vs UDP)

    
    server_address.sin_family = AF_INET; // Expected protocols. Domain
    server_address.sin_port = htons(9006); // PORT. function htons converts the unsigned short integer hostshort from host byte order to network byte order.
    server_address.sin_addr.s_addr = INADDR_ANY; // Endpoint ip address: 0.0.0.0
      
    // Bind the socket to our specified IP and Port
    // arg1: socket connection, 
    // arg2: server_address of cast type sockaddr, 
    // arg3: size of the server address

    if (!(server_socket > 0))
    {
        printf("Connection failed. Error occurred \n");
    }

    bind(server_socket,(struct sockaddr *) &server_address, sizeof(server_address));

    // Listen for connections
    // arg1: socket connection
    // arg2: N connection requests will be queued before further requests are refused.
    //listen(server_socket, 5);

    // Store the connection socket state with the client
    //int client_socket;
    //client_socket = socket(AF_INET, SOCK_DGRAM, 0);

  
    // Server accepting client connection request
    //client_socket = accept(server_socket, NULL, NULL);
    // arg1: a socket that has been created with socket()
    // arg2: pointer to a sockaddr structure.
    // arg2: ... the structure is filled in with the address of the peer socket as known to the communications layer
    // arg3: address length of argument 2
    // Sending a message to the client
        fgets(server_message, 100, stdin);

    sendto(server_socket,server_message,256,0,(struct sockaddr*)&client_address,sizeof(client_address));





    // Close the socket connection
    close(server_socket);

    return 0;
}