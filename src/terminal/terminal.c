#include <stddef.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

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

void reset_terminal() {
    printf("\x1b[?1049l");  
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

TermCtx terminal_setup(void){
    get_term_size(&terminal);
    printf("\x1b[?1049h");  
    fflush(stdout);
    clear_screen();
    enable_raw_mode();
    atexit(reset_terminal);
    return terminal;
}
