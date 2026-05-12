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
        .frame_buffer_size = terminal.cols * (terminal.rows + 1) + 1 ,
        .frame_buffer = (char *)malloc(sizeof(char) * (terminal.rows + 1) * terminal.cols + 1),
        .redraw = 0,
        .margin_left = 0
    };
    if(!new_ctx->frame_buffer){
        free(new_ctx);
        return NULL;
    }
     return new_ctx;
};

void draw_buffer(BufferCtx *buffer, RenderCtx *render_ctx){
    memset(render_ctx->frame_buffer, ' ', render_ctx->frame_buffer_size);
    int i2 = 0;

    for(int i = buffer->view.start; i <= buffer->view.end; i++){
        int slice_start = get_slice_start(i, *buffer);
        int rows_taken = (buffer->slices[i].len / render_ctx->terminal.cols) + 1;

        for(int j = slice_start; j < slice_start + (render_ctx->terminal.cols * rows_taken); j++){
            if(i2 >= render_ctx->frame_buffer_size - 1){
                break;
            }

            if(j < slice_start + buffer->slices[i].len - 1){
                render_ctx->frame_buffer[i2] = buffer->mem[j];
                i2++;
            }else{
                render_ctx->frame_buffer[i2] = ' ';
                i2++;
            }
        }

        if(i2 >= render_ctx->frame_buffer_size - 1){
            break;
        }

        render_ctx->frame_buffer[i2] = '\n';
        i2++;
    }

    render_ctx->frame_buffer[i2] = '\0';
}

void render_frame(BufferCtx *buffer, RenderCtx *render_ctx){
    int digits = 0;
    for(int i = 1; (buffer->slices->len / (1 * i)) != 0 ;i*=10,digits++);
    render_ctx->margin_left = digits;

    draw_buffer(buffer,render_ctx);


    printf("\x1b[H%s", render_ctx->frame_buffer);
    fflush(stdout);
}
