#include "Operation.h"

#ifndef GALIERAF_NEGATIVE_H
#define GALIERAF_NEGATIVE_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Negative class represents a negative filter operation.
 * It inverts the colors of the image.
 * @inherit{Operation}
 */
class Negative : public Operation{
public:
    /**
     * @brief Applies the negative filter to the given image.
     * @param image The image to apply the negative filter to.
     * @override
     */
    void applyFilter(IMAGE *image) override;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_NEGATIVE_H
