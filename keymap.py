from pprint import pprint
from src.keymap.generator import generate_keymap
from typing import Tuple

keymap = {
    "l" : "MOV_CURSOR_RIGHT",
    "k" : "MOV_CURSOR_UP",
    "j" : "MOV_CURSOR_DOWN",
    "h" : "MOV_CURSOR_LEFT",
    "q" : "EXIT"
}


generate_keymap(keymap)
print("Keymap generated successfuly!")
