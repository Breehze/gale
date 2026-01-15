#ifndef TYPES_H
#define TYPES_H
#include <unistd.h>

typedef struct {
    unsigned int y;
    unsigned int x;
}TermPos; 

typedef struct {
    int rows;
    int cols;
}TermCtx;

typedef struct{
    int len;
}Slice;

typedef struct{
    int start;
    int end;
}BufferView;

typedef struct {
    char * mem;
    size_t mem_len;
    int buff_pos;
    int slices_len;
    Slice *slices;
    BufferView view;
}BufferCtx;

#endif

