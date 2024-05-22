#include <stdio.h>
#include <stdlib.h>
#include "molido.h"
#include "gui.h"
#include "cli.h"

// Init the Map
static int map[MAP_SIZE][MAP_SIZE] = {0};

int main(int argc, char** argv) {

    int startGui = 0;
    int isLogScale = 0;
    char* targetFile;
    parseArgs(argc, argv, &targetFile, &isLogScale, &startGui);

    if (startGui) {
        return launchUIEventLoop();
    } else {
        return launchCLI(targetFile, isLogScale);
    }
}

