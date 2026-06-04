from pprint import pprint
from src.keymap.generator import generate_keymap,nested_insert
from typing import Tuple

keymap = {
    "l" : "MOV_CURSOR_RIGHT",
    "k" : "MOV_CURSOR_UP",
    "j" : "MOV_CURSOR_DOWN",
    "h" : "MOV_CURSOR_LEFT",
    "w" : "MOV_CURSOR_NEXT_WORD",
    "$" : "MOV_CURSOR_EOL",
    "b" : "MOV_CURSOR_PREVIOUS_WORD",
    "tt" : "CENTER_VIEW_AROUND_CURSOR_H",
    "zz" : "CENTER_VIEW_AROUND_CURSOR_V",
    ":q\n" : "EXIT",
    "i" : "INSERT_MODE",
    "a" : "INSERT_MODE_APPEND",
    ":w\n" : "SAVE_BUFFER"
}


generate_keymap(keymap)
print("Keymap generated successfuly!")
