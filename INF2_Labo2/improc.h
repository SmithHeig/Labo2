/**
 -----------------------------------------------------------------------------------
 Projet      : Labo2
 \file       : improc.h
 \author     : James Smith
 \date       : 01.03.2017

 \brief      : Functions able to make modification on images. The images are send in 
 *             a vector of unsigned whole number representing the color of each pixels

 Remarque(s) : <à compléter>

 Compilateur : MinGW-g++ 5.3.0
 -----------------------------------------------------------------------------------
 */


#ifndef IMPROC_H
#define IMPROC_H

#include <vector>
#include <cstdint>

using namespace std;

/**
 * @brief Invert the color of a image 
 * @param img_bw a vector with all the pixels's color in 8 bits
 * @param w width of the image
 * @param h hight of the image
 */
void invert(vector<uint8_t>& img_bw, const uint32_t w, const uint32_t h);

/**
 * @brief Convert the ligth color in white and dark color in black
 * @param img_bw image to convert in black and white
 * @param w width of the image
 * @param h hight of the image
 * @param mid_value the value limit to change in white or in black
 */
void binary(vector<uint8_t>& img_bw, const uint32_t w, const uint32_t h, const uint8_t mid_value);

/**
 * @brief Draw a diagonal on a image
 * @param img_lines imgage to draw on.
 * @param w width of the image
 * @param h hight of the image
 * @param w_center middle of the width
 * @param h_center middle of the hight
 * @param direction is the slope of the diagonal
 */
void draw_diag(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t w_center, const uint32_t h_center, const int direction, const uint8_t color);

/**
 * @brief draw a horizontal line on a image
 * @param img_lines image to draw on.
 * @param w width of the image
 * @param h hight of the image
 * @param pos_vertical is the position horizontal where start the line.
 * @param color of the diagonal
 */
void draw_hline(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t pos_vertical, const uint32_t thickness, const uint8_t color);

/**
 * @brief draw a vertical line on a image
 * @param img_lines image to draw on.
 * @param w width of the image
 * @param h hight of the image
 * @param pos_horizontal is the position vertical where start the line
 * @param thickness of the line
 * @param color of the line
 */
void draw_vline(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t pos_horizontal, const uint32_t thickness, const uint8_t color);

/**
 * @brief draw a checker on a image
 * @param img_lines image to draw on
 * @param w width of the image
 * @param h hight of the image
 * @param size_square is the size of the squares
 */
void checker(vector<uint8_t>& img_checker, const uint32_t w, const uint32_t h, const uint32_t size_square);

/**
 * @brief draw the result of a number of iteration of the game of life
 * @param img_life image to draw on
 * @param w_life is the width of the image
 * @param h_life is the hight of the image
 * @param nb_iteration is the number of iteration to play to the game of life
 */
void life_game(vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life, const uint32_t nb_iteration);

#endif /* IMPROC_H */

