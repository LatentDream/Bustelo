// molido.c
#include "molido.h"
#include "stb_image_write.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAP_SIZE 256


void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE], int isLogScale) {

    // Find max
    int max = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] > max) {
                if (isLogScale) {
                    max = logf(map[i][j]);
                } else {
                    max = map[i][j];
                }
            }
        }
    }

    static uint32_t pixels[MAP_SIZE][MAP_SIZE] = {0};
    // Normalize the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            float value;
            if (isLogScale) {
                value = logf((float)map[i][j]) / max;
            } else {
                value = (float)map[i][j] / max;
            }
            uint32_t b = value * 255;
            // Pixel format (RBGA): 0xAABBGGRR
            pixels[i][j] = 0xFF000000 | b | (b<<8) | (b<<16);
        }
    }
    stbi_write_png(filename, MAP_SIZE, MAP_SIZE, 4, pixels, MAP_SIZE * 3);
    printf("[INFO] Image file %s created\n", filename);
}

void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale) {

    // Print all args to the console
    printf("------ Welcome to Bustelo ------\n");
    // Check if "log" is passed
    printf("log: %d\n", *isLogScale);
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "log") == 0) {
            // Set log scale flag to true
            *isLogScale = 1;
        }
    }
    // Set the target file
    if (argc < 2) {
        printf("[PANIC] No target file is provided\n\n");
        exit(1);
    }
    *targetFile = argv[1];
    printf(" Target file: %s\n\n", *targetFile);
    printf(" Arguments:\n");
    printf("log: %d\n", *isLogScale);
    if (*isLogScale) {
        printf(" [X] log: Enable log scale\n");
    } else {
        printf(" [ ] log: Enable log scale\n");
    }
    printf("--------------------------------\n");
}
