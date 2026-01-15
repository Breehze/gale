#include "utils.h"

int locate_slice(int position, BufferCtx buffer){
    int accumulated = 0;
    
    for(int i = 0; i < buffer.slices_len; i++){
        if(position < accumulated + buffer.slices[i].len){
            return i;
        }
        accumulated += buffer.slices[i].len;
    }  
    return buffer.slices_len - 1;
}

int get_slice_start(int slice_indx, BufferCtx buffer) {
    int start = 0;
    for(int i = 0; i < slice_indx; i++) {
        start += buffer.slices[i].len;
    }
    return start;
}
