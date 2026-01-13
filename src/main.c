#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "includes/types.h"
#include "terminal/terminal.h"
#include "bufferstuff/buffer_ops.h"


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
                case 'q':
                    return 0; 
                    break;
            }
        }
    }
    return 0;
}
