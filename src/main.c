#include <stdio.h>
#include <stdlib.h>
#include "molido.h"

#define MAP_SIZE 256



int main(void) {
    molido_function(); // Use the declared function
    printf("\n[INFO] Lauching ...\n");
    const char* fname = "data/avatar.png";
    FILE* fptr;

    fptr = fopen(fname, "rb");

    if (!fptr) {
        printf("[ERROR] The file is not opened. Aborting the mission\n");
        exit(0);
    }

    printf("[INFO] We are on the moons!\n");

    // Init the Map
    int map[MAP_SIZE][MAP_SIZE];
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = 0;
        }
    }

    // Fie Operation
    rewind(fptr);
    
    // Traverse the file
    int init = 1;
    int prevByte;
    int currByte;
    while((currByte = fgetc(fptr)) != EOF) {
        if (init == 1) {
            prevByte = currByte;
            init = 0;
            continue;
        }
        map[prevByte][currByte] += 1;
    }

    if (ferror(fptr)) {
        printf("[ERROR] While reading file: %02X\n", currByte);  
    } else if(feof(fptr)) {
        printf("[INFO] End of file reached successfilly\n");
    }

    // Some cleanup
    fclose(fptr);

    writeMapToImage("output/map.png", map);
    
    return 0;
}


