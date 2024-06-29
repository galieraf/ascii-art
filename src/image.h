#define BYTE unsigned char
#include "image_loader.h"
#include "map"
#include "vector"
#include "string"
#ifndef IMAGE_H
#define IMAGE_H

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Class for working with images and creating ASCII art images.
 */
class IMAGE {
public:
    const char* imgFile;            ///< Path to the image file (PNG or JPG).
    const char* artFile;            ///< Path to the result file with the art image.
    int InRows;                     ///< Number of rows in the original image.
    int InCols;                     ///< Number of columns in the original image.
    int nCols;                      ///< Number of symbols in one line of the art image (same as the number of columns).
    int nRows;                      ///< Number of lines in the art matrix.
    BYTE** GrayMatrix = nullptr;    ///< Grayscale matrix of size nRows x nCols.
    char** ArtMatrix = nullptr;     ///< ASCII art matrix.
    char* art;                      ///< Symbol array for the art image.
    image_loader* imgLoad;          ///< Help class object for reading data from different image types
    std::map<int, image_loader*> imageLoaders; ///< Map with imgLoad for different image types
    /**
     * @brief Constructor for the IMAGE class.
     * @param in_file Path to the input image file.
     * @param art_file Path to the output art file.
     * @param nCols Number of symbols in one line of the art image.
     * @param art Symbol array for the art image.
     * @param Op Vector of operation IDs for image processing.
     *
     * @throws invalid_argument If the image file format is unknown.
     */
    IMAGE(const char* in_file, const char* art_file, int nCols, char* art, std::vector<int> Op );

    /**
     * @brief Destructor for the IMAGE class.
     * Cleans up dynamically allocated memory.
     */
    virtual ~IMAGE();

    /**
     * @brief Convert the grayscale image to ASCII art.
     */
    void GrayToArt();

    /**
     * @brief Write the ASCII art matrix to the art file.
     * This function writes the content of the ASCII art matrix to the specified art file.
     * If the file already exists, its content will be overwritten. If the file does not exist,
     * a new file will be created. If an error occurs during the file operation, an exception will be thrown.
     * @throws std::runtime_error if the file cannot be opened or if an error occurs during the file operation.
     */
    void WriteArtFile();

    /**
     * @brief Create a temporary matrix.
     * @param rows Number of rows in the matrix.
     * @param cols Number of columns in the matrix.
     * @param matrix Pointer to the original matrix.
     * @return Pointer to the newly created matrix.
     */
    BYTE ** CreateTempMatrix(int rows, int cols, unsigned char **matrix);

    /**
     * @brief Free the memory allocated for a temporary matrix.
     * @param rows Number of rows in the matrix.
     * @param matrix Pointer to the matrix to be freed.
     */
    void FreeTempMatrix(int rows, unsigned char **matrix);

private:
    /**
     * @brief Calculate the window dimensions for averaging grayscale values.
     */
    void CalcWnd();

    /**
     * @brief Allocate memory for the grayscale and ASCII art matrices.
     */
    void getMemory();

    int wnd_width;                  ///< Window width for averaging grayscale values.
    int wnd_height;                 ///< Window height for averaging grayscale values.
    std::vector<int> operations;    ///< Vector of operation IDs for image processing.
};
//----------------------------------------------------------------------------------------------------------------------

#endif // IMAGE_H