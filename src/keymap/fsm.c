#include <stdlib.h>

#include "keymap_api.h"
#include "../includes/common_keymap.h"

handler call7002c394(char*);
handler call269d9c8e(char*);
handler call5df455b1(char*);
handler calleaf3f44a(char*);

handler call0(char * switcheroo){
   update_nest_count(0);
   switch(switcheroo[0]){
       case 'l': return &MOV_CURSOR_RIGHT;
       case 'k': return &MOV_CURSOR_UP;
       case 'j': return &MOV_CURSOR_DOWN;
       case 'h': return &MOV_CURSOR_LEFT;
       case 'w': return &MOV_CURSOR_NEXT_WORD;
       case 'b': return call7002c394(switcheroo);
       case ':': return call269d9c8e(switcheroo);
       case 'i': return &INSERT_MODE;
       default: return NULL;
   }
}
handler call7002c394(char * switcheroo){
   update_nest_count(1);
   switch(switcheroo[1]){
       case 'd': return &MOV_CURSOR_PREVIOUS_WORD;
       default: return NULL;
   }
}
handler call269d9c8e(char * switcheroo){
   update_nest_count(1);
   switch(switcheroo[1]){
       case 'q': return call5df455b1(switcheroo);
       case 'w': return calleaf3f44a(switcheroo);
       default: return NULL;
   }
}
handler call5df455b1(char * switcheroo){
   update_nest_count(2);
   switch(switcheroo[2]){
       case '\n': return &EXIT;
       default: return NULL;
   }
}
handler calleaf3f44a(char * switcheroo){
   update_nest_count(2);
   switch(switcheroo[2]){
       case '\n': return &SAVE_BUFFER;
       default: return NULL;
   }
}
