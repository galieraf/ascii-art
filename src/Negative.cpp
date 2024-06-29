#include "Negative.h"
#define maxCH 255

//----------------------------------------------------------------------------------------------------------------------
void Negative::applyFilter(IMAGE *image) {
    for (int i = 0; i < image->InRows; i++) {
        for (int j = 0; j < image->InCols; j++) {
            image->imgLoad->InMatrix[i][j] = maxCH  - image->imgLoad->InMatrix[i][j];
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------