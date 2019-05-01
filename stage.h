/*
 * stage.h
 *
 *  Created on: Apr 30, 2019
 *      Author: Damien
 */

#ifndef STAGE_H_
#define STAGE_H_

#include "game.h"
#include "LCDLib.h"
#include <stdint.h>

#define stage_block_size 8
#define sqrt_of_block_types 2

#define stage_height_in_chunks MAX_SCREEN_X/2/stage_block_size
#define stage_width_in_chunks MAX_SCREEN_Y/2/stage_block_size

typedef struct stage_block {
    const uint16_t block_colors[stage_block_size][stage_block_size];
} stage_block_t;

#define background_block {\
    .block_colors = {\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR},\
                 {BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR, BACK_COLOR}\
    }\
}
#define red_block {\
    .block_colors = {\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED},\
                 {LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED, LCD_RED}\
    }\
}
#define green_block {\
    .block_colors = {\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN},\
                 {LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN, LCD_GREEN}\
    }\
}
#define blue_block {\
    .block_colors = {\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE},\
                 {LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE, LCD_BLUE}\
    }\
}

static const stage_block_t stage_blocks_lookup[sqrt_of_block_types*sqrt_of_block_types] =
{
 background_block, red_block, green_block, blue_block
};

typedef struct stage_chunk {
    unsigned int block0 : sqrt_of_block_types;
    unsigned int block1 : sqrt_of_block_types;
    unsigned int block2 : sqrt_of_block_types;
    unsigned int block3 : sqrt_of_block_types;
} stage_chunk_t;

static stage_chunk_t test_stage[stage_height_in_chunks * stage_width_in_chunks] =
{
 {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3},
 {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3},
 {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3}, {3, 3, 3, 3},
 {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
 {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
 {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
 {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
 {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2},
 {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1},
 {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1},
 {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1},
 {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1},
 {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},
 {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},
 {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3},
 {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0},
 {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0},
 {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0},
 {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0},
 {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}, {3, 2, 1, 0}
};

//stage_block_t * get_stage_block_from_chunk(stage_chunk_t * chunk,
//                                           uint_fast8_t block_num);

// defined in LCDLib.c
extern void draw_stage_block(uint16_t block_colors[stage_block_size][stage_block_size], int16_t xStart,
                             int16_t yStart);

void draw_stage(stage_chunk_t * stage);

#endif /* STAGE_H_ */
