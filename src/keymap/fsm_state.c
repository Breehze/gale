static int nest_count = 0;

void update_nest_count(int nest){
    nest_count = nest;
}

int get_nest_count(){
    return nest_count;
}
