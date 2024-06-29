#include "image_loader.h"
#include "png.h"
#include "pngconf.h"
#ifndef GALIERAF_IMAGE_LOADER_PNG_H
#define GALIERAF_IMAGE_LOADER_PNG_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Class for loading PNG images and processing them.
 * @inherit{image_loader}
 */
class image_loader_png : public image_loader {
public:
    /**
     * @brief Constructs an image_loader_png object with the specified file name.
     * @param FileName The name of the PNG file to load.
     */
    image_loader_png(const char *FileName);

    /**
     * @brief Reads the PNG file and extracts necessary information.
     */
    void ReadFile() override;

    /**
     * @brief Converts the PNG image to grayscale.
     *
     * @throws std::runtime_error If the file cannot be opened for reading, or if an error occurs during PNG processing.
     */
    void ToGray() override;

    /**
     * @brief Destroys the image_loader_png object and frees allocated memory.
     */
    ~image_loader_png() override;
private:
    int width;                  ///< The width of the PNG image.
    int height;                 ///< The height of the PNG image.
    int bytes_per_pixel;        ///< The number of bytes per pixel.
    png_byte color_type;        ///< The color type of the PNG image.
    png_byte bit_depth;         ///< The bit depth of the PNG image.
    float aspect_ratio;         ///< The aspect ratio of the PNG image.

    png_structp png_ptr;        ///< The PNG structure pointer.
    png_infop info_ptr;         ///< The PNG info structure pointer.
    int number_of_passes;       ///< The number of passes for interlaced PNG images.
    png_bytep* row_pointers;    ///< The row pointers to the image data.
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_IMAGE_LOADER_PNG_H
