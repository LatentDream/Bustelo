// molido.h
#ifndef MOLIDO_H
#define MOLIDO_H
#define MAP_SIZE 256

typedef int MapType[MAP_SIZE][MAP_SIZE];

void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale, int* parseArgs);
void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE], int isLogScale);
void fillMap(char* targetFile, MapType* map);

#endif // MOLIDO_H

