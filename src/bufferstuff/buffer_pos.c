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


void move_buff_pos_eol(BufferCtx* buffer){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int slice_end = get_slice_start(slice,*buffer) + buffer->slices[slice].len-1;
    buffer->buff_pos = slice_end;
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
    
    if(locate_slice(buff->buff_pos,*buff) > buff->view.end){
        buff->view.start += 1;
    }
};


void center_around_cursor_v(BufferCtx *buff){
    int cursor_line = locate_slice(buff->buff_pos, *buff);
    int half_screen = buff->logical_terminal.rows / 2;

    buff->view.start = cursor_line - half_screen;

    if(buff->view.start < 0){
        buff->view.start = 0;
    }

    int max_start = buff->slices_mem_filled - buff->logical_terminal.rows;
    if(max_start < 0) max_start = 0;
    if(buff->view.start > max_start){
        buff->view.start = max_start;
    }

    update_view_end(0, buff);
}


void center_around_cursor_h(BufferCtx *buff){
    int slice = locate_slice(buff->buff_pos, *buff);
    int slice_start = get_slice_start(slice, *buff);
    int col_in_line = buff->buff_pos - slice_start;
    int half_cols = buff->logical_terminal.cols / 2;

    buff->col_offset = col_in_line - half_cols;

    if(buff->col_offset < 0){
        buff->col_offset = 0;
    }

    int line_len = buff->slices[slice].len - 1;
    int max_offset = line_len - buff->logical_terminal.cols;
    if(max_offset < 0) max_offset = 0;
    if(buff->col_offset > max_offset){
        buff->col_offset = max_offset;
    }
}


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
