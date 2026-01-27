#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bufferstuff/utils.h"
#include "includes/types.h"
#include "terminal/terminal.h"
#include "bufferstuff/buffer_ops.h"

Mode mode = NORMAL;

void move_n_render(BufferCtx *buff,TermCtx terminal,void (*pos_change)(BufferCtx* buffer,int step)){
    // Takes in a function which changes position of a cursor within a buffer
    // Will have to take step argument eventually
        
    int view_start_old = buff->view.start;
    int view_end_old = buff->view.end;
    pos_change(buff,1);
    update_view_end(buff, terminal);
    if(view_start_old != buff->view.start || view_end_old != buff->view.end){
        draw_buffer(*buff);   
    }
}


void normal_mode(char c, BufferCtx * buff,TermCtx terminal,StatusBar * status_bar){
    TermPos a;
    switch (c) {
        case 'h':
            move_n_render(buff,terminal,move_buff_pos_left);
            break;
        case 'j':
            move_n_render(buff,terminal,move_buff_pos_down);
            break;
        case 'k':
            move_n_render(buff,terminal,move_buff_pos_up);
            break;
        case 'l':
            move_n_render(buff,terminal,move_buff_pos_right);
            break;
        case 'i':
            mode = INSERT;
            change_cursor_to_line();           
            break;
        case 'w':
            move_n_render(buff,terminal,jump_next_word);
            break;
        case 'b':
            move_n_render(buff,terminal,jump_previous_word);
            break;
        case 's':
            save_buffer(*buff);
            a = translate_buff_pos_relative(*buff,terminal);
            printf("\x1b[%d;15H written to \"%s\"",status_bar->pos.y,buff->fpath);
            move_cursor(a);
            break;
        case 'q':
            clear_screen();
            exit(0);
    }   
    if(status_bar){
        SBAR_update(status_bar,translate_buff_pos_absolute(*buff),buff->fpath,mode);
        SBAR_draw(*status_bar);
    }
        
    a = translate_buff_pos_relative(*buff,terminal);
    move_cursor(a);
}


void insert_mode(char c,BufferCtx* buff,TermCtx terminal,StatusBar * status_bar){
    TermPos a;
    switch (c) {
        case '\e':
            mode = NORMAL;
            change_cursor_to_block();
            break;
        case 127:
            remove_from_buffer(buff);
            break;
        case '\n':
            insert_new_line(buff,terminal);
            break;
        case 9:
            for(int i = 0;i < 4;i++){
                insert_into_buffer(' ',buff);
            }
            break;
        default:
            insert_into_buffer(c,buff);
            break;
    }
    draw_buffer(*buff);
    if(status_bar){
        SBAR_update(status_bar,translate_buff_pos_absolute(*buff),buff->fpath,mode);
        SBAR_draw(*status_bar);
    }
    a = translate_buff_pos_relative(*buff,terminal);
    move_cursor(a);
}


int main(int argc, char **argv){
    assert(argc == 2);
    BufferCtx buff;
    TermCtx terminal = terminal_setup();
    StatusBar bar = (StatusBar){
        .mode = NORMAL,
        .pos = (TermPos){.x = terminal.cols ,.y = terminal.rows},
        .open_fname = buff.fpath,
        .buffer_pos = (TermPos){.x = 1,.y = 1}
    };
    terminal.rows -= 1;
    build_buffer(&buff,argv[1] );
    update_view_end(&buff, terminal);
    draw_buffer(buff);
    SBAR_draw(bar);
   
    reset_cursor();
    for(;;){
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            switch (mode) {
                case NORMAL:
                    normal_mode(c,&buff,terminal,&bar);
                    break;
                case INSERT:
                    insert_mode(c,&buff,terminal,&bar);
                    break;
            }
        }
    }
    return 0;
}
