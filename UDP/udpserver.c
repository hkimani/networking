//standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For creating an endpoint for communication
#include <sys/socket.h>

// htonl, htons, ntohl, ntohs - convert values between host and network byte order

#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}
int main ()
{
    int length,fromlen,n;
    //socket variable
    struct sockaddr_in server;
    struct sockaddr_in from;

    //File descriptor (socket Id)
     int sock;
    char buf[1024];

    

    sock=socket(AF_INET,SOCK_DGRAM,0);//CREATE SOCKET SOCKDGRAM species it is UDP
    if (sock < 0)
    {
        error("opening socket");
    }
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port = htons(9051);       // Listener port

    if (bind(sock,(struct sockaddr *)&server,length)<0)
    {
        error("binding");
    }
    fromlen = sizeof(struct sockaddr_in);

    while (1)
    {
        n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen); //
        if (n<0)
        {
            error ( "revfrom");
        }
        write( 1,"Recieved a Datagram: ",21);
        write(1,buf,n); 
        n=sendto(sock,"Got your message\n ",17,
                        0,(struct sockaddr *)&from,fromlen);
        if (n < 0)
        {
          error("sendto");
        }
        
        
    }
    
    
    
    
    
}