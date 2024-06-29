#include "iostream"
#include "image_loader_jpg.h"

//----------------------------------------------------------------------------------------------------------------------
image_loader_jpg::image_loader_jpg(const char *FileName) : image_loader(FileName) {
    raw_image = nullptr;
}
//----------------------------------------------------------------------------------------------------------------------
void image_loader_jpg::ReadFile() {
    //same logic as in image_loader_png

    // Create decompress struct and error manager
    struct jpeg_decompress_struct cinfo1;
    struct jpeg_error_mgr jerr;

    cinfo1.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo1);

    FILE* infile;
    if ((infile = fopen(this->imgFile, "rb")) == NULL) {
        throw std::runtime_error("Can't open " + std::string(this->imgFile));
    }
    jpeg_stdio_src(&cinfo1, infile);

    if (jpeg_read_header(&cinfo1, true) != JPEG_HEADER_OK) {
        fclose(infile);
        jpeg_destroy_decompress(&cinfo1);
        throw std::runtime_error("Invalid JPEG header");
    }

    if (jpeg_start_decompress(&cinfo1) != TRUE) {
        fclose(infile);
        jpeg_destroy_decompress(&cinfo1);
        throw std::runtime_error("Failed to start JPEG decompression");
    }

    // Retrieve image properties
    this->width = cinfo1.output_width;
    this->height = cinfo1.output_height;
    this->bytes_per_pixel = cinfo1.num_components;
    this->cinfo = cinfo1;

    // Calculate target number of rows based on desired aspect ratio
    this->InRows = this->height;
    this->InCols = this->width;
    this->nRows = this->nCols * ((double)InCols / InRows) * 0.23;

    // Allocate memory for raw image dat
    raw_image = new BYTE[cinfo.output_width * cinfo.output_height * cinfo.num_components];
    BYTE* row_pointer[1];

    // Read scanlines of the image
    while (cinfo.output_scanline < cinfo.image_height) {
        row_pointer[0] = &raw_image[cinfo.output_scanline * cinfo.output_width * cinfo.num_components];
        if (jpeg_read_scanlines(&cinfo, row_pointer, 1) != 1) {
            fclose(infile);
            jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            throw std::runtime_error("Failed to read JPEG scanlines");
        }
    }

    // Finish decompression and cleanup
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

//----------------------------------------------------------------------------------------------------------------------
void image_loader_jpg::ToGray() {
    // Allocate memory for the grayscale image matrix
    InMatrix = new BYTE* [InRows];
    for (int i = 0; i < InRows; i++) {
        InMatrix[i] = new BYTE[InCols];
    }


    if (cinfo.num_components == 1) {
        // The image is already in gray

        for (int i = 0; i < InRows; i++) {

            for (int j = 0; j < InCols; j++) {
                InMatrix[i][j] = raw_image[i * InCols + j];
            }
        }
        return;
    }

    // Convert RGB image to grayscale
    for (int i = 0; i < InRows; i++) {

        for (int j = 0; j < InCols; j++) {
            int index = i * InCols * cinfo.num_components + j * cinfo.num_components;
            int red = raw_image[index + 0];
            int green = raw_image[index + 1];
            int blue = raw_image[index + 2];

            // Convert RGB values to grayscale using the magic formula:
            double gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
            InMatrix[i][j] = gray + 0.5;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
image_loader_jpg::~image_loader_jpg() {
    if(raw_image != nullptr) delete[] raw_image;
}
//----------------------------------------------------------------------------------------------------------------------
