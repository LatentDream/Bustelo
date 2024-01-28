#include <stdio.h>
#include <stdlib.h>
#include "molido.h"



int main(void) {
    molido_function(); // Use the declared function
    printf("\nLauching ...");
    const char* fname = "data/avatar.png";
    FILE* fptr;

    fptr = fopen(fname, "rb");

    if (!fptr) {
        printf("The file is not opened. Aborting the mission");
        exit(0);
    }

    printf("We are on the moons!\n");

    // Fie Operation
    rewind(fptr);
    
    int byte;
    while((byte = fgetc(fptr)) != EOF) {
        // printf("%02X ", byte);
    }

    if (ferror(fptr)) {
        printf("%02X ", byte);  
    } else if(feof(fptr)) {
        printf("\nEnd of file reached successfilly\n\n");
    }

    // Some cleanup
    fclose(fptr);
    return 0;

}
