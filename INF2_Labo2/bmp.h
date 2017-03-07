/* 
 * Author: Romain Maffina
 * Date: 11.02.2016
 * 
 * BMP image read/write
 */

//HIDE 

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define CLR_DEPTH 8
#define CLR_USED ( 1 << (CLR_DEPTH)) /* 256 */

// tell the compiler to *not* do any memory alignment
#pragma pack(push, 1)

struct bmp_file_header_t {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct bmp_info_header_t
{
    uint32_t biSize;            // size of the structure
    int32_t biWidth;            // image width
    int32_t biHeight;           // image height
    uint16_t biPlanes;          // bitplanes
    uint16_t biBitCount;        // resolution 
    uint32_t biCompression;     // compression
    uint32_t biSizeImage;       // size of the image
    int32_t biXPelsPerMeter;   // pixels per meter X
    int32_t biYPelsPerMeter;   // pixels per meter Y
    uint32_t biClrUsed;         // colors used
    uint32_t biClrImportant;    // important colors
};

struct bmp_color_index_t{
    uint8_t r,g,b,pad;
};

// back to normal packing
#pragma pack(pop)

int bmp_read(const std::string path, 
             std::vector<uint8_t> &pixels, uint32_t &w, uint32_t &h);
int bmp_write(const std::string path, 
              const std::vector<uint8_t> &pixels, uint32_t w, uint32_t h);


