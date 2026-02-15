#include <stdlib.h>

#include "keymap_api.h"
#include "../includes/common_keymap.h"
#include "../bufferstuff/buffer_ops.h"

void MOV_CURSOR_DOWN(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_down(buff,1);
    update_view_end(0, buff,*term);
}

void MOV_CURSOR_UP(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_up(buff,1);
    update_view_end(1, buff,*term);
}

void MOV_CURSOR_LEFT(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_left(buff,1);
    update_view_end(0, buff,*term);
}

void MOV_CURSOR_RIGHT(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    move_buff_pos_right(buff,1);
    update_view_end(0, buff,*term);
}

void MOV_CURSOR_NEXT_WORD(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    jump_next_word(buff,1);
    update_view_end(0, buff,*term);
}

void MOV_CURSOR_PREVIOUS_WORD(Inputs inputs, Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    jump_previous_word(buff,1);
    update_view_end(1, buff,*term);
}

void EXIT(Inputs inputs,Error error){
    exit(0);
}

void INSERT_MODE(Inputs inputs,Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    Mode * mode = ((WrappedInput*)inputs)->mode;
    *mode = INSERT;
}

void SAVE_BUFFER(Inputs inputs,Error error){
    BufferCtx * buff = ((WrappedInput*)inputs)->buff;
    TermCtx * term = ((WrappedInput *)inputs)->term;
    save_buffer(*buff);
}
