#ifndef BUFF_OPS_H 
#define BUFF_OPS_H
#include <stdio.h>
#include "../includes/types.h"

int build_buffer(BufferCtx* buffer,FILE* file);

void update_view_end(BufferCtx* buffer,TermCtx terminal);

void move_buff_pos_up(BufferCtx* buffer,int step);

void move_buff_pos_down(BufferCtx* buffer,int step);

void move_buff_pos_left(BufferCtx* buffer,int step);

void move_buff_pos_right(BufferCtx* buffer, int step);

TermPos translate_buff_pos_absolute(BufferCtx buffer);

TermPos translate_buff_pos_relative(BufferCtx buffer, TermCtx terminal);

#endif
