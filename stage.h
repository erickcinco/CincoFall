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

typedef struct stage_piece {
    int16_t x;
    int16_t y;
    int16_t x_size;
    int16_t y_size;
    
    uint16_t * texture[];
} stage_piece_t;

typedef enum {none, left, right, up, down} collision_dir;

collision_dir check_collision(Point location, int16_t x_size, int16_t y_size,
                      int16_t x_velocity, int16_t y_velocity,
                      stage_piece_t * stage_piece);

#endif /* STAGE_H_ */
