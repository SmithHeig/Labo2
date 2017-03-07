/*
 -----------------------------------------------------------------------------------
 Laboratoire : Labo2
 Fichier     : improc.cpp
 Auteur(s)   : James Smith
 Date        : 01.03.2017

 Bugs        : None 

 Remarque(s) : None
 -----------------------------------------------------------------------------------
 */

#include "improc.h"

unsigned nb_element_around(const vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life, const unsigned i, const unsigned j, const unsigned color);
void change_state(vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life);

void invert(vector<uint8_t>& img_bw, const uint32_t w, const uint32_t h){
   const uint8_t MAX_COLOR = 255;
   for (unsigned i = 0; i < w * h; ++i){
      img_bw.at(i) = MAX_COLOR - img_bw.at(i);
   }
}

void binary(vector<uint8_t>& img_bw, const uint32_t w, const uint32_t h, const uint8_t mid_value){
   const uint8_t WHITE = 255;
   const uint8_t BLACK = 0;
   for (unsigned i = 0; i < w * h; ++i){
      if(img_bw.at(i) >= mid_value){
         img_bw.at(i) = WHITE;
      }
      else {
         img_bw.at(i) = BLACK;
      }
   }
}

void draw_diag(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t w_center, const uint32_t h_center, const int direction, const uint8_t color){
   if((direction == 1 || direction == -1) && w > w_center && h > h_center){
      int size_diago;
      if(w > h){
         if (w - w_center > w_center){
            size_diago = int(w - w_center);
         } else{
            size_diago = int(w_center);
         }
      } else {
         if(h - h_center > h_center){
            size_diago = int(h - h_center);
         } else {
            size_diago = int(h_center);
         }
      }
      unsigned center = h_center * (w) + w_center;
      unsigned pos_diag_sup = 0;
      unsigned pos_diag_inf = 0;
      for(long i = 0; i < size_diago; ++i){
         if(direction == -1) {
            if(long(h_center) - i > 0 && long(w_center) - i > 0){
               pos_diag_sup = center - (w * i) - i; // Diag sup
            }
            if(long(h_center) + i < long(h) && long(w_center) + i < long(w)){
               pos_diag_inf = center + (w * i) + i; // Diag inf
            }
         } else {
            if(long(h_center) - i > 0 && long(w_center) + i < long(w)){
               pos_diag_sup = center - (w * i) + i; // diag sup
            }
            if(long(w_center) - i > 0 && long(h_center) + i < long(h)){
               pos_diag_inf = center + (w * i) - i; // diag inf
            }
         }
         img_lines.at(pos_diag_sup) = color;
         img_lines.at(pos_diag_inf) = color;
      }
   }
}

void draw_hline(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t pos_vertical, const uint32_t thickness, const uint8_t color){
   if(pos_vertical + thickness <= h){
      unsigned pos_initial = pos_vertical * w;
      for(unsigned i = 0; i < w * thickness; ++i){
         img_lines.at(pos_initial + i) = color;
      } 
   }
   
}

void draw_vline(vector<uint8_t>& img_lines, const uint32_t w, const uint32_t h, const uint32_t pos_horizontal, const uint32_t thickness, const uint8_t color){
   if(pos_horizontal + thickness <= w){
      for(unsigned i = 0; i < h; ++i){
         for(unsigned j = 0; j < thickness; ++j){
            img_lines.at((pos_horizontal + w * i) + j) = color;
         }
      }  
   }
}

void checker(vector<uint8_t>& img_checker, const uint32_t w, const uint32_t h, const uint32_t size_square){
   if(size_square > 0){
      unsigned j = 0;
      unsigned i = 0;
      while(i < h){
         while(j < w){
            img_checker.at(i * w + j) = 0;
            // Test if j is at the end of the square
            if (++j % size_square == 0){
               j += size_square; // Jump to the position to draw the next square
            }
         }
         // Test if the next line is the start of a square
         if(++i / size_square % 2 == 0){
            j = 0; // Start the square one the first pixel of the line
         } else {
            j = size_square; // Leave a empty square before the first black square
         }
      }
   }
}

void life_game(vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life, const uint32_t nb_iteration){
   for (unsigned i = 0; i < nb_iteration; ++i){
      change_state(img_life, w_life, h_life);
   }
}

void change_state(vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life){
  const uint8_t COLOR_ACTIVE = 255;
   const uint8_t COLOR_INACTIVE = 0;
   vector<uint8_t> v_temp = img_life;
   unsigned nb_element;
   unsigned pos;
   for (unsigned i = 0; i < h_life; ++i){
      for (unsigned j =0; j < w_life; ++j){
         nb_element = nb_element_around(img_life, w_life, h_life, i, j, COLOR_ACTIVE);
         pos = i * w_life + j;
         // Test if a element is going to "die" or not
         if (img_life.at(pos) == COLOR_ACTIVE && nb_element != 2 && nb_element != 3){
            v_temp.at(pos) = COLOR_INACTIVE;
         }
         else {
            // Test if a element is going to "born"
            if(nb_element == 3){
               v_temp.at(pos) = COLOR_ACTIVE;
            }
         }
      }
   }
   img_life = v_temp; 
}

unsigned nb_element_around(const vector<uint8_t>& img_life, const uint32_t w_life, const uint32_t h_life, const unsigned pos_line, const unsigned pos_col, const unsigned color){
   unsigned cmpt = 0;
   // Test if the element is one the first line
   unsigned start_line = pos_line == 0 ? pos_line : pos_line - 1;
   // Test if the element is one the last line
   unsigned end_line = pos_line == h_life - 1 ? pos_line : pos_line + 1;
   // Test if the element is one the first column
   unsigned start_col = pos_col == 0 ? pos_col : pos_col - 1;
   // Test if the element is one the last column
   unsigned end_col = pos_col == w_life - 1 ? pos_col : pos_col + 1;

   for (unsigned i = start_line; i <= end_line; ++i) {
      for (unsigned j = start_col; j <= end_col; ++j) {
         // Test if the element currently analyse is the same then the base one 
         // and test if it's an active element
         if (!(i == pos_line && j == pos_col) && img_life.at(i * w_life + j) == color) {
            ++cmpt;
         }
      }
   }
   return cmpt;
}

