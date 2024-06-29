#include "Blur.h"

//----------------------------------------------------------------------------------------------------------------------
void Blur::applyFilter(IMAGE* image) {
    // Create a temporary matrix to store the image pixels
    BYTE** temp = image->CreateTempMatrix(image->InRows, image->InCols, image->imgLoad->InMatrix);

    // Define the blur kernel (3x3 matrix)
    static float kernel[3][3] = {{1, 1, 1},
                          {1, 1, 1},
                          {1, 1, 1}};

    // Apply the blur filter to each pixel in the image (except the border pixels)
    for (int i = 1; i < image->InRows - 1; i++) {
        for (int j = 1; j < image->InCols - 1; j++) {
            float sum = 0.0f;
            // Convolution: Multiply each pixel value with the corresponding kernel value and sum them up
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    // Multiply the pixel value with the kernel value and accumulate the sum
                    sum += static_cast<float>(temp[i + k][j + l]) * kernel[k + 1][l + 1];
                }
            }
            // Normalize the sum by dividing it by 9.0 (the sum of all kernel elements)
            int val = static_cast<int>(sum / 9.0f);

            // Clamp the value within the valid range of 0 to 255
            if (val < 0) {
                val = 0;
            } else if (val > 255) {
                val = 255;
            }

            // Update the pixel value in the image with the blurred value
            image->imgLoad->InMatrix[i][j] = static_cast<BYTE>(val);
        }
    }

    // Free the memory allocated for the temporary matrix
    image->FreeTempMatrix(image->InRows, temp);
}
//----------------------------------------------------------------------------------------------------------------------
