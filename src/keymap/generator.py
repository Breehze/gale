from typing import Tuple,List
import os
import hashlib

def hash(to_hash : str) -> str:
    hash_obj = hashlib.md5(to_hash.encode())  
    full_hash = hash_obj.hexdigest()       

    short_hash = hash_obj.hexdigest()[:8]
    return short_hash



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


def create_switch_function(state_machine_layer : List[dict],nest : int,result : str,hashes_created : List[str]) -> Tuple[str,int,list]:
    if(not len(state_machine_layer)):
        return result,nest,hashes_created
    next_layer = []
    for branch in state_machine_layer:
        states = branch.items()
        
        function = f"handler call{hash(str(branch)) if nest else 0}(char * switcheroo){{\n   update_nest_count({nest});\n   switch(switcheroo[{nest}]){{\n"
        for state,next_state in branch.items():
            j = 0
            if state == "\n":
                state =  r"\n"
            if type(next_state) == type(dict()):
                new_hash = hash(str(next_state)) 
                case = f"       case '{state}': return call{new_hash}(switcheroo);\n"
                hashes_created.append(new_hash)
                next_layer.append(next_state)
            else:
                case = f"       case '{state}': return &{next_state};\n"    
            j+=1
            function += case;
        function += "       default: return NULL;\n   }\n}\n"
        result += function
    return create_switch_function(next_layer,nest + 1,result,hashes_created)


def generate_keymap(keymap:dict):
    content = "#include <stdlib.h>\n\n#include \"keymap_api.h\"\n#include \"../includes/common_keymap.h\"\n\n"
    
    state_machines = dict()
    for key,val in keymap.items():
        nested_insert(key,val,state_machines)
    
    e : List[dict] = [state_machines]
    switch_cases,forward_declarations,hashes = create_switch_function(e,0,"",[])

    for i in hashes:
        content += f"handler call{i}(char*);\n"
    content += "\n"
    content += switch_cases

    script_dir = os.path.dirname(os.path.abspath(__file__))
    with open(os.path.join(script_dir, "fsm.c"),"w") as out:
        out.write(content)
