#include "image.h"
#ifndef GALIERAF_OPERATION_H
#define GALIERAF_OPERATION_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Operation class represents a generic image operation.
 */
class Operation {
public:
    /**
     * @brief Applies the filter to the given image.
     * @param image The image to apply the filter to.
     */
    virtual void applyFilter(IMAGE * image) = 0;
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_OPERATION_H
