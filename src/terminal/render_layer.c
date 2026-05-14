#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "terminal.h"

#include "../includes/types.h"
#include "../bufferstuff/utils.h"


RenderCtx * init_render_ctx(TermCtx terminal){
    RenderCtx * new_ctx = (RenderCtx *)malloc(sizeof(RenderCtx));
    if(!new_ctx){
        return NULL;
    }
    *new_ctx = (RenderCtx){
        .terminal = terminal,
        .frame_buffer_size = terminal.cols * terminal.rows,
        .frame_buffer = (char *)malloc(sizeof(char) * (terminal.rows ) * terminal.cols + 1),
        .margin = {0, 0, 0, 0},
        .redraw = 0
    };
    if(!new_ctx->frame_buffer){
        free(new_ctx);
        return NULL;
    }
     return new_ctx;
};


TermCtx calculate_content_size(RenderCtx *render_ctx){
    TermCtx availible_draw_size = render_ctx->terminal;
    availible_draw_size.rows -= render_ctx->margin.top + render_ctx->margin.bottom;
    availible_draw_size.cols -= render_ctx->margin.left + render_ctx->margin.right;
    return availible_draw_size;
}


int calculate_line_margin(BufferCtx * buffer){
    int digits = 0;
    for(int i = 1; (buffer->slices_mem_filled / (1 * i)) != 0 ;i*=10,digits++);
    return digits;
}


int translate_row2absolute(int row,RenderCtx * render_ctx){
    return row * (render_ctx->terminal.cols + 1);
}


void draw_buffer(BufferCtx *buffer, RenderCtx *render_ctx){
    int render_row = 0;

    for(int buffer_line_idx = buffer->view.start; buffer_line_idx <= buffer->view.end;buffer_line_idx++){
        int slice_start = get_slice_start(buffer_line_idx, *buffer);
        int render_line_idx = translate_row2absolute(render_row, render_ctx) + render_ctx->margin.left;

        for(int buffer_cpy_pos = slice_start;buffer_cpy_pos < slice_start + buffer->logical_terminal.cols ;buffer_cpy_pos++){
            if(render_line_idx >= render_ctx->frame_buffer_size - 1){
                break;
            }
            if(buffer_cpy_pos < slice_start + buffer->slices[buffer_line_idx].len - 1){
                render_ctx->frame_buffer[render_line_idx] = buffer->mem[buffer_cpy_pos];
                render_line_idx++;
            }else{
                render_ctx->frame_buffer[render_line_idx] = ' ';
                render_line_idx++;
            }
        }
        
        if(render_line_idx >= render_ctx->frame_buffer_size - 1){
            break;
        }

        if(render_row < render_ctx->terminal.rows - 1){
            int newline_pos = translate_row2absolute(render_row, render_ctx) + render_ctx->terminal.cols;
            render_ctx->frame_buffer[newline_pos] = '\n';
        }

        render_row++;
    }
}

void render_lnumbers(BufferCtx *buffer, RenderCtx * render_ctx){
    char tmp_buff[21] = {0};
    int render_row = 0;
    int margin = calculate_line_margin(buffer);
    for(int slice = buffer->view.start; slice <= buffer->view.end; slice++){
        int row_start = translate_row2absolute(render_row, render_ctx);
        snprintf(tmp_buff, 20, "%d",slice + 1);

        int len = strlen(tmp_buff) - 1;
        for(int i = len; i >= 0; i--){
            render_ctx->frame_buffer[row_start + (margin - i -1 )] = tmp_buff[len - i];
        }
        render_row++;
    }
};



//void draw_buffer(BufferCtx *buffer, RenderCtx *render_ctx){
//    memset(render_ctx->frame_buffer, ' ', render_ctx->frame_buffer_size);
//    int i2 = 0;
//
//    for(int i = buffer->view.start; i <= buffer->view.end; i++){
//        int slice_start = get_slice_start(i, *buffer);
//        int rows_taken = (buffer->slices[i].len / buffer->logical_terminal.cols) + 1;
//
//        for(int j = slice_start; j < slice_start + (buffer->logical_terminal.cols * rows_taken); j++){
//            if(i2 >= render_ctx->frame_buffer_size - 1){
//                break;
//            }
//
//            if(j < slice_start + buffer->slices[i].len - 1){
//                render_ctx->frame_buffer[i2] = buffer->mem[j];
//                i2++;
//            }else{
//                render_ctx->frame_buffer[i2] = ' ';
//                i2++;
//            }
//        }
//
//        if(i2 >= render_ctx->frame_buffer_size - 1){
//            break;
//        }
//
//        render_ctx->frame_buffer[i2] = '\n';
//        i2++;
//    }
//
//    render_ctx->frame_buffer[i2] = '\0';
//}


void render_frame(BufferCtx *buffer, RenderCtx *render_ctx){
    memset(render_ctx->frame_buffer, ' ', render_ctx->frame_buffer_size);
    draw_buffer(buffer,render_ctx);
    render_lnumbers(buffer,render_ctx);


    printf("\x1b[H%*s",render_ctx->frame_buffer_size,render_ctx->frame_buffer);
    fflush(stdout);
}
