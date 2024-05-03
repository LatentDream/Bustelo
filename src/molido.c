// molido.c
#include "molido.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAP_SIZE 256

void writeMapToImage_bak(const char* filename, int map[MAP_SIZE][MAP_SIZE]) {
    unsigned char* pixels = (unsigned char*)malloc(3 * MAP_SIZE * MAP_SIZE); // 3 colors

    // Convert map data to pixel data
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            // Assuming your map values are between 0 and 255
            pixels[3 * (y * MAP_SIZE + x)    ] = (unsigned char)map[y][x]; // Red
            pixels[3 * (y * MAP_SIZE + x) + 1] = (unsigned char)map[y][x]; // Green
            pixels[3 * (y * MAP_SIZE + x) + 2] = (unsigned char)map[y][x]; // Blue
        }
    }

    // Write pixel data to image file
    stbi_write_png(filename, MAP_SIZE, MAP_SIZE, 3, pixels, MAP_SIZE * 3);
    printf("[INFO] Image file %s created\n", filename);
    free(pixels);
}

void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE]) {

    const int useLogScale = 1;

    // Find max
    size_t max = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] > max) {
                if (useLogScale) {
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
            if (useLogScale) {
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
}

const char* getExtension(const char* filename) {
    char* token;
    const char* extension;
    token = strrchr(filename, '.');
    if (token != NULL) {
        token++;
        extension = strdup(token);
        return extension;
    } else {
        printf("[ERROR] No extension found for %s\n", filename);
        exit(1);
    }
}

void writeMapToCLI(int map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j]) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}
