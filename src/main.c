#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bufferstuff/utils.h"
#include "includes/types.h"
#include "terminal/terminal.h"
#include "bufferstuff/buffer_ops.h"

typedef enum{
    INSERT,
    NORMAL
}Mode;

Mode mode = NORMAL;

void move_n_render(BufferCtx *buff,TermCtx terminal,void (*pos_change)(BufferCtx* buffer,int step)){
    // Takes in a function which changes position of a cursor within a buffer
    // Will have to take step argument eventually
    clear_screen();
    pos_change(buff,1);
    update_view_end(buff, terminal);
    draw_buffer(*buff);   
}

void normal_mode(char c, BufferCtx * buff,TermCtx terminal){
    TermPos a,b;
    switch (c) {
        case 'h':
            move_n_render(buff,terminal,move_buff_pos_left);
            a = translate_buff_pos_relative(*buff,terminal);
            move_cursor(a);
            break;
        case 'j':
            move_n_render(buff,terminal,move_buff_pos_down);
            a = translate_buff_pos_relative(*buff,terminal);
            b = translate_buff_pos_absolute(*buff);
            printf("\x1b[43;30H %d:%d",b.y,b.x);
            move_cursor(a);
            break;
        case 'k':
            move_n_render(buff,terminal,move_buff_pos_up);
            a = translate_buff_pos_relative(*buff,terminal);
            b = translate_buff_pos_absolute(*buff);
            printf("\x1b[43;30H %d:%d",b.y,b.x);
            move_cursor(a);
            break;
        case 'l':
            move_n_render(buff,terminal,move_buff_pos_right);
            a = translate_buff_pos_relative(*buff,terminal);
            move_cursor(a);
            break;
        case 'i':
            mode = INSERT;
            change_cursor_to_line();           
            break;
        case 'q':
            exit(0);
    }   
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
    //slice mem check
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

    if(buffer->slices_mem_filled > terminal.rows - 1 ){
        if(locate_slice(buffer->buff_pos,*buffer) > terminal.rows - 1){
            buffer->view.start += 1;
            update_view_end(buffer,terminal);
        }
    }else{
        buffer->view.end +=1;
    }
   
    buffer->slices_mem_filled += 1;
};


void insert_mode(char c,BufferCtx* buff,TermCtx terminal){
    switch (c) {
        case '\e':
            mode = NORMAL;
            change_cursor_to_block();
        case 8:
            //remove_from_buffer();
            break; 
        case '\n':
            insert_new_line(buff,terminal);
            draw_buffer(*buff);
            TermPos b = translate_buff_pos_relative(*buff,terminal);
            printf("\x1b[43;30H %d:%d",b.y,b.x);
            move_cursor(b);
            break;
        default:
            insert_into_buffer(c,buff);
            draw_buffer(*buff);
            TermPos a = translate_buff_pos_relative(*buff,terminal);
            printf("\x1b[43;30H %d:%d",a.y,a.x);
            move_cursor(a);
            break;
    }
}

int main(int argc, char **argv){
    assert(argc == 2);
    FILE * file = fopen(argv[1],"r");
    BufferCtx buff;
    TermCtx terminal = terminal_setup();
    build_buffer(&buff, file);
    update_view_end(&buff, terminal);
    draw_buffer(buff);
   
    reset_cursor();
    TermPos a,b;
    for(;;){
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            switch (mode) {
                case NORMAL:
                    normal_mode(c,&buff,terminal);
                    break;
                case INSERT:
                    insert_mode(c,&buff,terminal);
                    break;
            }
        }
    }
    return 0;
}
