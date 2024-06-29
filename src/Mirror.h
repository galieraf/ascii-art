#include "Operation.h"
#ifndef GALIERAF_MIRROR_H
#define GALIERAF_MIRROR_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Mirror class represents a mirror filter operation.
 * It horizontally flips the image.
 * @inherit{Operation}
 */
class Mirror : public Operation{
public:
    /**
     * @brief Applies the mirror filter to the given image.
     * @param image The image to apply the mirror filter to.
     * @override
     */
    void applyFilter(IMAGE * image) override;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_MIRROR_H
