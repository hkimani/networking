#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    FILE *input, *output;     

    input = fopen("input.txt", "rb");
    output = fopen("output.txt", "wb");

    // Get the size of the file (move seek to the end, get pos of seek, then reset seek)
    fseek(input, 0, SEEK_END);
    long fsize = ftell(input);
    fseek(input, 0, SEEK_SET);  

    if(!input)
        printf("Error Input File not found");

    if(!output)
        printf("Error: Output File not found");

    char* buffer = (char*) calloc (1, fsize+1);
    buffer[0] = '\0';

    // read the input file
     fread(buffer, fsize, 1, input);
    
    // Write to output file
    fwrite(buffer, strlen(buffer), 1, output);

    free(buffer);
    fclose(input);
    fclose(output);

    return 0;
}

