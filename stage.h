/*
 * stage.h
 *
 *  Created on: May 1, 2019
 *      Author: Damien
 */

#ifndef STAGE_H_
#define STAGE_H_

#include "LCDLib.h"
#include <stdint.h>
#include <stdbool.h>
#include "game.h"

static uint16_t test_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {
    LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_PURPLE,  LCD_BLUE,    LCD_BLUE,    LCD_PURPLE,  LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,
    LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_PURPLE,  LCD_BLUE,    LCD_BLUE,    LCD_PURPLE,  LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_RED,     LCD_RED,     LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_PURPLE,  LCD_BLACK,
    LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK
};

typedef struct stage_piece {
    int16_t x;
    int16_t y;
    int16_t x_size;
    int16_t y_size;
    
    uint16_t * texture;
} stage_piece_t;

typedef enum {none, left, right, top, bottom} collision_dir;

static stage_piece_t stage_1[] = {
                           {0, 0, 16, MAX_SCREEN_X, test_texture}, // left border
                           {MAX_SCREEN_X-16, 0, 16, MAX_SCREEN_X, test_texture}, // right border
                           {16, 0, MAX_SCREEN_X-16-16, 16, test_texture}, // top border
                           {16, MAX_SCREEN_Y-16, MAX_SCREEN_X-16-16, 16, test_texture}, // bottom border
                           {64, 64, 32, 32, test_texture}, // upper left block
                           {MAX_SCREEN_X-64-32, 64, 32, 32, test_texture}, // upper right block
                           {64, MAX_SCREEN_Y-64-32, 32, 32, test_texture}, // lower left block
                           {MAX_SCREEN_X-64-32, MAX_SCREEN_Y-64-32, 32, 32, test_texture}, // lower right block
                           {MAX_SCREEN_X/2 - 32/2, MAX_SCREEN_Y/2 - 32/2, 32, 32, test_texture}
};

void draw_stage_piece(stage_piece_t * stage_piece);

collision_dir check_collision(Point location, int16_t x_size, int16_t y_size,
                      int16_t x_velocity, int16_t y_velocity,
                      stage_piece_t * stage_piece);

#endif /* STAGE_H_ */
