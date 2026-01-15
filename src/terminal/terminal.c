#include <stddef.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "../includes/types.h"
#include "../bufferstuff/utils.h"
#include "terminal.h"

static struct termios orig_termios;
static TermCtx terminal; 

void get_term_size(TermCtx * terminal) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    terminal->cols = ws.ws_col;
    terminal->rows = ws.ws_row;
}


void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void clear_screen(void){
    printf("\x1b[2J");
    fflush(stdout);
}


void reset_cursor(void){
    printf("\x1b[H");
    fflush(stdout);
}


void move_cursor(TermPos location){
    printf("\x1b[%d;%dH",location.y,location.x);
    fflush(stdout);
}


void change_cursor_to_line(void){
    printf("\x1b[6 q");
    fflush(stdout);
}


void change_cursor_to_block(void){
    printf("\x1b[0 q");
    fflush(stdout);
}


TermCtx terminal_setup(void){
    get_term_size(&terminal);
    clear_screen();
    enable_raw_mode();
    return terminal;
}


void draw_buffer(BufferCtx buffer){
    char out[10000] = {0};
    int i2 = 0;
    for(int i = buffer.view.start; i <= buffer.view.end;i++){
        int slice_start = get_slice_start(i, buffer);
        for(int j = slice_start;j < slice_start + buffer.slices[i].len;j++){
            out[i2]= buffer.mem[j];
            i2++;
        }
    }
    out[i2] = '\0';

    while(i2 > 0 && (out[i2-1] == '\n' || out[i2-1] == '\r')){
        i2--;
        out[i2] = '\0';
    }
    
    printf("\x1b[2J\x1b[H%s",out);
    fflush(stdout);      
}
