#include <stdio.h>
#include <stdlib.h>

#include "buffer_ops.h"
#include "utils.h"

int build_buffer(BufferCtx* buffer,FILE* file){
    *buffer = (BufferCtx){
        .mem_len = 100,
        .mem = (char *)malloc(sizeof(char) * 100),
        .slices_len = 0,
        .slices = (Slice *)malloc(sizeof(Slice) * 1000)
    };

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
        if(i > buffer->mem_len){
            buffer->mem_len *= 10;
            char * new_mem_block = (char *)realloc(buffer->mem,buffer->mem_len);
            buffer->mem = new_mem_block;
        }
    }
    buffer->view.start = 0;
    return 0;
}


void move_buff_pos_up(BufferCtx* buffer,int step){ 
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice - step < 0){
        return;
    }
    
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice-1].len){
        x_offset = buffer->slices[slice-1].len;
    }
    buffer->buff_pos = buffer->slices[slice-1].start + x_offset;
    if(slice <= buffer->view.start){
        buffer->view.start -= 1;
    }
}


void move_buff_pos_down(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice + step >= buffer->slices_len){
        return;
    }
    
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice+1].len){
        x_offset = buffer->slices[slice+1].len;
    }
    buffer->buff_pos = buffer->slices[slice+1].start + x_offset;
    if(slice >= buffer->view.end){
        buffer->view.start += 1;
    }
}


void move_buff_pos_left(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos - step;
    buffer->buff_pos = (new_pos < buffer->slices[slice].start) ? buffer->slices[slice].start : new_pos;
}


void move_buff_pos_right(BufferCtx* buffer, int step){  
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos + step;
    buffer->buff_pos = (new_pos > buffer->slices[slice].end) ? buffer->slices[slice].end : new_pos;
}


void update_view_end(BufferCtx* buffer,TermCtx terminal){
    int lines_loaded = 0;
    int i = buffer->view.start;
    while(lines_loaded < terminal.rows && i < buffer->slices_len){
        int slice_lines = (buffer->slices[i].len+terminal.cols) / terminal.cols;
        if(lines_loaded + slice_lines  > terminal.rows){
            break;
        }
        
        lines_loaded += slice_lines; 
        i++;
    }
    buffer->view.end = i-1;
}
