#ifndef BUFFER_UTILS
#define BUFFER_UTILS
#include "../includes/types.h"

int locate_slice(int position,BufferCtx buffer);

int get_slice_start(int slice_indx, BufferCtx buffer);

int init_buffer_on_empty_file(BufferCtx* buffer);

#endif 

