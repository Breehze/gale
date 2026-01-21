#include <stdlib.h>
#include <string.h>

#include "../includes/types.h"
#include "buffer_ops.h"
#include "utils.h"


void insert_into_buffer(char c,BufferCtx * buffer){
    if(++buffer->mem_filled > buffer->mem_len){
        buffer->mem_len *= 10;
        buffer->mem = (char*)realloc(buffer->mem,buffer->mem_len);
    }
    
    size_t tail_len = buffer->mem_filled - buffer->buff_pos;
    memmove(&buffer->mem[buffer->buff_pos+1],&buffer->mem[buffer->buff_pos],tail_len);
    buffer->slices[locate_slice(buffer->buff_pos,*buffer)].len += 1;
    buffer->mem[buffer->buff_pos] = c;
    buffer->buff_pos +=1;
}


void insert_new_line(BufferCtx * buffer,TermCtx terminal){
    if(buffer->slices_mem_filled + 1 > buffer->slices_mem_len){
        buffer->slices_mem_len *= 10;
        buffer->slices = (Slice *)realloc(buffer->slices,buffer->slices_mem_len);
    }

    if(buffer->mem_filled + 1 > buffer->mem_len){
        buffer->mem_len *= 10;
        buffer->mem = (char*)realloc(buffer->mem,buffer->mem_len);
    }
    
    int curr_slice = locate_slice(buffer->buff_pos,*buffer);
    int slice_start = get_slice_start(curr_slice,*buffer);
    int pos_in_slice = buffer->buff_pos - slice_start;
    
    size_t tail_len = buffer->mem_filled - buffer->buff_pos;
    memmove(&buffer->mem[buffer->buff_pos+1],&buffer->mem[buffer->buff_pos],tail_len);

    buffer->mem_filled +=1;
    buffer->mem[buffer->buff_pos] = '\n';

    size_t slice_tail = (buffer->slices_mem_filled - 1)  - curr_slice;
    memmove(&buffer->slices[curr_slice+2],&buffer->slices[curr_slice+1],slice_tail * sizeof(Slice));

    buffer->slices[curr_slice+1].len = buffer->slices[curr_slice].len - pos_in_slice;  
    buffer->slices[curr_slice].len = pos_in_slice + 1 ;

    buffer->buff_pos +=1;

    if(buffer->slices_mem_filled > terminal.rows - 1 ){
        if(locate_slice(buffer->buff_pos,*buffer) > terminal.rows - 1){
            buffer->view.start += 1;
            update_view_end(buffer,terminal);
        }
    }else{
        buffer->view.end +=1;
    }
   
    buffer->slices_mem_filled += 1;
}

void remove_from_buffer(BufferCtx * buffer){
    int slice  = locate_slice(buffer->buff_pos,*buffer);
    if(buffer->buff_pos - 1 < 0){ 
        return;
    }
    
    if(buffer->mem[buffer->buff_pos - 1] == '\n'){
        buffer->slices[slice - 1].len += buffer->slices[slice].len - 1;      
        memmove(&buffer->slices[slice],             
                &buffer->slices[slice + 1],
                (buffer->slices_mem_filled - slice - 1) * sizeof(Slice));
        buffer->slices_mem_filled--;
    } else {
        buffer->slices[slice].len--;
    }
    
    memmove(&buffer->mem[buffer->buff_pos - 1],
            &buffer->mem[buffer->buff_pos]
            ,sizeof(char) * (buffer->mem_filled - buffer->buff_pos + 1));
    
    buffer->buff_pos--;
    buffer->mem_filled--;
}
