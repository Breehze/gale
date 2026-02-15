from pprint import pprint
from src.keymap.generator import generate_keymap
from typing import Tuple

keymap = {
    "l" : "MOV_CURSOR_RIGHT",
    "k" : "MOV_CURSOR_UP",
    "j" : "MOV_CURSOR_DOWN",
    "h" : "MOV_CURSOR_LEFT",
    "w" : "MOV_CURSOR_NEXT_WORD",
    "b" : "MOV_CURSOR_PREVIOUS_WORD",
    "q" : "EXIT",
    "i" : "INSERT_MODE",
    ":w" : "SAVE_BUFFER"
}


generate_keymap(keymap)
print("Keymap generated successfuly!")
