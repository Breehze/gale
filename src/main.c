#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "includes/types.h"
#include "includes/common_keymap.h"
#include "terminal/terminal.h"
#include "bufferstuff/buffer_ops.h"

Mode mode = NORMAL;

char combo_buff[100] = {0};
int combo_buff_index = 0;

void normal_mode(char * sequence, BufferCtx * buff,RenderCtx *render_ctx){
    int view_start_old = buff->view.start;
    int view_end_old = buff->view.end;
    
    handler handler = call0(sequence);
    WrappedInput handler_input = (WrappedInput){.buff = buff,.term = render_ctx->terminal,.mode = &mode};
    
    if(handler){    
        handler(&handler_input,NULL);
    }   

    if(handler || (get_nest_count() < combo_buff_index)){
        memset(combo_buff,0,sizeof(combo_buff));
        combo_buff_index = 0;
    }
        
    if(view_start_old != buff->view.start || view_end_old != buff->view.end){
        render_frame(buff, render_ctx);
    }

    if(render_ctx->status_bar){
        SBAR_update(render_ctx->status_bar,translate_buff_pos_absolute(*buff),buff->fpath,mode);
        render_frame(buff, render_ctx);
    }

    if(mode == INSERT){
        change_cursor_to_line();
    }
    
    TermPos a = translate_buff_pos_relative(*buff,render_ctx->terminal);
    a.x += render_ctx->margin.left;
    a.y += render_ctx->margin.top;
    move_cursor(a);

}


void insert_mode(char c,BufferCtx* buff,RenderCtx * render_ctx){
    TermPos a;
    switch (c) {
        case '\e':
            mode = NORMAL;
            change_cursor_to_block();
            break;
        case 127:
            remove_from_buffer(buff);
            update_view_end(0,buff);
            break;
        case '\n':
            insert_new_line(buff);
            break;
        case 9:
            for(int i = 0;i < 4;i++){
                insert_into_buffer(' ',buff);
            }
            break;
        default:
            insert_into_buffer(c,buff);
            break;
    }
    render_frame(buff,render_ctx);
    if(render_ctx->status_bar){
        SBAR_update(render_ctx->status_bar,translate_buff_pos_absolute(*buff),buff->fpath,mode);
        render_frame(buff, render_ctx);
    }
    a = translate_buff_pos_relative(*buff,render_ctx->terminal);
    a.x += render_ctx->margin.left;
    a.y += render_ctx->margin.top;
    move_cursor(a);
}


int main(int argc, char **argv){
    assert(argc == 2);
    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(STDIN_FILENO, &descriptors);

    BufferCtx buff;
    RenderCtx *render_ctx;
    
    
    TermCtx terminal = terminal_setup();
    render_ctx = init_render_ctx(terminal);

    build_buffer(&buff,argv[1]);
    

    render_ctx->margin.left = calculate_line_margin(&buff) + 1;
    render_ctx->margin.bottom = 1;
    update_logical_terminal(&buff, calculate_content_size(render_ctx));
    update_view_end(0, &buff);
    
    StatusBar bar;
    render_ctx->status_bar = &bar;
    SBAR_update(render_ctx->status_bar,translate_buff_pos_absolute(buff),buff.fpath, NORMAL);
    
    render_frame(&buff,render_ctx);
    
    move_cursor((TermPos){.x = 1 + render_ctx->margin.left ,.y = 1 + render_ctx->margin.right});
    for(;;){
        int ready = select(STDIN_FILENO + 1, &descriptors, NULL, NULL, NULL);
        
        if(!ready){ continue; }

        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            switch (mode) {
                case NORMAL:
                    combo_buff[combo_buff_index++] = c;
                    normal_mode(combo_buff,&buff,render_ctx);
                    break;
                case INSERT:
                    insert_mode(c,&buff,render_ctx);
                    break;
            }
        }
    }
    return 0;
}
