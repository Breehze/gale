from typing import Tuple
import os

def nested_insert(sequence : str,handler_name : str,nest_here : dict) -> None:
    """Creates a state machine representation of keymap"""
    next_nest = nest_here
    for char in sequence:
        if next_nest.get(char):
            nested_insert(sequence[1:],handler_name,next_nest[char])
            return
        next_nest.update({char: {}})
        next_nest = next_nest[char]
    
    next_dict = nest_here
    for ind,char in enumerate(sequence):
        if ind == len(sequence)-1 :
            break
        next_dict = next_dict[char] 
    
    next_dict.update({sequence[-1] : handler_name})


def create_switch_function(state_machine_layer : dict,nest:int,result : str) -> Tuple[str,int]:
    if not len(state_machine_layer):
        return result,nest
    next_layer = {}
    function = f"handler call{nest}(char * switcheroo){{\n   update_nest_count({nest});\n   switch(switcheroo[{nest}]){{\n"
    for state,next_state in state_machine_layer.items():
        if type(next_state) == type(dict()):
            case = f"       case '{state}': return call{nest+1}(switcheroo);\n"
            next_layer.update(next_state)
        else:
            case = f"       case '{state}': return &{next_state};\n"    
        function += case;
    function += "       default: return NULL;\n   }\n}\n"
    result += function
    return create_switch_function(next_layer,nest + 1,result)


def generate_keymap(keymap:dict):
    content = "#include <stdlib.h>\n\n#include \"keymap_api.h\"\n#include \"../includes/common_keymap.h\"\n\n"
    
    state_machines = dict()
    for key,val in keymap.items():
        nested_insert(key,val,state_machines)
    
    switch_cases,forward_declarations = create_switch_function(state_machines,0,"")

    for i in range(forward_declarations):
        content += f"handler call{i}(char*);\n"
    content += "\n"
    content += switch_cases

    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    with open(os.path.join(script_dir, "fsm.c"),"w") as out:
        out.write(content)
