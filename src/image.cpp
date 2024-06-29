#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "image.h"
#include "image_loader.h"
#include "image_loader_png.h"
#include "image_loader_jpg.h"
#include "Operation.h"
#include "Negative.h"
#include "Mirror.h"
#include "Sharpen.h"
#include "Blur.h"
#include "EdgeDetection.h"
#include "map"
#include <unistd.h>

#define IS_PNG 1
#define IS_JPG 2
#define IS_OTHER 3
using namespace std;
//----------------------------------------------------------------------------------------------------------------------
EdgeDetection f1;
Mirror f2;
Sharpen f3;
Negative f4;
Blur f5;

static map<int, Operation*> operation= {
        {1, &f1},
        {2, &f2},
        {3, &f3},
        {4, &f4},
        {5, &f5}
};

//----------------------------------------------------------------------------------------------------------------------
IMAGE::IMAGE(const char* imgFile,const char* artFile, int ncol, char* art, vector<int> Op) {
    this->imgFile = imgFile;
    this->artFile = artFile;
    this->nCols = ncol;
    this->art = art;
    this->operations = Op;
    image_loader * imgLoad1 = new image_loader_png(imgFile);
    image_loader * imgLoad2 = new image_loader_jpg(imgFile);
    imageLoaders.insert(make_pair(IS_PNG, imgLoad1));
    imageLoaders.insert(make_pair(IS_JPG, imgLoad2));
    int type = imageLoaders[IS_PNG]->DefineType();

    if (imageLoaders.find(type) == imageLoaders.end()) {
        throw invalid_argument("Unknown file format.\n");
    }

    imgLoad = imageLoaders[type];
    imgLoad->nCols = this->nCols;
    imgLoad->ReadFile();
    this->InCols = imgLoad->InCols;
    this->InRows = imgLoad->InRows;
    this->nCols = imgLoad->nCols;
    this->nRows = imgLoad->nRows;
    this->CalcWnd();
    this->getMemory();
    imgLoad->ToGray();
    this->GrayToArt();
    this->WriteArtFile();
}

//----------------------------------------------------------------------------------------------------------------------
IMAGE::~IMAGE() {
    if (GrayMatrix) {
        for (int i = 0; i < nRows; i++) {
            delete[] GrayMatrix[i];
        }
        delete[] this->GrayMatrix;
    }

    if (ArtMatrix) {
        for (int i = 0; i < nRows; i++) {
            delete[] ArtMatrix[i];
        }
        delete[] this->ArtMatrix;
    }

    //delete imgLoad;
    for (auto& pair : imageLoaders) {
        delete pair.second;
    }
    imageLoaders.clear();

}

//----------------------------------------------------------------------------------------------------------------------
void IMAGE::getMemory() {

    this->GrayMatrix = new BYTE * [this->nRows]; // symbol number in art sequence
    for (int i = 0; i < nRows; i++) {
        GrayMatrix[i] = new BYTE[nCols];
    }

    this->ArtMatrix = new char* [nRows];
    for (int i = 0; i < nRows; i++) {
        ArtMatrix[i] = new char[nCols];
    }
}

//----------------------------------------------------------------------------------------------------------------------
void IMAGE::GrayToArt() {

    for (int filter : operations) {
        operation[filter]->applyFilter(this);
    }


    int L_art = strlen(art);
    float nInterval = 256. / L_art;

    int up, left, s, n, i1, j1, k;
    for (int i = 0; i < nRows; i++) {
        up = i * wnd_height;
        for (int j = 0; j < nCols; j++) {
            left = j * wnd_width;
            s = 0;
            n = 0;

            // working with scanning window wnd_height*wnd_width
            for (i1 = up; i1 < up + wnd_height && i1 < InRows; i1++) {
                for (j1 = left; j1 < left + wnd_width && j1 < InCols; j1++) {
                    s += imgLoad->InMatrix[i1][j1];
                    n++;
                }
            }
            s = s / n;
            GrayMatrix[i][j] = s;
            k = s / nInterval;
            char c = art[k];
            ArtMatrix[i][j] = c;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void IMAGE::WriteArtFile() {
    FILE* f = fopen(this->artFile, "w");
    if (f == NULL) {
        throw std::runtime_error("Failed to open file for writing: " + std::string(this->artFile));
    }

    // Clear the file by truncating its content
    if (truncate(this->artFile, 0) != 0) {
        throw std::runtime_error("Failed to clear file: " + std::string(this->artFile));
    }

    // Write the ASCII art to the file
    for (int i = 0; i < this->nRows; i++) {
        for (int j = 0; j < this->nCols; j++) {
            fprintf(f, "%c", ArtMatrix[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}


//----------------------------------------------------------------------------------------------------------------------
void IMAGE::CalcWnd() {
    // edge windows may be not full
    wnd_width = InCols / nCols + (InCols % nCols == 0 ? 0 : 1);
    nCols = InCols / wnd_width;
    wnd_height = InRows / nRows + (InRows % nRows == 0 ? 0 : 1);
    nRows = InRows / wnd_height;
}

//----------------------------------------------------------------------------------------------------------------------
BYTE **IMAGE::CreateTempMatrix(int rows, int cols, unsigned char **matrix) {

    BYTE** temp = new BYTE*[rows];
    for (int i = 0; i < rows; i++) {
        temp[i] = new BYTE[cols];
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[i][j] = matrix[i][j];
        }
    }
    return temp;
}


//----------------------------------------------------------------------------------------------------------------------
void IMAGE::FreeTempMatrix(int rows, unsigned char **matrix) {

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
//----------------------------------------------------------------------------------------------------------------------
