from pprint import pprint
from src.keymap.generator import generate_keymap
from typing import Tuple

keymap = {
    "viw" : "SELECT_WORD",
    "vid" : "IDK",
    "j" : "MOV_CURSOR_DOWN",
    "h" : "MOV_CURSOR_LEFT"
}


generate_keymap(keymap)
print("Keymap generated successfuly!")
