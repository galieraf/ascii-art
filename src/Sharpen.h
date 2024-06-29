#include "Operation.h"
#ifndef GALIERAF_SHARPEN_H
#define GALIERAF_SHARPEN_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Sharpen class represents a sharpen filter operation.
 * It enhances the edges and details of the image.
 * @inherit{Operation}
 */
class Sharpen : public Operation{
public:
    /**
     * @brief Applies the sharpen filter to the given image.
     * @param image The image to apply the sharpen filter to.
     * @override
     */
    void applyFilter(IMAGE * image) override;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_SHARPEN_H
