#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer_ops.h"
#include "utils.h"


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


void jump_next_word(BufferCtx * buff,int step){
    //step not implemented
    int slice = locate_slice(buff->buff_pos,*buff);
    
    size_t i,state = 0;
    for(i = buff->buff_pos;i < buff->mem_filled;i++){
        if(state && buff->mem[i] != ' '){
            break;
        }
        state = (!state && (buff->mem[i] == ' ' || buff->mem[i] == '\n')) ? 1 : 0; 
    }
    buff->buff_pos =(i >= buff->mem_filled) ? buff->mem_filled - 1 : i;
    
    if(slice > buff->view.end){
        buff->view.start += 1;
    }
};

void jump_previous_word(BufferCtx * buff,int step){
    //step not implemented 
    size_t i,state = 0;
    for(i = buff->buff_pos;i > 0;i--){
        if(state && (buff->mem[i] != ' ')){
            break;
        }
        state = (!state && (buff->mem[i] == ' ' || buff->mem[i] == '\n')) ? 1 : 0; 
    }
    buff->buff_pos =(i >= buff->mem_filled) ? buff->mem_filled - 1 : i;
    
    
    int slice = locate_slice(buff->buff_pos,*buff);
    if(slice < buff->view.start){
        buff->view.start -= 1;
    }
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
