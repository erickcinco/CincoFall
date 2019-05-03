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


typedef struct stage_piece {
    int16_t x;
    int16_t y;
    int16_t x_size;
    int16_t y_size;

    uint16_t * texture;
} stage_piece_t;

typedef enum {none, left, right, top, bottom} collision_dir;


#if (MAP_CHOICE == 1)

static uint16_t border_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {
                                                               0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef9e, 0xef9e,
                                                                                                                                  0xf7bf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xffff, 0xffff, 0xffff, 0xef9e, 0xcebc, 0xe75e, 0xef7e, 0xf7df, 0xffff,
                                                                                                                                  0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xef9e, 0xbe5b, 0xc69c, 0xd6fd, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf7df, 0xcebc, 0xbe5b, 0xae1b,
                                                                                                                                  0xef9f, 0xf7bf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
                                                                                                                                  0xef7e, 0xc69c, 0xdf3d, 0xd6fd, 0xef7e, 0xffff, 0xe75e, 0xdf3e, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xffff, 0xffff, 0xffff, 0xf7bf, 0xb63b, 0xa5da, 0xd6dd, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xffff, 0xdf3d, 0xe75e, 0xffff, 0xffff, 0xf7df, 0xef9e, 0xe75e, 0xbe5b,
                                                                                                                                  0x9d9a, 0xbe7c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef9e, 0xcedc, 0xf7bf,
                                                                                                                                  0xffff, 0xef7e, 0xe75e, 0xdf1d, 0xdf1d, 0xcedd, 0xf7bf, 0xffff, 0xd71d, 0xcedd,
                                                                                                                                  0xe77e, 0xc67c, 0xef9e, 0xc6bc, 0xdf3d, 0xef7e, 0xffff, 0xdf3d, 0xcebc, 0xdf1d,
                                                                                                                                  0xd6fd, 0xd6fd, 0xc67c, 0xcedd, 0xd71d, 0xcedd, 0xcedd, 0xdf3e, 0xcebc, 0xdf3e,
                                                                                                                                  0xcebc, 0xcedd, 0xe77e, 0xdf3d, 0xb63b, 0xef9e, 0xffff, 0xffff, 0xf7df, 0xc69c,
                                                                                                                                  0xffff, 0xef7e, 0xef9e, 0xe77e, 0xf7bf, 0xd6fd, 0xf7bf, 0xd6dd, 0xf7df, 0xe75e,
                                                                                                                                  0xc67c, 0xdf1d, 0xbe5b, 0xc69c, 0xbe5b, 0xbe5b, 0xc67c, 0xbe7c, 0xc69c, 0xb63b,
                                                                                                                                  0xb63b, 0xbe5b, 0xc67c, 0xbe7c, 0xf7bf, 0xdf1d, 0xc69c, 0xc69c, 0xb63b, 0xa5ba,
                                                                                                                                  0xb63b, 0xb63b, 0xae1b, 0xa5fb, 0xb63b, 0xbe7c, 0xae1b, 0xbe7c, 0xa5db, 0xbe5b,
                                                                                                                                  0xffdf, 0xffdf, 0xef9e, 0xef9e, 0xef9e, 0xb65b, 0xc67b, 0xef3c, 0xf75c, 0xef3c,
                                                                                                                                  0xef5d, 0xef5c, 0xef5c, 0xf75c, 0xce9b, 0xb63b, 0xe75e, 0xffff, 0xffff, 0xffff,
                                                                                                                                  0xffff, 0xef9e, 0xffbe, 0xffbd, 0xffbd, 0xffbe, 0xffde, 0xffde, 0xffbe, 0xffbd,
                                                                                                                                  0xf77d, 0xef9f, 0xffff};

