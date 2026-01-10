#include "terminal.c"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int y;
    unsigned int x;
}TermPos; 

typedef struct{
    int len;
    int start;
    int end;
}Slice;

typedef struct {
    char * mem;
    size_t mem_len;
    int buff_pos;
    int slices_len;
    Slice *slices;
}BufferCtx;

int locate_slice(BufferCtx buffer){
    int i = 0;
    for(i = 0;i < buffer.slices_len;i++){
        if(buffer.buff_pos >=buffer.slices[i].start && buffer.buff_pos <= buffer.slices[i].end){
            break;
        }
    }
    return i;
}

void move_buff_pos_up(BufferCtx* buffer,int step){ 
    int slice = locate_slice(*buffer);
    if(slice - step < 0){
        return;
    }
    
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice-1].len){
        x_offset = buffer->slices[slice-1].len;
    }
    buffer->buff_pos = buffer->slices[slice-1].start + x_offset;
}

void move_buff_pos_down(BufferCtx* buffer,int step){
    int slice = locate_slice(*buffer);
    if(slice + step >= buffer->slices_len){
        return;
    }
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice+1].len){
        x_offset = buffer->slices[slice+1].len;
    }
    buffer->buff_pos = buffer->slices[slice+1].start + x_offset;
}

void move_buff_pos_left(BufferCtx* buffer,int step){
    int slice = locate_slice(*buffer);
    int new_pos = buffer->buff_pos - step;
    buffer->buff_pos = (new_pos < buffer->slices[slice].start) ? buffer->slices[slice].start : new_pos;
}

void move_buff_pos_right(BufferCtx* buffer, int step){  
    int slice = locate_slice(*buffer);
    int new_pos = buffer->buff_pos + step;
    buffer->buff_pos = (new_pos > buffer->slices[slice].end) ? buffer->slices[slice].end : new_pos;
}

TermPos translate_buff_pos(BufferCtx buffer){
    int slice = locate_slice(buffer);
    return (TermPos){
        .x = buffer.buff_pos - buffer.slices[slice].start+1,
        .y = slice + 1 
    };
};


int build_buffer(BufferCtx* buffer,FILE* file){
    buffer->mem_len = 1000;
    buffer->mem = (char *)malloc(1000);
    buffer->buff_pos = 0;
    
    buffer->slices_len = 0;
    buffer->slices = (Slice *)malloc(sizeof(Slice) * 1000);
    char c;
    
    int slice_start = 0;
    int i = 0;
    while ((c = fgetc(file)) != EOF) {
        buffer->mem[i] = c;
        if(c == '\n'){
            buffer->slices[buffer->slices_len] = (Slice){
                .start = slice_start,
                .end = i,
                .len = i - slice_start
            };
            buffer->slices_len++; 
            slice_start = i+1;
        }
        i++;
    }
    return 1;
}


int main(int argc, char **argv){
    assert(argc == 2);
    FILE * file = fopen(argv[1],"r");
    BufferCtx buff;
    build_buffer(&buff, file);
    terminal_setup();
    
    printf("\x1b[1;1H%s",buff.mem);
    fflush(stdout);
    
    reset_cursor();
    TermPos a;
    for(;;){
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            switch (c) {
                case 'h':
                    move_buff_pos_left(&buff,1); 
                    a = translate_buff_pos(buff);    
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'j':
                    move_buff_pos_down(&buff,1);
                    a = translate_buff_pos(buff);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'k':
                    move_buff_pos_up(&buff,1);
                    a = translate_buff_pos(buff);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'l':
                    move_buff_pos_right(&buff,1);
                    a = translate_buff_pos(buff);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
            }
        }
    }
    return 0;
}
