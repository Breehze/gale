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

void EXIT(Inputs inputs,Error error);


#endif
