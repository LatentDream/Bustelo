// molido.c
#include "molido.h"
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAP_SIZE 256

void molido_function() {
    printf("[INFO] Hello from molido.c\n");
}

void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE]) {
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
    free(pixels);
}


