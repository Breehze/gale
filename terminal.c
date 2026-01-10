#include <stddef.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

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
    clear_screen();
    enable_raw_mode();
}
