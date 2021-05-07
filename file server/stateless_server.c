#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2048
#define PORT 9051

void error(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

// function to trim input line feed
void str_trim_lf(char * arr, int length){
    for(int i=0; i<length; i++){
        if(arr[i] == '\n'){
            arr[i] = '\0';
            break;
        }
    }
}
// function to read from file
char * readFile(char * filename, int start, int size){
    FILE *input;

    input = fopen(filename, "rb");

    if(!input){  // if File does not exist, inform the client
        char * error = "The file does not exist!";
        return error;
    }

    // Move file to start position
    fseek(input, start, SEEK_CUR);

    char* buffer = (char*) calloc (1, size+1);
    buffer[0] = '\0';

    // read the input file
    fread(buffer, size, 1, input);

    fclose(input);
   // free(buffer);

    return buffer;
}

// function to read from file
char * writeFile(char * filename, int start, char * message){
    FILE *input;

    input = fopen(filename, "wb");

    if(!input){  // if File does not exist, inform the client
        char * error = "The file does not exist!";
        return error;
    }

    //Go to start position
    fseek(input, start, SEEK_CUR);

    // write to file
    fwrite(message, strlen(message), 1, input);

    fclose(input);

    return NULL;
}

// Function to set up the server
int setup_server(int server_port, int backlog){
    int server_socket;
    struct sockaddr_in server_address;

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Error: Creating Socket");
    }

    //Initialize the Address Struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    // bind
    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        error("Error: Binding Failed");
    }

    //listen
    if(listen(server_socket, backlog) < 0){
        error("Error: Listening Error");
    }

    return server_socket;
}

// function to accept new client connections
int accept_new_connection(int server_socket){
    int addr_size = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client_addr;

    if((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t *)&addr_size)) < 0)
        error("Error: Accept connection Failed");

    return client_socket;
}

//function to handle client connections
void handleClient(int fd){
    char buffIn[BUFFER_SIZE], buffOut[BUFFER_SIZE], message[BUFFER_SIZE];
    char filename[100], start[10], end[10];

    recv(fd, buffIn, BUFFER_SIZE, 0);

    // Extract Filename, start, end position and message
      bzero(filename, sizeof(filename)); // clear out filename
      bzero(start, sizeof(start)); // clear out start
      bzero(end, sizeof(end)); // clear out end
      bzero(message, sizeof(message)); // clear out message
      int par_count = 0;

    for(int i=0; i< strlen(buffIn); ++i){
        if(buffIn[i] == ' ' || buffIn[i] == 10){ // filename
            par_count++;
            continue;
        }
        if(buffIn[i] == 'r' || buffIn[i] == 'w'){
            i++;
            continue;
        }
        if(par_count == 0)
            strcat(filename, (char[]){buffIn[i], 0});
        if(par_count == 1)
            strcat(start, (char[]){buffIn[i], 0});
        if(par_count == 2)
            strcat(end, (char[]){buffIn[i], 0});
        if(par_count == 3)
            memcpy(message, buffIn+i, strlen(buffIn)-i);
    }

    if(strncmp(buffIn, "r", 1) == 0){ // A read request
        bzero(buffOut, sizeof(buffOut));
        int size_to_read = atoi(end) - atoi(start);
        strcpy(buffOut, readFile(filename, atoi(start), size_to_read));
    }
    if(strncmp(buffIn, "w", 1) == 0){  // A write request
        bzero(buffOut, sizeof(buffOut));
        writeFile(filename, atoi(start),  message);
        strcpy(buffOut, "WRITE SUCCESSFUL");
    }

     send(fd, buffOut, BUFFER_SIZE, 0);

     close(fd);
}

int main(){

    int server_socket = setup_server(PORT, 10);

    for(;;){

        int client_socket = accept_new_connection(server_socket);

        handleClient(client_socket);


    }

    return 0;
}
