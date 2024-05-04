
// molido.h
#ifndef MOLIDO_H
#define MOLIDO_H
#define MAP_SIZE 256

void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale, int* parseArgs);
void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE], int isLogScale);

#endif // MOLIDO_H

