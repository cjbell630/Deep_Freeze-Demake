//---------------------------------------------------------------------------------
// GBA sample code for devkitARM - http://www.devkitpro.org
// xboo hello world
//
//---------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <mappy.h>
#include <gba_console.h>
#include <gba_types.h>
#include <gba_sio.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>

#include "gba_systemcalls.h"


// for mbv2 use mbv2.h
// for vba/mappy use mappy.h

#include "xcomms.h"

void u8loop(u8 *src, u8 *dst)  {

    for ( u8 i = 0; i< 32; i++) {
        dst[i] = src[i];
    }
}

void intloop(u8 *src, u8 *dst)  {

    for ( int i = 0; i< 32; i++) {
        dst[i] = src[i];
    }
}

char *int_to_string(int i){
    int length = snprintf( NULL, 0, "%d", i);
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", i);
    return str;
}

bool *u16_to_array(u16 u){
    bool array[16];
    int i;
    for(i = 0; i<16; i++){
        array[i]= (u & 0x01)==1;
        u = u >> 1;
    }
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
//---------------------------------------------------------------------------------
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    consoleDemoInit();
    fprintf(stdout, "debug message on GBA console screen");
    clearerr(stdout);
    int h = 0;
    float f = 0;
    getchar();
    u16 keys = keysDown();
    while (1){
        scanKeys();
        VBlankIntrWait();
        keys = keysHeld();
        fprintf(stdout, "\x1b[2J");
        h = (h+1)%200;
        fprintf(stdout, int_to_string(h));
        fprintf(stdout, "\nkeys: ");

        if(keys & KEY_A){
            fprintf(stdout, "A");
        }
        bool *keys_array = u16_to_array(keys);
        fprintf(stdout, "\n");

        int i = 0;
        while(i<16){
            if(keys & 1){
                fprintf(stdout, "1");
            }else{
                fprintf(stdout, "0");
            }
            keys >>= 1;
            i++;
        }

        char *bruh = "";
        for(i = 0; i<16; i++){
            fprintf(stdout, int_to_string(i));
            fprintf(stdout, ": ");
            if(keys_array[i]==true){
                bruh = "true";
            }else{
                bruh = "false";
            }
            fprintf(stdout, bruh);
            fprintf(stdout, "\n");
        }
    }
}
