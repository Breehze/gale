#include "utils.h"

int locate_slice(int position,BufferCtx buffer){
    int i = 0;
    for(i = 0;i < buffer.slices_len;i++){
        if(position >=buffer.slices[i].start && position <= buffer.slices[i].end){
            break;
        }
    }
    return i;
}
