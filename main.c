#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "terminal.c"
#include "types.h"

int locate_slice(int position,BufferCtx buffer){
    int i = 0;
    for(i = 0;i < buffer.slices_len;i++){
        if(position >=buffer.slices[i].start && position <= buffer.slices[i].end){
            break;
        }
    }
    return i;
}

void move_buff_pos_up(BufferCtx* buffer,int step){ 
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice - step < 0){
        return;
    }
    
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice-1].len){
        x_offset = buffer->slices[slice-1].len;
    }
    buffer->buff_pos = buffer->slices[slice-1].start + x_offset;
    if(slice <= buffer->view.start){
        buffer->view.start -= 1;
    }
}

void move_buff_pos_down(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    if(slice + step >= buffer->slices_len){
        return;
    }
    
    int x_offset = buffer->buff_pos - buffer->slices[slice].start;
    
    if(x_offset > buffer->slices[slice+1].len){
        x_offset = buffer->slices[slice+1].len;
    }
    buffer->buff_pos = buffer->slices[slice+1].start + x_offset;
    if(slice >= buffer->view.end){
        buffer->view.start += 1;
    }
}

void move_buff_pos_left(BufferCtx* buffer,int step){
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos - step;
    buffer->buff_pos = (new_pos < buffer->slices[slice].start) ? buffer->slices[slice].start : new_pos;
}

void move_buff_pos_right(BufferCtx* buffer, int step){  
    int slice = locate_slice(buffer->buff_pos,*buffer);
    int new_pos = buffer->buff_pos + step;
    buffer->buff_pos = (new_pos > buffer->slices[slice].end) ? buffer->slices[slice].end : new_pos;
}

void update_view_end(BufferCtx* buffer,TermCtx terminal){
    int lines_loaded = 0;
    int i = buffer->view.start;
    while(lines_loaded < terminal.rows && i < buffer->slices_len){
        int slice_lines = (buffer->slices[i].len+terminal.cols) / terminal.cols;
        if(lines_loaded + slice_lines  > terminal.rows){
            break;
        }
        
        lines_loaded += slice_lines; 
        i++;
    }
    buffer->view.end = i-1;
}

TermPos translate_buff_pos_absolute(BufferCtx buffer){
    int slice = locate_slice(buffer.buff_pos,buffer);
    return (TermPos){
        .x = buffer.buff_pos - buffer.slices[slice].start+1,
        .y = slice + 1 
    };
};

TermPos translate_buff_pos_relative(BufferCtx buffer, TermCtx terminal){
    int slice = locate_slice(buffer.buff_pos, buffer);
    
    int offset_in_line = buffer.buff_pos - buffer.slices[slice].start;
    
    int how_many_wrapped_rows = offset_in_line / terminal.cols;  
    int column_in_wrapped_row = offset_in_line % terminal.cols;  
    
    int screen_row_for_line_start = 0;
    
    for (int i = buffer.view.start; i < slice; i++) {
        int chars_in_line = buffer.slices[i].len;
        int rows_needed = (chars_in_line + terminal.cols ) / terminal.cols;
        screen_row_for_line_start += rows_needed;
    }
    
    int final_screen_row = screen_row_for_line_start + how_many_wrapped_rows;
    
    return (TermPos){
        .x = column_in_wrapped_row + 1,  
        .y = final_screen_row + 1        
    };
}

int build_buffer(BufferCtx* buffer,FILE* file){
    buffer->mem_len = 100;
    buffer->mem = (char *)malloc(sizeof(char) * buffer->mem_len);
    buffer->buff_pos = 0;
    
    buffer->slices_len = 0;
    buffer->slices = (Slice *)malloc(sizeof(Slice) * 1000);

    char c;
    
    int slice_start = 0;
    int i = 0;
    while ((c = fgetc(file)) != EOF) {
        buffer->mem[i] = c;
        if(c == '\n'){
            buffer->slices[buffer->slices_len] = (Slice){
                .start = slice_start,
                .end = i,
                .len = i - slice_start
            };
            buffer->slices_len++; 
            slice_start = i+1;
        }
        i++;
        if(i > buffer->mem_len){
            buffer->mem_len *= 10;
            char * new_mem_block = (char *)realloc(buffer->mem,buffer->mem_len);
            buffer->mem = new_mem_block;
        }
    }
    buffer->view.start = 0;
    return 1;
}


int main(int argc, char **argv){
    assert(argc == 2);
    FILE * file = fopen(argv[1],"r");
    BufferCtx buff;
    terminal_setup();
    build_buffer(&buff, file);
    update_view_end(&buff, terminal);
    printf("%d %d",buff.view.start,buff.view.end);
    draw_buffer(buff);
    
    reset_cursor();
    TermPos a;
    TermPos b;
    for(;;){
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            switch (c) {
                case 'h':
                    move_buff_pos_left(&buff,1); 
                    a = translate_buff_pos_relative(buff,terminal);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'j':
                    clear_screen();
                    move_buff_pos_down(&buff,1);
                    update_view_end(&buff, terminal);
                    draw_buffer(buff);
                    a = translate_buff_pos_relative(buff,terminal);
                    b = translate_buff_pos_absolute(buff);
                    printf("\x1b[43;30H %d:%d",b.y,b.x);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'k':
                    clear_screen();
                    move_buff_pos_up(&buff,1);
                    update_view_end(&buff, terminal);
                    draw_buffer(buff);
                    a = translate_buff_pos_relative(buff,terminal);
                    b = translate_buff_pos_absolute(buff);
                    printf("\x1b[43;30H %d:%d",b.y,b.x);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
                case 'l':
                    move_buff_pos_right(&buff,1);
                    a = translate_buff_pos_relative(buff,terminal);
                    printf("\x1b[%d;%dH",a.y,a.x);
                    fflush(stdout);
                    break;
            }
        }
    }
    return 0;
}
