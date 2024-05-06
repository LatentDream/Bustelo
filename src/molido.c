// molido.c
#include "molido.h"
#include "stb_image_write.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAP_SIZE 256


void writeMapToImage(const char* filename, MapType* normalizedMap) {
    // Write a map to a png file
    // Expect the map to be already normalize (int value between 0 & 255
    static uint32_t pixels[MAP_SIZE][MAP_SIZE] = {0};
    // Normalize the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            uint32_t b = (*normalizedMap)[i][j];
            // Pixel format (RBGA): 0xAABBGGRR
            pixels[i][j] = 0xFF000000 | b | (b<<8) | (b<<16);
        }
    }
    stbi_write_png(filename, MAP_SIZE, MAP_SIZE, 4, pixels, MAP_SIZE * 3);
    printf("[INFO] Image file %s created\n", filename);
}


void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale, int* startGui ) {
    // Print all args to the console
    printf("------ Welcome to Bustelo ------\n");
    // Check if "log" is passed
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "log") == 0) {
            // Set log scale flag to true
            *isLogScale = 1;
        }
        if (strcmp(argv[i], "--gui") == 0) {
            // Set log scale flag to true
            *startGui = 1;
            return;
        }
    }
    // Set the target file
    if (argc < 2) {
        printf("[PANIC] No target file is provided\n\n");
        exit(1);
    }
    *targetFile = argv[1];
    printf(" To launch the GUI, pass --gui (other parameter will be ignore\n");
    printf(" Target file: %s\n\n", *targetFile);
    printf(" Arguments:\n");
    if (*isLogScale) {
        printf(" [X] log: Enable log scale\n");
    } else {
        printf(" [ ] log: Enable log scale\n");
    }
    printf("--------------------------------\n");
}


void fillMap(char* targetFile, MapType* map) {
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
        (*map)[prevByte][currByte] += 1;
        prevByte = currByte;
    }
    if (ferror(fptr)) {
        printf("[ERROR] While reading file: %02X\n", currByte);  
    }
    fclose(fptr);
}

void fillMap3D(char* targetFile, Map3DType* map) {
    FILE* fptr;
    fptr = fopen(targetFile, "rb");
    if (!fptr) {
        printf("[ERROR] The file is not opened. Aborting the mission\n");
        exit(0);
    }

    // Fie Operation
    rewind(fptr);
    
    // Traverse the file
    int init = 2;
    int zByte;
    int yByte;
    int xByte;
    while((xByte = fgetc(fptr)) != EOF) {
        switch (init) {
            case 2:
                yByte = xByte;
                init -= 1;
                break;
            case 1:
                zByte = yByte;
                yByte = xByte;
                init -= 1;
                break;
            case 0:
                (*map)[xByte][yByte][zByte] += 1;
                zByte = yByte;
                yByte = xByte;
                break;
        }
    }

    if (ferror(fptr)) {
        printf("[ERROR] While reading file: %02X\n", xByte);  
    }
    fclose(fptr);
}

void normaliseMap(MapType* map, int isLogScale) {
    // Find max
    int max = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if ((*map)[i][j] > max) {
                if (isLogScale) {
                    max = logf((*map)[i][j]);
                } else {
                    max = (*map)[i][j];
                }
            }
        }
    }
    // Normalize the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            float value;
            if (isLogScale) {
                value = logf((float)(*map)[i][j]) / max;
            } else {
                value = (float)(*map)[i][j] / max;
            }
            (*map)[i][j] = value * 255;
        }
    }
}

void normaliseMap3D(Map3DType* map, int isLogScale) {
    // Find max
    int max = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            for (int k = 0; k < MAP_SIZE; k++) {
                if ((*map)[i][j][k] > max) {
                    if (isLogScale) {
                        max = logf((*map)[i][j][k]);
                    } else {
                        max = (*map)[i][j][k];
                    }
                }
            }
        }
    }
    // Normalize the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            for (int k = 0; k < MAP_SIZE; k++) {
                float value;
                if (isLogScale) {
                    value = logf((float)(*map)[i][j][k]) / max;
                } else {
                    value = (float)(*map)[i][j][k] / max;
                }
                (*map)[i][j][k] = value * 255;
            }
        }
    }
}


void copyMap(MapType* orig, MapType* dest) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            (*dest)[i][j] = (*orig)[i][j];
        }
    }
}

void copyMap3D(Map3DType* orig, Map3DType* dest) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            for (int k = 0; k < MAP_SIZE; k++) {
                (*dest)[i][j][k] = (*orig)[i][j][k];
            }
        }
    }
}



