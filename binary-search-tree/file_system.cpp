#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>

File* createFile(std::string fileName, size_t size){
    if(fileName == ""){
        printf("error: fileName missing\n");
        return NULL;
    }

    File* newFile = new File;
    if (newFile) {
        size_t dotPos = fileName.find(".");

        if(dotPos != std::string::npos){
            newFile->name = fileName.substr(0, dotPos);
            newFile->extension = fileName.substr(dotPos + 1, std::string::npos);
        } else {
            newFile->name = fileName;
            newFile->extension = "";
        }

        newFile->size = size;
    }
    return newFile;
}

void printFile(File* f){
    if(f == NULL){
        printf("[ NULL ]");
        return;
    }

    printf("[\n");
    printf(" name: ");
    printf("%s", f->name.c_str());
    printf("\n");
    if(f->extension != ""){
        printf(" extension: ");
        printf("%s", f->extension.c_str());
        printf("\n");
    }
    printf(" size: ");
    printf("%d", f->size);
    printf("\n");
    printf("]\n");

    return;
}
