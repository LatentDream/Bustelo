#include <stdio.h>
#include <stdlib.h>



int main(void) {
    
    printf("\nLauching ...");
    const char* fname = "./data/avatar.png";
    FILE* fptr;

    fptr = fopen(fname, "r");

    if (!fptr) {
        printf("The file is not opened. Aborting the mission");
        exit(0);
    }

    printf("We are on the moons!\n");

    // Fie Operation
    rewind(fptr);
    
    int c;
    while((c = fgetc(fptr) != EOF)) {
        putchar(c);
    }

    if (ferror(fptr)) {
        printf("I/O Error when reading file");
    } else if(feof(fptr)) {
        printf("\nEnd of file reached successfilly\n\n");
    }

    // Some cleanup
    fclose(fptr);
    return 0;

}
