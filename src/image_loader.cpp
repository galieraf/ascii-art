#include <iostream>
#include "cstring"
#include "image_loader.h"
#define IS_PNG 1
#define IS_JPG 2
#define IS_OTHER -1

//----------------------------------------------------------------------------------------------------------------------
image_loader::image_loader(const char* FileName) {
    this->imgFile = FileName;
    InMatrix = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
image_loader::~image_loader() {
    if (this->InMatrix == nullptr) return;
    for (int i = 0; i < InRows; i++) {
        delete[] InMatrix[i];
    }
    delete[] InMatrix;
}

//----------------------------------------------------------------------------------------------------------------------
int image_loader::DefineType() {
    int type;
    FILE* fp = fopen(imgFile, "rb");
    if (!fp) {
        throw std::runtime_error("File " + std::string(imgFile) + " could not be opened for reading");
    }
    unsigned char h[12];
    // if image file begins with these bytes, it is PNG file
    unsigned char pngHeader[] = { 137, 80, 78, 71, 13, 10, 26, 10 }; // look https://www.w3.org/TR/PNG-Structure.html
    fread(h, 1, 12, fp);
    fclose(fp);

    if (memcmp(h, pngHeader, 8) == 0) { // PNG
        type = IS_PNG;
    }
    else {
        if (memcmp(h + 6, "JFIF", 4) == 0) { // JPG
            type = IS_JPG;
        }
        else {
            type = IS_OTHER;
        }
    }
    return type;
}
//----------------------------------------------------------------------------------------------------------------------