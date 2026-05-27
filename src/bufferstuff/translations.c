#include "buffer_ops.h"
#include "utils.h"

TermPos translate_buff_pos_absolute(BufferCtx buffer){
    int slice = locate_slice(buffer.buff_pos,buffer);
    int slice_start = get_slice_start(slice,buffer);
    return (TermPos){
        .x = buffer.buff_pos - slice_start +1,
        .y = slice + 1 
    };
};

TermPos translate_buff_pos_relative(BufferCtx buffer, TermCtx terminal){
    int slice = locate_slice(buffer.buff_pos, buffer);
    int slice_start = get_slice_start(slice, buffer);

    int offset_in_line = buffer.buff_pos - slice_start;

    int screen_row = slice - buffer.view.start;
    int screen_col = offset_in_line - buffer.col_offset;  

    return (TermPos){
        .x = screen_col + 1,
        .y = screen_row + 1
    };
}
