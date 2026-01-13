#ifndef TERMINAL_H 
#define TERMINAL_H

#include "../includes/types.h"

void clear_screen(void);

void reset_cursor(void);

TermCtx terminal_setup(void);

void draw_buffer(BufferCtx buffer);

#endif 
