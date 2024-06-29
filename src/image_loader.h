#ifndef GALIERAF_IMAGE_LOADER_H
#define GALIERAF_IMAGE_LOADER_H

#define BYTE unsigned char
//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Class for loading and processing image files.
 */
class image_loader{
public:
    /**
     * @brief Constructor for the image_loader class.
     * @param FileName Path to the image file.
     */
    image_loader(const char *FileName);

    /**
     * @brief Destructor for the image_loader class.
     * Cleans up dynamically allocated memory.
     */
    virtual ~image_loader();

    /**
     * @brief Read the image file.
     * This is a virtual function that needs to be implemented by derived classes.
     */
    virtual void ReadFile() = 0;

    /**
     * @brief Convert the image to grayscale.
     * This is a virtual function that needs to be implemented by derived classes.
     */
    virtual void ToGray() = 0;

    /**
     * @brief Defines the type of the image file.
     * This method determines the type of the image file based on its header.
     * @return The type of the image file: IS_PNG, IS_JPG, or IS_OTHER.
     *
     * @throws runtime_error If imgFile could not be opened for reading
     */
    int DefineType();

    const char* imgFile;    ///< Path to the image file.
    int imageType;          ///< Type of the image file (PNG, JPG, or other).
    int InRows, InCols;     ///< Number of rows and columns in the image.
    int nCols, nRows;       ///< Number of columns and rows after processing.
    BYTE** InMatrix;        ///< Matrix containing the pixel values of the image.
};
//----------------------------------------------------------------------------------------------------------------------

#endif //GALIERAF_IMAGE_LOADER_H
