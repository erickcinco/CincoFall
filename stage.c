/*
 * stage.c
 *
 *  Created on: May 1, 2019
 *      Author: Damien
 */

#include "stage.h"
#include "LCDLib.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void draw_stage_piece(stage_piece_t * stage_piece) {
    LCD_Draw_Textured_Block(stage_piece->x, stage_piece->x+stage_piece->x_size,
                            stage_piece->y, stage_piece->y+stage_piece->y_size,
                            stage_piece->texture);
}

collision_dir check_collision(Point location, int16_t x_size, int16_t y_size,
                      int16_t x_velocity, int16_t y_velocity,
                      stage_piece_t * stage_piece) {
    // we're doing predictions, not detecting already existing collisions
    int16_t new_x = location.x + x_velocity;
    int16_t new_y = location.y + y_velocity;

    if(((new_x <= (stage_piece->x + stage_piece->x_size)) &&
            ((new_x + x_size) >= stage_piece->x)) &&
       ((new_y <= (stage_piece->y + stage_piece->y_size)) &&
            ((new_y + y_size) >= stage_piece->y)))
    {
        // if the location came from the y direction (because x still passing collision check)
        if(((location.x <= (stage_piece->x + stage_piece->x_size)) &&
                ((location.x + x_size) >= stage_piece->x)))
        {
            if(location.y < stage_piece->y) // if it's below the block and hitting it
                return top;
            else // if it's above it and hitting it
                return bottom;
        }
        // if the location came from the x direction
        else
        {
            if(location.x < stage_piece->x) // if it's hitting the block from the left
                return left;
            else
                return right;
        }
    }
    else
    {
        return none;
    }
}
