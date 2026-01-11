#include <stddef.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "types.h"

static struct termios orig_termios;
static int term_rows, term_cols;

void get_term_size(int *rows, int *cols) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void clear_screen(){
    printf("\x1b[2J");
    fflush(stdout);
}

void reset_cursor(){
    printf("\x1b[H");
    fflush(stdout);
}

void terminal_setup(){
    get_term_size(&term_rows,&term_cols);
    clear_screen();
    enable_raw_mode();
}

void draw_buffer(BufferCtx buffer){
    char out[10000] = {0};
    int i2 = 0;
    for(int i = buffer.view.start; i <= buffer.view.end;i++){
        for(int j = buffer.slices[i].start;j <= buffer.slices[i].end;j++){
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
