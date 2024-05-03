
// molido.h
#ifndef MOLIDO_H
#define MOLIDO_H
#define MAP_SIZE 256

void writeMapToImage(const char* filename, int map[MAP_SIZE][MAP_SIZE]);
const char* getExtension(const char* filename);

#endif // MOLIDO_H

