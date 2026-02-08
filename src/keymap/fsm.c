#include <stdlib.h>

#include "keymap_api.h"

handler call0(char*);

handler call0(char * switcheroo){
   switch(switcheroo[0]){
       case 'l': return &MOV_CURSOR_RIGHT;
       case 'k': return &MOV_CURSOR_UP;
       case 'j': return &MOV_CURSOR_DOWN;
       case 'h': return &MOV_CURSOR_LEFT;
       case 'q': return &EXIT;
       default: return NULL;
   }
}
