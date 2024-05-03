#include <stdio.h>
#include <stdlib.h>
#include "molido.h"

#define MAP_SIZE 256

// Init the Map
static int map[MAP_SIZE][MAP_SIZE] = {0};

int main(void) {
    printf("\n[INFO] Lauching ...\n");
    const char* fname = "data/avatar.png";
    const char* extension = getExtension(fname);
    FILE* fptr;

    fptr = fopen(fname, "rb");
    if (!fptr) {
        printf("[ERROR] The file is not opened. Aborting the mission\n");
        exit(0);
    }

    printf("[INFO] We are on the moons!\n");


    // Fie Operation
    rewind(fptr);
    
    // Traverse the file
    int init = 1;
    int prevByte;
    int currByte;
    int max = 0;
    while((currByte = fgetc(fptr)) != EOF) {
        if (init) {
            prevByte = currByte;
            init = 0;
            continue;
        }
        map[prevByte][currByte] = 1;
        if (map[prevByte][currByte] > max) {
            max = map[prevByte][currByte];
        }
        prevByte = currByte;
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = map[i][j] * 255;
        }
    }


    if (ferror(fptr)) {
        printf("[ERROR] While reading file: %02X\n", currByte);  
    } else if(feof(fptr)) {
        printf("[INFO] End of file reached successfilly\n");
    }

    // Some cleanup
    fclose(fptr);

    const char* base = "output/b_%s.png";
    int len = snprintf(NULL, 0, base, extension);
    char* output = (char*)malloc(len + 1);
    snprintf(output, len + 1, base, extension);

    writeMapToImage(output, map);
    writeMapToCLI(map);

    // Clean up
    free(output);
    printf("\n");

    return 0;
}


