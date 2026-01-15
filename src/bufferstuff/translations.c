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
    int slice_start = get_slice_start(slice,  buffer);
   
    int offset_in_line = buffer.buff_pos - slice_start;

    int screen_row_for_line_start = 0;
    
    for (int i = buffer.view.start; i < slice; i++) {
        int rows_needed = (buffer.slices[i].len + terminal.cols - 1) / terminal.cols;
        screen_row_for_line_start += rows_needed;
    }
    
    int final_screen_row = screen_row_for_line_start + offset_in_line / terminal.cols;
    
    return (TermPos){
        .x = offset_in_line % terminal.cols + 1,  
        .y = final_screen_row + 1        
    };
}
