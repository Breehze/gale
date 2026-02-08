#ifndef COMMON_KEYMAP_H
#define COMMON_KEYMAP_H
/*Interface between main.c and keymap*/

#include "types.h"

typedef struct{
    BufferCtx * buff;
    TermCtx * term;
}WrappedInput;

typedef int * Error ;
typedef void * Inputs ;

typedef void (*handler)(Inputs,Error);

handler call0(char *);

#endif
