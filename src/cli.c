#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "molido.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#define MAP_SIZE 256

// Init the Map
static MapType map = {0};

int launchCLI(char* targetFile, int isLogScale) {

    fillMap(targetFile, &map);
    normaliseMap(&map, isLogScale);

    const char* base = "%s.bustelo.png";
    int len = snprintf(NULL, 0, base, targetFile);
    char* output = (char*)malloc(len + 1);
    snprintf(output, len + 1, base, targetFile);

    writeMapToImage(output, &map);

    // Clean up
    free(output);
    printf("\n");

    return 0;
}

