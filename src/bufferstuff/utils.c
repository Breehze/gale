#include "utils.h"

int locate_slice(int position, BufferCtx buffer){
    int accumulated = 0;
    
    for(int i = 0; i < buffer.slices_mem_filled; i++){
        if(position < accumulated + buffer.slices[i].len){
            return i;
        }
        accumulated += buffer.slices[i].len;
    }  
        return buffer.slices_mem_filled - 1;
}

int get_slice_start(int slice_indx, BufferCtx buffer) {
    int start = 0;
    for(int i = 0; i < slice_indx; i++) {
        start += buffer.slices[i].len;
    }
    return start;
}

int init_buffer_on_empty_file(BufferCtx* buffer){
    if(buffer->mem_filled){ return 1;}
    if(buffer->slices_mem_filled) {return 1;}
    buffer->slices_mem_filled = 1;
    
    if(!buffer->slices[0].len){
        buffer->slices[0].len = 1;
        buffer->mem[0] = '\n';
        buffer->mem_filled ++;
    }   
    return 0;
}
