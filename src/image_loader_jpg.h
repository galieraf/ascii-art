#include "image_loader.h"
#include "jpeglib.h"
#ifndef GALIERAF_IMAGE_LOADER_JPG_H
#define GALIERAF_IMAGE_LOADER_JPG_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Class for loading JPG images and processing them.
 * @inherit{image_loader}
 */
class image_loader_jpg : public image_loader{
public:
    /**
     * @brief Constructs an image_loader_jpg object with the specified file name.
     * @param FileName The name of the JPG file to load.
     */
    image_loader_jpg(const char *FileName);

    /**
     * @brief Reads the JPG file and extracts necessary information.
     */
    void ReadFile() override;

    /**
     * @brief Converts the JPG image to grayscale.
     *
     * @throws std::runtime_error If the file cannot be opened for reading, if the JPEG header is invalid,
 *                            or if there is an error during JPEG decompression or scanline reading
     */
    void ToGray() override;

    /**
     * @brief Destroys the image_loader_jpg object and frees allocated memory.
     */
    ~image_loader_jpg() override;

private:
    int width;                              ///< The width of the JPG image.
    int height;                             ///< The height of the JPG image.
    int bytes_per_pixel;                    ///< The number of bytes per pixel.
    BYTE* raw_image;                        ///< The raw image data.
    struct jpeg_decompress_struct cinfo;    ///< The JPEG decompression structure.
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_IMAGE_LOADER_JPG_H
