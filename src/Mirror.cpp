#include "Mirror.h"

//----------------------------------------------------------------------------------------------------------------------
void Mirror::applyFilter(IMAGE *image) {
    for (int i = 0; i < image->InRows; i++) {
        for (int j = 0; j <= image->InCols/2; j++) {
            unsigned char temp = image->imgLoad->InMatrix[i][j];
            image->imgLoad->InMatrix[i][j] = image->imgLoad->InMatrix[i][image->InCols - 1 - j];
            image->imgLoad->InMatrix[i][image->InCols - 1 - j] = temp;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
