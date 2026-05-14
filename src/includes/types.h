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

typedef enum{
    INSERT,
    NORMAL
}Mode;

typedef struct{
    int start;
    int end;
}BufferView;

typedef struct{
    int left;
    int right;
    int top;
    int bottom;
}Margin;

typedef struct {
    char fpath[100];
    char * mem;
    size_t mem_filled;
    size_t mem_len;
    int buff_pos;
    size_t slices_mem_len;
    size_t slices_mem_filled;
    Slice *slices;
    BufferView view;
    TermCtx logical_terminal;
}BufferCtx;

typedef struct {
    TermPos pos;
    TermPos buffer_pos;
    char * open_fname;
    Mode mode;
}StatusBar;

typedef struct{
    char * frame_buffer;
    size_t frame_buffer_size;
    TermCtx terminal;
    Margin margin;
    int redraw;
}RenderCtx;

#endif

