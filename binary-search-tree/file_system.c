#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

typedef struct File{
    int size;
    std::string name;
    std::string extension;
} File;

File* createFile(std::string fileName, int size);

#endif // FILE_SYSTEM_H
