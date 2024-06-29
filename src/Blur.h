
#include "Operation.h"
#ifndef GALIERAF_BLUR_H
#define GALIERAF_BLUR_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Blur class represents a blur filter operation.
 * It applies a blurring effect to the image.
 * @inherit{Operation}
 */
class Blur : public Operation{
public:
    /**
     * @brief Applies the blur filter to the given image.
     * @param image The image to apply the blur filter to.
     * @override
     */
    void applyFilter(IMAGE * image) override;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_BLUR_H
