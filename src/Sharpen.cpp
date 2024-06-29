#include "Sharpen.h"

//----------------------------------------------------------------------------------------------------------------------
void Sharpen::applyFilter(IMAGE *image) {
    // Create a temporary matrix for calculations based on the input image
    BYTE** temp = image->CreateTempMatrix(image->InRows, image->InCols, image->imgLoad->InMatrix);

    // Define the sharpening kernel
    static float kernel[3][3] = {{-1, -1, -1},
                          {-1,  8, -1},
                          {-1, -1, -1}};

    // Apply the sharpening filter to the image
    for (int i = 1; i < image->InRows - 1; i++) {
        for (int j = 1; j < image->InCols - 1; j++) {
            float sum = 0.0f;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    // Convolution: Multiply the corresponding elements of the kernel and the neighboring pixels,
                    // and sum them up
                    sum += static_cast<float>(temp[i + k][j + l]) * kernel[k + 1][l + 1];
                }
            }
            // Add the sum to the original pixel value
            int val = static_cast<int>(image->imgLoad->InMatrix[i][j] + sum);

            // Clamp the value within the valid range of [0, 255]
            if (val < 0) {
                val = 0;
            } else if (val > 255) {
                val = 255;
            }
            // Update the pixel value
            image->imgLoad->InMatrix[i][j] = static_cast<BYTE>(val);
        }
    }

    // Free the temporary matrix memory
    image->FreeTempMatrix(image->InRows, temp);
}
//----------------------------------------------------------------------------------------------------------------------
