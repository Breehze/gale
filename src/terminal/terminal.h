#ifndef TERMINAL_H 
#define TERMINAL_H

#include "../includes/types.h"

void clear_screen(void);

void reset_cursor(void);

void move_cursor(TermPos locationTermPos);

void move_cursor(TermPos location);

void change_cursor_to_line(void);

void change_cursor_to_block(void);

TermCtx terminal_setup(void);

TermCtx calculate_content_size(RenderCtx *render_ctx);

int calculate_line_margin(BufferCtx * buffer);

RenderCtx * init_render_ctx(TermCtx terminal);

void draw_buffer(BufferCtx * buffer, RenderCtx * render_ctx);

void render_frame(BufferCtx *buffer,char *command, RenderCtx *render_ctx);

void SBAR_update(StatusBar * bar,TermPos buffer_pos,char * open_fname,Mode mode);

#endif 
