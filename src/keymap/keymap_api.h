#ifndef KMAP_API_H
#define KMAP_API_H

typedef int * Error ;
typedef void * Inputs ;

typedef void (*handler)(Inputs,Error);

handler call0(char *);

void update_nest_count(int nest);

void MOV_CURSOR_UP(Inputs,Error);

void MOV_CURSOR_DOWN(Inputs,Error);

void MOV_CURSOR_LEFT(Inputs,Error);

void MOV_CURSOR_RIGHT(Inputs,Error);

void MOV_CURSOR_NEXT_WORD(Inputs,Error);

void MOV_CURSOR_PREVIOUS_WORD(Inputs,Error);

void MOV_CURSOR_EOL(Inputs, Error);

void CENTER_VIEW_AROUND_CURSOR_H(Inputs inputs, Error error); 

void CENTER_VIEW_AROUND_CURSOR_V(Inputs inputs, Error error); 

void EXIT(Inputs inputs,Error error);

void INSERT_MODE(Inputs inputs,Error error);

void INSERT_MODE_APPEND(Inputs inputs,Error error);

void SAVE_BUFFER(Inputs inputs,Error error);

#endif
