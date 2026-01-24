#include <stdio.h>

#include "../includes/types.h"

void SBAR_draw(StatusBar bar){
    if(bar.mode == NORMAL){   
        printf("\x1b[30m\x1b[41m\x1b[%d;1H NORMAL \x1b[0m",bar.pos.y);
    }else{   
        printf("\x1b[30m\x1b[41m\x1b[%d;1H INSERT \x1b[0m",bar.pos.y);
    }
    
    printf("\x1b[%d;%dH %s",bar.pos.y,bar.pos.x - 30,bar.open_fname);
    printf("\x1b[%d;%dH %d:%d",bar.pos.y,bar.pos.x - 10,bar.buffer_pos.y,bar.buffer_pos.x);
    fflush(stdout);
};

void SBAR_update(StatusBar * bar,TermPos buffer_pos,char * open_fname,Mode mode){
    bar->buffer_pos = buffer_pos ;
    bar->open_fname = open_fname;
    bar->mode = mode;
}
