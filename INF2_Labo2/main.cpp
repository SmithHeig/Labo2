/* 
 * Author: Romain Maffina
 * Date: 11.02.2016
 * 
 * INF2 - Lab 2
 * Some simple image processing
 */

#include "bmp.h"
#include "improc.h"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//OUT bw.bmp lines.bmp checker.bmp castle2.bmp life2.bmp
int main(void) {
    
    string input("castle.bmp");
    string output_bw("bw.bmp");
    string output_lines("lines.bmp");
    string output_checker("checker.bmp");
    string output_orig("castle2.bmp");
    
    string input_life("life.bmp");
    string output_life("life2.bmp");
    
    vector<uint8_t> img_orig, img_bw, img_lines, img_checker, img_life;
    uint32_t w, h, w_life, h_life;
    
    /* input */
    cout << "Reading input BMP image files..." << endl;
    
    if ( !bmp_read(input, img_orig, w, h) ) {
        cerr << "Failed to read '" << input << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    if ( !bmp_read(input_life, img_life, w_life, h_life) ) {
        cerr << "Failed to read '" << input_life << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    cout << "Doing some image processing..." << endl;
    
    /* black and white */
    img_bw = img_orig;    
    invert(img_bw, w, h);
    binary(img_bw, w, h, 128);
    
    /* lines */
    img_lines = img_orig;
    
    // cross in the middle
    draw_diag(img_lines, w, h, w/3, h/4, -1, 255);
    draw_diag(img_lines, w, h, w/4, h/4, 1, 255);
    
    // rule of thirds
    draw_hline(img_lines, w, h, (h/3), 1, 0);
    draw_hline(img_lines, w, h, (h/3)*2, 1, 0);
    draw_vline(img_lines, w, h, (w/3), 1, 0);
    draw_vline(img_lines, w, h, (w/3)*2, 1, 0);    

    // picture frame
    //draw_hline(img_lines, w, h, 0, 10, 128);
    //draw_vline(img_lines, w, h, w-10, 10, 128);
    //draw_hline(img_lines, w, h, h-10, 10, 128);
    //draw_vline(img_lines, w, h, 0, 10, 128);
    
    /* black/transparent checker */
    img_checker = img_orig;
    checker(img_checker, w, h, 100);
    
    /* error checking */
    draw_diag(img_orig, w, h, w, h, -1, 255);
    draw_hline(img_orig, w, h, h-1, 2, 0);
    draw_vline(img_orig, w, h, w, 1, 0);   
    checker(img_orig, w, h, 0);
    
    /* life game - Gosper glider gun */
    life_game(img_life, w_life, h_life, 200);
    
    /* output */
    cout << "Writing back resulting BMP image files..." << endl;
    
    if ( !bmp_write(output_bw, img_bw, w, h) ) {
        cerr << "Failed to write '" << output_bw << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    if ( !bmp_write(output_lines, img_lines, w, h) ) {
        cerr << "Failed to write '" << output_lines << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    if ( !bmp_write(output_checker, img_checker, w, h) ) {
        cerr << "Failed to write '" << output_checker << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    if ( !bmp_write(output_orig, img_orig, w, h) ) {
        cerr << "Failed to write '" << output_orig << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    if ( !bmp_write(output_life, img_life, w_life, h_life) ) {
        cerr << "Failed to write '" << output_life << "' image!" << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

