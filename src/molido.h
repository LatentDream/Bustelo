// molido.h
#ifndef MOLIDO_H
#define MOLIDO_H
#define MAP_SIZE 256

typedef int MapType[MAP_SIZE][MAP_SIZE];
typedef int Map3DType[MAP_SIZE][MAP_SIZE][MAP_SIZE];

void parseArgs(int argc, char** argv, char** targetFile, int* isLogScale, int* parseArgs);
void writeMapToImage(const char* filename, MapType* normalizedMap);

void fillMap(char* targetFile, MapType* map);
void fillMap3D(char* targetFile, Map3DType* map);
void copyMap(MapType* orig, MapType* dest);
void copyMap3D(Map3DType* orig, Map3DType* dest);
void normaliseMap(MapType* map, int isLogScale);
void normaliseMap3D(Map3DType* map, int isLogScale);

#endif // MOLIDO_H

