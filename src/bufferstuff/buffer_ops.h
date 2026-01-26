#ifndef BUFF_OPS_H 
#define BUFF_OPS_H
#include <stdio.h>
#include "../includes/types.h"

int build_buffer(BufferCtx* buffer,const char * fpath);

// Buffer position modifications
void update_view_end(BufferCtx* buffer,TermCtx terminal);

void move_buff_pos_up(BufferCtx* buffer,int step);

void move_buff_pos_down(BufferCtx* buffer,int step);

void move_buff_pos_left(BufferCtx* buffer,int step);

void move_buff_pos_right(BufferCtx* buffer, int step);

void jump_next_word(BufferCtx * buff,int step);

void jump_previous_word(BufferCtx * buff,int step);

// Buffer content modifications
void insert_into_buffer(char c,BufferCtx * buffer);

void insert_new_line(BufferCtx * buffer,TermCtx terminal);

void remove_from_buffer(BufferCtx * buffer);

// Buffer cursor position translations
TermPos translate_buff_pos_absolute(BufferCtx buffer);

TermPos translate_buff_pos_relative(BufferCtx buffer, TermCtx terminal);

// File/Buffer operations
void save_buffer(BufferCtx buffer);

#endif
