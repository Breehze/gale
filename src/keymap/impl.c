#include <stdlib.h>

#include "keymap_api.h"
#include "../includes/common_keymap.h"
#include "../bufferstuff/buffer_ops.h"

void MOV_CURSOR_DOWN(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_down(buff,1);
}

void MOV_CURSOR_UP(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_up(buff,1);
}

void MOV_CURSOR_LEFT(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_left(buff,1);
}

void MOV_CURSOR_RIGHT(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_right(buff,1);
}

void EXIT(Inputs inputs,Error error){
    exit(0);
}
