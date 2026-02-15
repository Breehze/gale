#include <stdlib.h>

#include "keymap_api.h"
#include "../includes/common_keymap.h"

handler call0(char*);
handler call1(char*);

handler call0(char * switcheroo){
   update_nest_count(0);
   switch(switcheroo[0]){
       case 'l': return &MOV_CURSOR_RIGHT;
       case 'k': return &MOV_CURSOR_UP;
       case 'j': return &MOV_CURSOR_DOWN;
       case 'h': return &MOV_CURSOR_LEFT;
       case 'w': return &MOV_CURSOR_NEXT_WORD;
       case 'b': return &MOV_CURSOR_PREVIOUS_WORD;
       case 'q': return &EXIT;
       case 'i': return &INSERT_MODE;
       case ':': return call1(switcheroo);
       default: return NULL;
   }
}
handler call1(char * switcheroo){
   update_nest_count(1);
   switch(switcheroo[1]){
       case 'w': return &SAVE_BUFFER;
       default: return NULL;
   }
}
