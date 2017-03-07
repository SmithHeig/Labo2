/* 
 * Author: Romain Maffina
 * Date: 11.02.2016
 * 
 * BMP image read/write
 */
 
//HIDE

#include "bmp.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

bool is_integer(float k);
void bmp_print_header(struct bmp_file_header_t &h);
void bmp_print_info(struct bmp_info_header_t &info);

int bmp_read(const string path, 
             vector<uint8_t> &pixels, uint32_t& w, uint32_t& h) {
    
    struct bmp_file_header_t header;
    struct bmp_info_header_t info;
    vector<struct bmp_color_index_t> palette(CLR_USED);
    uint32_t pad_size;
    
    // open image
    ifstream img(path.c_str(), ios::in | ios::binary);
    if (!img)
        return false;
    
    // read the bmp file header
    img.read((char*)&header, sizeof(header));
    if (!img)
        return false;
    
    // check standard BITMAPFILEHEADER
    if (header.bfType != 0x4D42)
        return false;

#ifdef DEBUG    
    bmp_print_header(header);
#endif
    
    // read the bmp info header
    img.read((char*)&info, sizeof(info));
    if (!img)
        return false;
    
    // check standard BITMAPINFOHEADER
    if (info.biSize != 0x28) 
        return false;

#ifdef DEBUG    
    bmp_print_info(info);
#endif

    // we don't support non-classic top to bottom pixels array
    if (info.biWidth < 0 || info.biHeight < 0)
        return false;

    // check if we support this color depth
    if (info.biBitCount != CLR_DEPTH)
        return false;
    
    // WARN: using less than the full palette, could lead to unexpected results
    if (info.biClrUsed != 0 && info.biClrUsed != CLR_USED)
        palette.resize(info.biClrUsed);

    // read color table: RGB 0-255 shadings + 1 padding byte (biClrUsed*4B)
    img.read((char*)palette.data(), 
        (streamsize)(info.biClrUsed*sizeof(struct bmp_color_index_t)) );
    
    // move to start of pixel array
    img.seekg(header.bfOffBits);

    // read pixels array
    h = (uint32_t)abs(info.biHeight);
    w = (uint32_t)abs(info.biWidth);
    pad_size = (4 - (w % 4)) % 4;

    pixels.resize(w*h);
    for (size_t i=0; i<h; i++) {
        img.read((char*)(pixels.data()+(h-1-i)*w), (streamsize)w);
        img.ignore(pad_size);
        if (!img)
            return false;
    }
    
    img.close();
    
    return true;
}

int bmp_write(const string path, 
              const vector<uint8_t> &pixels, uint32_t w, uint32_t h) {

    uint32_t pad_size = (4 - (w % 4)) % 4;
    uint32_t w_padded = w+pad_size;
    uint8_t pad = 0;
    uint32_t offset =   sizeof(struct bmp_file_header_t) +
                        sizeof(struct bmp_info_header_t) +
                        CLR_USED*sizeof(struct bmp_color_index_t);

    struct bmp_file_header_t header = {
        /*.bfType =*/ 0x4D42,
        /*.bfSize =*/ w_padded*h + offset,
        /*.bfReserved1 =*/ 0,
        /*.bfReserved2 =*/ 0,
        /*.bfOffBits =*/ offset,
    };
    
    struct bmp_info_header_t info = {
        /*.biSize =*/ sizeof(struct bmp_info_header_t),
        /*.biWidth =*/ (int32_t)w,
        /*.biHeight =*/ (int32_t)h,
        /*.biPlanes =*/ 1,
        /*.biBitCount =*/ CLR_DEPTH,
        /*.biCompression =*/ 0,
        /*.biSizeImage =*/ w_padded*h,
        /*.biXPelsPerMeter =*/ 0,
        /*.biYPelsPerMeter =*/ 0,
        /*.biClrUsed =*/ CLR_USED,
        /*.biClrImportant =*/ 0,
    };
    
    struct bmp_color_index_t color;
    
    // open output image
    ofstream img(path.c_str(), ios::out | ios::binary);
    if (!img)
        return false;
    
#ifdef DEBUG
    // show headers
    bmp_print_header(header);
    bmp_print_info(info);
#endif
        
    // write headers
    img.write((char*)&header, sizeof(header));
    img.write((char*)&info, sizeof(info));

    // write color map
    for (uint32_t i=0; i<info.biClrUsed; i++) {
        color.r = i; color.g = i; color.b = i; color.pad = 0;
        img.write((char*)&color, sizeof(color));
    }

    // write pixels array
    img.seekp(header.bfOffBits);

    for (uint32_t i=0; i<h; i++) {
        img.write((const char*)(pixels.data()+(h-1-i)*w), (streamsize)w);
        img.write((char*)&pad, pad_size);
    }
    
    img.close();
    
    return true;
}

bool is_integer(float k)
{
    return k == (float)(int)k;
}

void bmp_print_header(struct bmp_file_header_t &h) {

    cout << "BMP file header (" << dec << sizeof(h) << "B):" << endl;
    cout << "- Magic: 0x" << hex << h.bfType << dec << endl;
    cout << "- Size: " << h.bfSize << endl;
    cout << "- Reserved: " << h.bfReserved1 << h.bfReserved2 << endl;
    cout << "- Offset: " << h.bfOffBits << endl;
}

void bmp_print_info(struct bmp_info_header_t &info) {

    cout << "BMP info header (" << dec << sizeof(info) << "B):" << endl;
    cout << "- Header size: " << info.biSize << endl;
    cout << "- Image width: " << info.biWidth << endl;
    cout << "- Image height: " << info.biHeight << endl;
    cout << "- Color planes: " << info.biPlanes << endl;
    cout << "- Color depth: " << info.biBitCount << endl;
    cout << "- Compression method: " << info.biCompression << endl;
    cout << "- Image size: " << info.biSizeImage << endl;
    cout << "- Horizontal ppm: " << info.biXPelsPerMeter << endl;
    cout << "- Vertical ppm: " << info.biYPelsPerMeter << endl;
    cout << "- Colors used: " << info.biClrUsed << endl;
    cout << "- Important colors: " << info.biClrImportant << endl;
}

