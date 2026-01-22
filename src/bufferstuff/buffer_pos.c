#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer_ops.h"
#include "utils.h"

int build_buffer(BufferCtx* buffer,const char * fpath){
    *buffer = (BufferCtx){
        .mem_len = 100,
        .mem = (char *)malloc(sizeof(char) * 100),
        .slices_mem_filled = 0,
        .slices_mem_len = 1000,
        .slices = (Slice *)malloc(sizeof(Slice) * 1000)
    };
    
    FILE * file = fopen(fpath,"r");
    if(!file) return 1;
    char c;
    int slice_start = 0;
    int i = 0;
    
    while ((c = fgetc(file)) != EOF) {
        buffer->mem[i] = c;
        if(c == '\n'){
            buffer->slices[buffer->slices_mem_filled].len = i - slice_start + 1;
            buffer->slices_mem_filled++; 
            slice_start = i+1;     
            if(buffer->slices_mem_filled  > buffer->slices_mem_len){
                buffer->slices_mem_len *= 10;
                buffer->slices = (Slice *)realloc(buffer->slices,buffer->slices_mem_len);
            }
        }
        i++;
        if(i > buffer->mem_len){
            buffer->mem_len *= 10;
            char * new_mem_block = (char *)realloc(buffer->mem,buffer->mem_len);
            buffer->mem = new_mem_block;
        }
    }
    fclose(file);
    buffer->mem_filled = i;
    buffer->view.start = 0;
    strncpy(buffer->fpath,fpath,100);
    return 0;
}


void move_buff_pos_up(BufferCtx* buffer,int step){ 
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice - step < 0){
        return;
    }
    
    int x_offset = buffer->buff_pos - get_slice_start(slice,*buffer);
    
    if(x_offset > buffer->slices[slice-1].len-1){
        x_offset = buffer->slices[slice-1].len-1;
    }
    buffer->buff_pos = get_slice_start(slice-1,*buffer) + x_offset;
    if(slice <= buffer->view.start){
        buffer->view.start -= 1;
    }
}


void move_buff_pos_down(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice + step >= buffer->slices_mem_filled){
        return;
    }
    
    int x_offset = buffer->buff_pos - get_slice_start(slice,*buffer);
    if(x_offset > buffer->slices[slice+1].len-1){
        x_offset = buffer->slices[slice+1].len-1;
    }
    buffer->buff_pos = get_slice_start(slice+1,*buffer) + x_offset;
    if(slice >= buffer->view.end){
        buffer->view.start += 1;
    }
}


void move_buff_pos_left(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos - step;
    int slice_start = get_slice_start(slice,*buffer);
    buffer->buff_pos = (new_pos < slice_start) ? slice_start : new_pos;
}


void move_buff_pos_right(BufferCtx* buffer, int step){  
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos + step;
    int slice_end = get_slice_start(slice,*buffer) + buffer->slices[slice].len-1;
    buffer->buff_pos = (new_pos > slice_end) ? slice_end : new_pos;
}


void update_view_end(BufferCtx* buffer,TermCtx terminal){
    int lines_loaded = 0;
    int i = buffer->view.start;
    while(lines_loaded < terminal.rows && i < buffer->slices_mem_filled){
        int slice_lines = (buffer->slices[i].len+terminal.cols) / terminal.cols;
        if(lines_loaded + slice_lines  > terminal.rows){
            break;
        }
        
        lines_loaded += slice_lines; 
        i++;
    }
    buffer->view.end = i-1;
}
