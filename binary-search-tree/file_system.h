#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

typedef struct File{
    std::string name;
    std::string extension;
    size_t size;
} File;

File* createFile(std::string fileName, int size);

void printFile(File* f);

#endif // FILE_SYSTEM_H
