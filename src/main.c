#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

void insert_mode(char c,BufferCtx* buff,TermCtx terminal){
    switch (c) {
        case '\e':
            mode = NORMAL;
            change_cursor_to_block();
        default:
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
    printf("%d %d",buff.view.start,buff.view.end);
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
