#include <stdio.h>

#include "../includes/types.h"

void SBAR_update(StatusBar * bar,TermPos buffer_pos,char * open_fname,Mode mode){
    bar->buffer_pos = buffer_pos ;
    bar->open_fname = open_fname;
    bar->mode = mode;
}
