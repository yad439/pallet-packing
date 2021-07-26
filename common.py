from typing import Union


class Item:
    def __init__(self, width: int, height: int, mass: Union[int, None] = None):
        self.width = width
        self.height = height
        self.mass = mass


class Position:
    def __init__(self, x: Union[int, float], y: Union[int, float], rotated: bool):
        self.x = x
        self.y = y
        self.rotated = rotated
