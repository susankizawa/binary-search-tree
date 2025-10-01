#include "file_system.h"

#include <stdio.h>
#include <stdlib.h>

File* createFile(std::string fileName, int fileSize){
    if(fileName == ""){
        printf("error: fileName missing\n");
        return NULL;
    }

    if(fileSize < 0){
        printf("error: size needs to be a positive number\n");
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

        newFile->size = static_cast<size_t>(fileSize);
    }
    return newFile;
}

void printFile(File* f){
    if(f == NULL){
        printf("[ NULL ]\n");
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
    printf("%zu", f->size);
    printf("\n");
    printf("]\n");

    return;
}
