from ctypes import POINTER, Structure, c_bool, c_double, c_int, cdll
from typing import Callable, List, Optional, Tuple

from common import Item, Position


class CItem(Structure):
    _fields_ = [('width', c_int), ('height', c_int)]


class CPosition(Structure):
    _fields_ = [('placed', c_bool), ('x', c_double), ('y', c_double), ('rotated', c_bool)]


class Library:
    def __init__(self, path: str) -> None:
        self.inner = cdll.LoadLibrary(path)

    def get_function(self, name: str) -> Callable[[int, int, List[Item]], Tuple[int, List[Optional[Position]]]]:
        func = getattr(self.inner, name)
        func.restype = c_int
        func.argtypes = [c_int, c_int, c_int, POINTER(CItem), POINTER(CPosition)]

        def call(pallet_width: int, pallet_height: int, items: List[Item]) -> Tuple[int, List[Optional[Position]]]:
            n = len(items)
            c_items = (CItem * n)(*map(lambda it: CItem(it.width, it.height), items))
            c_positions = (CPosition * n)()
            result = func(pallet_width, pallet_height, n, c_items, c_positions)
            positions = list(map(lambda p: Position(p.x, p.y, p.rotated) if p.placed else None, c_positions))
            return result, positions

        return call
