// molido.h
#ifndef MOLIDO_H
#define MOLIDO_H
#define MAP_SIZE 256

typedef int MapType[MAP_SIZE][MAP_SIZE];

void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale, int* parseArgs);
void writeMapToImage(const char* filename, MapType* normalizedMap);
void fillMap(char* targetFile, MapType* map);
void normaliseMap(MapType* map, int isLogScale);

#endif // MOLIDO_H

