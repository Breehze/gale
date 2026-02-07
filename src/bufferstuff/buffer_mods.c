#include <stdlib.h>
#include <string.h>

#include "../includes/types.h"
#include "buffer_ops.h"
#include "utils.h"


int build_buffer(BufferCtx* buffer,const char * fpath){
    *buffer = (BufferCtx){
        .mem_len = 100,
        .mem = (char *)malloc(sizeof(char) * 100),
        .slices_mem_filled = 0,
        .slices_mem_len = 1000,
        .slices = (Slice *)calloc(1000,sizeof(Slice)),
        .buff_pos = 0,
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
                buffer->slices = (Slice *)realloc(buffer->slices,buffer->slices_mem_len * sizeof(Slice));
            }
        }
        i++;
        if(i > buffer->mem_len - 1){
            buffer->mem_len *= 10;
            char * new_mem_block = (char *)realloc(buffer->mem,buffer->mem_len);
            buffer->mem = new_mem_block;
        }
    }
    fclose(file);
         
    buffer->mem_filled = i;
    buffer->view.start = 0;
    strncpy(buffer->fpath,fpath,100);
    
    if(!i){
        init_buffer_on_empty_file(buffer);
    }
    
    return 0;
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


void save_buffer(BufferCtx buffer){
    FILE * file = fopen(buffer.fpath,"wb");
    fwrite(buffer.mem,sizeof(buffer.mem[0]),buffer.mem_filled,file);      
    fclose(file);
}


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

    buffer->slices_mem_filled += 1;
    curr_slice += 1;  

    if(curr_slice > buffer->view.end && buffer->view.end >= terminal.rows-1) {
        buffer->view.start++;
    }

    update_view_end(buffer, terminal);
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
        
        if(buffer->view.end >= buffer->slices_mem_filled) {
            buffer->view.end = buffer->slices_mem_filled - 1;
        }
        if(slice - 1 < buffer->view.start){
            buffer->view.start -= 1;
        }
    } else {
        buffer->slices[slice].len--;
    }
    
    memmove(&buffer->mem[buffer->buff_pos - 1],
            &buffer->mem[buffer->buff_pos]
            ,sizeof(char) * (buffer->mem_filled - buffer->buff_pos + 1));
    
    buffer->buff_pos--;
    buffer->mem_filled--;
}
