//
// obj_demo.c
// testing various sprite related things
//
// (20031003 - 20060924, Cearn)

//NOTE: CODE NOT PERSONLIZED YET!!! COPIED DIRECTLY FROM MY TEST DEMO THING

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/tonc/toolbox.h"
#include "../assets/images/santa/santa.h"
#include "../assets/images/platform/platform.h"
#include "../include/DWedit/debug.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *) obj_buffer;
int const NUM_TILES_IN_SANTA = 4;


// testing a few sprite things
// D-pad: move 
// SELECT: switch palette
// START: toggle mapping mode
// A: horizontal flip
// B: vertical flip
// L & R shift starting tile
void obj_test() {
    int x = 96, y = 64;
    u32 tid = 0, pb = 0;        // tile id, pal-bank

    OBJ_ATTR *santa = &obj_buffer[0];
    obj_set_attr(santa,
                 ATTR0_SQUARE | ATTR0_4BPP,                // Square, regular sprite
                 ATTR1_SIZE_16,                    // 16x16p,
                 ATTR2_PALBANK(pb) | tid);        // palbank 0, tile 0

    // position sprite (redundant here; the _real_ position
    // is set further down
    obj_set_pos(santa, x, y);

    OBJ_ATTR *platform = &obj_buffer[NUM_TILES_IN_SANTA];
    obj_set_attr(platform,
                 ATTR0_WIDE | ATTR0_4BPP,                // Square, regular sprite
                 ATTR1_SIZE_32x8,                    // 16x16p,
                 ATTR2_PALBANK(1) | NUM_TILES_IN_SANTA);        // palbank 1, tile 0

    // position sprite (redundant here; the _real_ position
    // is set further down
    obj_set_pos(platform, x, y+16);

    int velocity = 0;
    const int GRAVITY = 1; //pos bc up is down
    const int GROUND_Y = 64;
    const int JUMP_INIT_VELO = -8;//neg bc up is down

    //vbaprint("hello world\n");

    while (1) {
        vid_vsync();
        key_poll();

        // move left/right
        x += 2 * key_tri_horz();

        if (key_released(KEY_LEFT) || key_released(KEY_RIGHT)) {
            tid = 0;
            if (key_released(KEY_LEFT)) {
                santa->attr1 ^= ATTR1_HFLIP;
            }
        }
        if (key_hit(KEY_LEFT) || key_hit(KEY_RIGHT)) {
            //tid = 8; //TODO: uncomment once sprites are there
            if (key_hit(KEY_LEFT)) {
                santa->attr1 ^= ATTR1_HFLIP;
            }
        }

        // move up/down
        y += velocity;

        if (y > GROUND_Y) { // if object is below ground level
            //TODO: stupid code bc allows the object to fall slightly below the "floor" before resetting
            //vbaprint("below ground level\n");
            y = GROUND_Y; //move it back up
            velocity = 0;

            //TODO: uncomment once sprites are there
            //tid = 8 * abs(key_tri_horz()); // sets the sprite to 0 if no directions pressed, 8 otherwise


            //TODO: see below where it says tid = 4
        } else if (y != GROUND_Y) { // if object is in the air
            // apply gravity
            velocity += GRAVITY;
        }

        //accelerate
        //velocity += acceleration;

        // increment/decrement starting tile with R/L
        tid += bit_tribool(key_hit(-1), KI_R, KI_L);

        // flip or jump
        if (key_hit(KEY_A))    // horizontally
            // santa->attr1 ^= ATTR1_HFLIP;
            if (y == GROUND_Y) {
                velocity = JUMP_INIT_VELO;
                // tid = 4; //TODO: uncomment once sprites are there
            }
        if (key_hit(KEY_B)) {
            //hurt animation test
            tid = 12;
            int timer = 0;
            const int WIGGLE_HEIGHT = 3; // height at which to wiggle at
            const int WIGGLE_DURATION = 60;
            const int DESYNC = 2; // every DESYNC frames, it will wiggle. Test on OG hardware bc it wil look different in emu
            y += WIGGLE_HEIGHT;
            while (timer < WIGGLE_DURATION) {
                //TODO: make this not cringe
                //TODO: fade
                vid_vsync();
                y += (timer % DESYNC == 0) ? // if on a frame to wiggle
                     (
                             (timer / DESYNC % 2 == 0) ? -WIGGLE_HEIGHT
                                                       : WIGGLE_HEIGHT //wiggle up or down depending on frame numnber
                     )
                                           : 0; // if not on a wiggle frame, do nothing
                santa->attr2 = ATTR2_BUILD(tid, pb, 0);
                obj_set_pos(santa, x, y);

                oam_copy(oam_mem, obj_buffer, 2);    // only need to update one
                timer++;
            }
            tid = 0; //TODO: if hit while jumping this wont work
            //vbaprint("did tiny hop thing. y: ");
            //vbaprint(y>GROUND_Y? "greater than" : y<GROUND_Y? "less than" : "equal to");
            //vbaprint("\n");
        }

        // make it glow (via palette swapping)
        pb = key_is_down(KEY_SELECT) ? 1 : 0;

        // toggle mapping mode
        if (key_hit(KEY_START))
            REG_DISPCNT ^= DCNT_OBJ_1D;

        // Hey look, it's one of them build macros!
        santa->attr2 = ATTR2_BUILD(tid, pb, 0);
        obj_set_pos(santa, x, y);

        oam_copy(oam_mem, obj_buffer, 128);    // only need to update one
    }
}

int main() {
    // Places the glyphs of a 8bpp boxed santa sprite
    //   into LOW obj memory (cbb == 4)
    memcpy(&tile_mem[4][0], santaTiles, santaTilesLen);
    memcpy(&pal_obj_mem[0], santaPal, santaPalLen);

    // Places the glyphs of a 8bpp boxed platform sprite
    //   into LOW obj memory (cbb == 4)
    memcpy(&tile_mem[4][NUM_TILES_IN_SANTA], platformTiles, platformTilesLen);
    memcpy(&pal_obj_mem[16], platformPal, platformPalLen);

    // inits the oam with 128 objects [from me]
    oam_init(obj_buffer, 128);
    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D; //sets display info

    obj_test();

    while (1);

    return 0;
}
