from pprint import pprint
from src.keymap.generator import generate_keymap,nested_insert
from typing import Tuple

keymap = {
    "l" : "MOV_CURSOR_RIGHT",
    "k" : "MOV_CURSOR_UP",
    "j" : "MOV_CURSOR_DOWN",
    "h" : "MOV_CURSOR_LEFT",
    "w" : "MOV_CURSOR_NEXT_WORD",
    "bd" : "MOV_CURSOR_PREVIOUS_WORD",
    ":q\n" : "EXIT",
    "i" : "INSERT_MODE",
    ":w\n" : "SAVE_BUFFER"
}


generate_keymap(keymap)
print("Keymap generated successfuly!")
