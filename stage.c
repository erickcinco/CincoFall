/*
 * stage.c
 *
 *  Created on: Apr 30, 2019
 *      Author: Damien
 */

#include "stage.h"
#include "LCDLib.h"
#include "msp.h"
#include <stdint.h>

//stage_block_t * get_stage_block_from_chunk(stage_chunk_t * chunk,
//                                           uint_fast8_t block_num) {
//    switch(block_num)
//    {
//    case 0:
//        return &stage_blocks_lookup[chunk->block0];
//    case 1:
//        return &stage_blocks_lookup[chunk->block1];
//    case 2:
//        return &stage_blocks_lookup[chunk->block2];
//    case 3:
//        return &stage_blocks_lookup[chunk->block3];
//    default:
//        return 0;
//    }
//}

void draw_stage(stage_chunk_t * stage) {
    for(int i = 0; i < stage_height_in_chunks; i++)
    {
        for(int j = 0; j < stage_width_in_chunks; j++)
        {
            stage_chunk_t * chunk = &stage[i*stage_width_in_chunks + j];

            draw_stage_block(((stage_blocks_lookup[chunk->block0]).block_colors),
                             i * stage_block_size,
                             j * stage_block_size);

            draw_stage_block(((stage_blocks_lookup[chunk->block1]).block_colors),
                             i * stage_block_size + stage_block_size,
                             j * stage_block_size);

            draw_stage_block(((stage_blocks_lookup[chunk->block2]).block_colors),
                             i * stage_block_size,
                             j * stage_block_size + stage_block_size);

            draw_stage_block(((stage_blocks_lookup[chunk->block3]).block_colors),
                             i * stage_block_size + stage_block_size,
                             j * stage_block_size + stage_block_size);
        }
    }
}

