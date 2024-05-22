#include <stdio.h>
#include <stdlib.h>
#include "molido.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#define MAP_SIZE 256

// Init the Map
static int map[MAP_SIZE][MAP_SIZE] = {0};

int main(int argc, char** argv) {

    int isLogScale = 0;
    char* targetFile;
    parseArgs(argc, argv, &targetFile, &isLogScale);

    FILE* fptr;

    fptr = fopen(targetFile, "rb");
    if (!fptr) {
        printf("[ERROR] The file is not opened. Aborting the mission\n");
        exit(0);
    }

    // Fie Operation
    rewind(fptr);
    
    // Traverse the file
    int init = 1;
    int prevByte;
    int currByte;
    while((currByte = fgetc(fptr)) != EOF) {
        if (init) {
            prevByte = currByte;
            init = 0;
            continue;
        }
        map[prevByte][currByte] += 1;
        prevByte = currByte;
    }

    if (ferror(fptr)) {
        printf("[ERROR] While reading file: %02X\n", currByte);  
    }
    fclose(fptr);

    const char* base = "%s.bustelo.png";
    int len = snprintf(NULL, 0, base, targetFile);
    char* output = (char*)malloc(len + 1);
    snprintf(output, len + 1, base, targetFile);

    writeMapToImage(output, map, isLogScale);

    // Clean up
    free(output);
    printf("\n");

    return 0;
}