static uint16_t box_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd78, 0xfd79, 0xfd9a, 0xfdba, 0xfdba, 0xfd99, 0xfd78,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfdba,
                                                   0xf558, 0xc494, 0x9b8f, 0xbc32, 0xed37, 0xfd9a, 0xfd79, 0xfd79, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd99, 0xfd99, 0xac12, 0x52ab, 0x3a4a, 0x3208, 0x3208,
                                                   0x526a, 0xc494, 0xfdba, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd99,
                                                   0x6a6a, 0x10a2, 0x18e3, 0x18c3, 0x18e3, 0x18e3, 0x2104, 0x31c7, 0xc474, 0xfd99,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd78, 0xfdba, 0xb3f1, 0x861, 0x20c3, 0x28e3, 0x18c2,
                                                   0x20c2, 0x4984, 0x4143, 0x20e2, 0x4a29, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd58,
                                                   0xfdda, 0x8b0d, 0x1061, 0x59e6, 0x6a68, 0x51e7, 0x7288, 0xbc2c, 0xab8a, 0x8266,
                                                   0x2145, 0xd4d5, 0xfd99, 0xfd79, 0xfd79, 0xfd78, 0xfdda, 0xabd1, 0x3123, 0x8b0a,
                                                   0x9bae, 0xb470, 0xccf2, 0xbcaf, 0xbc4c, 0xab69, 0x18c2, 0xbc53, 0xfdba, 0xfd78,
                                                   0xfd79, 0xfd79, 0xfd99, 0xf537, 0x51c6, 0x6248, 0x6269, 0x8b2b, 0x9b6d, 0x5a07,
                                                   0x7268, 0xabaa, 0x6a47, 0xdc94, 0xfdba, 0xfd78, 0xfd79, 0xfd79, 0xfd99, 0xf517,
                                                   0x7a69, 0x7289, 0x7269, 0x932b, 0xc42d, 0x9b6b, 0xa38c, 0xc48d, 0xbc0c, 0xe493,
                                                   0xfd9a, 0xfd78, 0xfd79, 0xfd79, 0xfd79, 0xfd99, 0xabaf, 0x936d, 0xabce, 0x9b6d,
                                                   0xd4ef, 0xbbec, 0xb42f, 0xbc0c, 0xbc4d, 0xfd78, 0xfd79, 0xfd79, 0xfd79, 0xfd79,
                                                   0xfd78, 0xfdba, 0xbc11, 0x82ea, 0x932c, 0x82eb, 0x934c, 0xa36a, 0x932b, 0xa38b,
                                                   0xbc2e, 0xfd99, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd99, 0xf537, 0x8b2b,
                                                   0x830b, 0x7aaa, 0x932c, 0x932a, 0xa38c, 0xa3cb, 0xe4f5, 0xfd99, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd99, 0xcc73, 0x830b, 0x8b2b, 0x8aca, 0xa38b,
                                                   0xabcb, 0xcc51, 0xfd9a, 0xfd78, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd9a, 0xed17, 0xbc31, 0x9bae, 0xb40f, 0xdcd4, 0xfd99, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd78, 0xfd99, 0xfd99,
                                                   0xfd58, 0xfd79, 0xfd99, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79,
                                                   0xfd79, 0xfd79, 0xfd79, 0xfd79, 0xfd79};

static stage_piece_t stage_1[] = {
                           {0, 0, 16, MAX_SCREEN_X, border_texture}, // left border
                           {MAX_SCREEN_X-16, 0, 16, MAX_SCREEN_X, border_texture}, // right border
                           {16, 0, MAX_SCREEN_X-16-16, 16, border_texture}, // top border
                           {16, MAX_SCREEN_Y-16, MAX_SCREEN_X-16-16, 16, border_texture}, // bottom border
                           {64, 64, MAX_SCREEN_X-64-64, 32, box_texture}, // upper block
                           {64, MAX_SCREEN_Y-64-32, MAX_SCREEN_X-64-64, 32, box_texture} // lower block
};
#else

static uint16_t border_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {
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

static uint16_t box_texture[TEXTURE_SIZE * TEXTURE_SIZE] = {
    LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,
    LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,
    LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY,    LCD_GREEN,
    LCD_GRAY,    LCD_GRAY,    LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GREEN,   LCD_GRAY,    LCD_GRAY
};

static stage_piece_t stage_1[] = {
                           {0, 0, 16, MAX_SCREEN_X, border_texture}, // left border
                           {MAX_SCREEN_X-16, 0, 16, MAX_SCREEN_X, border_texture}, // right border
                           {16, 0, MAX_SCREEN_X-16-16, 16, border_texture}, // top border
                           {16, MAX_SCREEN_Y-16, MAX_SCREEN_X-16-16, 16, border_texture}, // bottom border
                           {64, 64, 32, 32, box_texture}, // upper left block
                           {MAX_SCREEN_X-64-32, 64, 32, 32, box_texture}, // upper right block
                           {64, MAX_SCREEN_Y-64-32, 32, 32, box_texture}, // lower left block
                           {MAX_SCREEN_X-64-32, MAX_SCREEN_Y-64-32, 32, 32, box_texture}, // lower right block
                           {MAX_SCREEN_X/2 - 32/2, MAX_SCREEN_Y/2 - 32/2, 32, 32, box_texture}
};

#endif

void draw_stage_piece(stage_piece_t * stage_piece);

collision_dir check_collision(Point location, int16_t x_size, int16_t y_size,
                      int16_t x_velocity, int16_t y_velocity,
                      stage_piece_t * stage_piece);

#endif /* STAGE_H_ */
