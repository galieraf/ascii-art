#include "Operation.h"
#ifndef GALIERAF_EDGEDETECTION_H
#define GALIERAF_EDGEDETECTION_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The EdgeDetection class represents an edge detection operation.
 * It applies the edge detection filter to an image.
 * The edge detection filter enhances the edges and boundaries of objects in an image.
 * It is based on convolution with a predefined kernel to highlight areas of rapid intensity transition.
 * @inherit{Operation}
 */
class EdgeDetection : public Operation {

    /**
    * @brief Applies the edge detection filter to the given image.
    * @param image The image to apply the edge detection filter to.
    */
    void applyFilter(IMAGE * image) override;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_EDGEDETECTION_H
