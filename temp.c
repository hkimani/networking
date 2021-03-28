// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main()
{

    // Client structure
    struct client
    {
        char name[256];
        int sid;
        bool online;
    };

    struct client clients[5];

    struct client *id;
    strcpy(id->name, "Jina");
    id->sid = 1;

    if (!connected[0])
    {
        struct client c1 = {"c1", client_socket};
        connected[0] = true;
        printf("Initialized client 1 \n");
    }
    else
    {
        struct client c2 = {"c2", client_socket};
        connected[0] = true;
        printf("Initialized client 2 \n");
    }

    return 0;
}
