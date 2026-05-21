#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

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
        .frame_buffer_size = (terminal.cols) * terminal.rows,
        .frame_buffer = (char *)malloc((terminal.cols) * terminal.rows),
        .margin = {0, 0, 0, 0},
        .redraw = 0,
        .status_bar = NULL
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
    return row * (render_ctx->terminal.cols);
}


void draw_buffer(BufferCtx *buffer, RenderCtx *render_ctx){
    int render_row = 0;

    for(int buffer_line_idx = buffer->view.start; buffer_line_idx <= buffer->view.end; buffer_line_idx++){
        if(render_row >= render_ctx->terminal.rows - render_ctx->margin.bottom){
            break;
        }

        int slice_start = get_slice_start(buffer_line_idx, *buffer);
        int line_len = buffer->slices[buffer_line_idx].len - 1;
        int row_start = translate_row2absolute(render_row, render_ctx);
        int render_line_idx = row_start + render_ctx->margin.left;

        for(int col = 0; col < buffer->logical_terminal.cols; col++){
            int buffer_pos = slice_start + buffer->col_offset + col;
            if(buffer_pos < slice_start + line_len){
                render_ctx->frame_buffer[render_line_idx++] = buffer->mem[buffer_pos];
            } else {
                break;
            }
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
        snprintf(tmp_buff, 20, "%d", slice + 1);

        int str_len = strlen(tmp_buff) - 1;
        for(int i = str_len; i >= 0; i--){
            render_ctx->frame_buffer[row_start + (margin - i - 1)] = tmp_buff[str_len - i];
        }

        render_row++;  
    }
};


void set_newlines(RenderCtx *render_ctx){
    for(int row = 0;row < render_ctx->terminal.rows;row++){
        if(row == render_ctx->terminal.rows - 1){
            return;
        }
        int row_start = translate_row2absolute(row, render_ctx);
        render_ctx->frame_buffer[row_start + render_ctx->terminal.cols] = '\n';
    }
}   


void render_status_bar(RenderCtx * render_ctx){
    if(!render_ctx->status_bar || !(render_ctx->margin.bottom > 0)){
        return;
    }
    char cursor_pos[20] = {0};
    char normal_mode[] = " NORMAL ";
    char insert_mode[] = " INSERT ";


    int render_row = render_ctx->terminal.rows - 1;
    int r_row_start = translate_row2absolute(render_row,render_ctx);
    if(render_ctx->status_bar->mode == NORMAL){   
        strncpy(&render_ctx->frame_buffer[r_row_start],normal_mode,8);
    }else{         
        strncpy(&render_ctx->frame_buffer[r_row_start],insert_mode,8);
    }
    sprintf(cursor_pos, "%d:%d", render_ctx->status_bar->buffer_pos.y, render_ctx->status_bar->buffer_pos.x);
    
    if(strlen(cursor_pos) + 8 + strlen(render_ctx->status_bar->open_fname) >  render_ctx->terminal.cols * 3/4 ){
        return;
    }
    strncpy(&render_ctx->frame_buffer[r_row_start + 10],render_ctx->status_bar->open_fname,strlen(render_ctx->status_bar->open_fname));
    strncpy(&render_ctx->frame_buffer[r_row_start + (render_ctx->terminal.cols) - 15],cursor_pos,strlen(cursor_pos));
}


void print_with_colors(RenderCtx *render_ctx) {
    printf("\x1b[?2026h");      
    printf("\x1b[H");  
    // Print row by row, applying colors to each section
    for (int row = 0; row < render_ctx->terminal.rows; row++) {

        int row_start = row * render_ctx->terminal.cols;

        // Last row is status bar (if margin.bottom > 0)
        if (row == render_ctx->terminal.rows - 1 && render_ctx->margin.bottom > 0) {
            // Status bar sections:
            // 0-7: Mode (purple bg)
            // 8-9: Space
            // 10+: Filename (cyan)
            // (cols-15)+: Position (yellow)

            // Mode section (purple background)
            printf("\x1b[1m\x1b[30m\x1b[45m");  // Bold black text on purple bg
            printf("%.*s", 8, &render_ctx->frame_buffer[row_start]);
            printf("\x1b[0m");  // Reset

            // Space between mode and filename
            printf("%.*s", 2, &render_ctx->frame_buffer[row_start + 8]);

            // Filename section (cyan)
            printf("\x1b[36m");  // Cyan
            int fname_len = render_ctx->terminal.cols - 15 - 10;  // Space between filename and position
            printf("%.*s", fname_len, &render_ctx->frame_buffer[row_start + 10]);
            printf("\x1b[0m");  // Reset

            // Position section (yellow)
            printf("\x1b[33m");  // Yellow
            printf("%.*s", 15, &render_ctx->frame_buffer[row_start + render_ctx->terminal.cols - 15]);
            printf("\x1b[0m");  // Reset
        } else {
            // Line numbers region (first margin.left columns)
            if (render_ctx->margin.left > 0) {
                printf("\x1b[34m");  // Blue
                printf("%.*s", render_ctx->margin.left, &render_ctx->frame_buffer[row_start]);
                printf("\x1b[0m");  // Reset
            }

            // Content region (rest of the row)
            int content_start = row_start + render_ctx->margin.left;
            int content_len = render_ctx->terminal.cols - render_ctx->margin.left;
            printf("%.*s", content_len, &render_ctx->frame_buffer[content_start]);
        }
    }
    printf("\x1b[?2026l");  // End synchronized update
    fflush(stdout);
}

void render_frame(BufferCtx *buffer, RenderCtx *render_ctx){
    memset(render_ctx->frame_buffer,' ', render_ctx->frame_buffer_size);

    draw_buffer(buffer, render_ctx);
    render_lnumbers(buffer, render_ctx);
    render_status_bar(render_ctx);

    print_with_colors(render_ctx);
    fflush(stdout);
}
