#include "image_loader_png.h"
#include "stdlib.h"
#include "iostream"

// color indexes within pixel
#define RED 0
#define GREEN 1
#define BLUE 2
//----------------------------------------------------------------------------------------------------------------------
image_loader_png::image_loader_png(const char *FileName) : image_loader(FileName) {

    png_ptr = nullptr;
    info_ptr = nullptr;
    row_pointers = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
void image_loader_png::ReadFile() {
    // inspired by http://zarb.org/~gc/html/libpng.html
    // http://www.libpng.org/pub/png/libpng-1.2.5-manual.html

    png_byte header[8];    // 8 is the maximum size that can be checked


    /* open file and test for it being a png */
    FILE* fp = fopen(this->imgFile, "rb");
    if (!fp)
        throw std::runtime_error("File " + std::string(imgFile) + " could not be opened for reading");
    fread(header, 1, 8, fp);

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        throw std::runtime_error("png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        throw std::runtime_error("png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        throw std::runtime_error("Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        throw std::runtime_error("Error during read_image");

    row_pointers = new png_bytep[sizeof(png_bytep) * height];
    int n = png_get_rowbytes(png_ptr, info_ptr);
    bytes_per_pixel = n / width;
    for (int y = 0; y < height; y++) {
        row_pointers[y] = new png_byte[n];
    }
    this->InRows = this->height;
    this->InCols = this->width;
    this->nRows = this->nCols * ((double)InCols / InRows) * 0.23;

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

}

//----------------------------------------------------------------------------------------------------------------------
void image_loader_png::ToGray() { // only for PNG_COLOR_TYPE_RGB
    // convert COLOR to GRAY
    if (this->color_type == PNG_COLOR_TYPE_GRAY) {
        InMatrix = this->row_pointers;
        return;
    }
    png_byte* Row, * pxl;
    double gray;
    InMatrix = new BYTE* [InRows];
    for (int i = 0; i < InRows; i++) {
        InMatrix[i] = new BYTE[InCols];
    }

    for (int i = 0; i < this->height; i++) {
        Row = row_pointers[i];
        for (int j = 0, k = 0; j < this->width; j++, k += bytes_per_pixel) {
            pxl = Row + k;
            //https://stackoverflow.com/questions/17615963/standard-rgb-to-grayscale-conversion
            gray = 0.2126 * pxl[RED] + 0.7152 * pxl[GREEN] + 0.0722 * pxl[BLUE];

            // gamma correction
            // gray = gray <= 0.0031308 ? 12.92 * gray : 1.055 * pow(gray, 0.416666667);
            // it turns out nonsense
            // InMatrix[i][j] = gray - 0.055;
            InMatrix[i][j] = gray + 0.5;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------
image_loader_png::~image_loader_png() {

    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

    // free memory allocated for row_pointers
    if (row_pointers != nullptr) {
        for (int i = 0; i < height; i++) {
            delete[] row_pointers[i];
        }

    delete[] row_pointers;
    }
}
//----------------------------------------------------------------------------------------------------------------------