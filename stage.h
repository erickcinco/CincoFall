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
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_WHITE,   LCD_BLUE,    LCD_BLUE,    LCD_WHITE,   LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,
    LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_WHITE,   LCD_BLUE,    LCD_BLUE,    LCD_WHITE,   LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,     LCD_RED,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_RED,     LCD_RED,     LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_WHITE,   LCD_BLACK,
    LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK,   LCD_BLACK
};

typedef struct stage_piece {
    int16_t x;
    int16_t y;
    int16_t x_size;
    int16_t y_size;
    
    uint16_t * texture[TEXTURE_SIZE * TEXTURE_SIZE];
} stage_piece_t;

typedef enum {none, left, right, up, down} collision_dir;

static stage_piece_t test_piece = {
                            20, 40,
                            100, 120,
                            &test_texture
};

void draw_stage_piece(stage_piece_t * stage_piece);

collision_dir check_collision(Point location, int16_t x_size, int16_t y_size,
                      int16_t x_velocity, int16_t y_velocity,
                      stage_piece_t * stage_piece);

#endif /* STAGE_H_ */
