from ctypes import POINTER, Structure, c_bool, c_double, c_int, c_uint, cdll
from math import log
from typing import Callable, List, Optional, Tuple

from common import Item, Position


class CItem(Structure):
    _fields_ = [('width', c_int), ('height', c_int)]


class CPosition(Structure):
    _fields_ = [('placed', c_bool), ('x', c_double), ('y', c_double), ('rotated', c_bool)]


class Library:
    def __init__(self, path: str) -> None:
        self._inner = cdll.LoadLibrary(path)
        self.__init_annealing()

    def get_function(self, name: str) -> Callable[[int, int, List[Item]], Tuple[int, List[Optional[Position]]]]:
        func = getattr(self._inner, name)
        return self._handle_default_args(func, [])

    def __init_annealing(self):
        func = self._inner.simulated_annealing_skyline
        defaulted = self._handle_default_args(func, [c_uint, c_uint, c_double, c_double])

        def call(pallet_width: int, pallet_height: int, items: List[Item], steps: int = 100_000,
                 same_temperature_steps: int = 1, start_temperature: Optional[float] = None,
                 power: Optional[float] = None) -> Tuple[int, List[Optional[Position]]]:
            if start_temperature is None:
                start_temperature = pallet_width * pallet_height / 2
            if power is None:
                power = (-start_temperature * log(1e-3)) ** (-1 / steps)

            return defaulted(pallet_width, pallet_height, items, steps, same_temperature_steps, start_temperature,
                             power)

        self.simulated_annealing = call

    @staticmethod
    def _handle_default_args(func, types: list):
        func.restype = c_int
        func.argtypes = [c_int, c_int, c_uint, POINTER(CItem), POINTER(CPosition)] + types

        def _call(pallet_width: int, pallet_height: int, items: List[Item], *args) -> Tuple[
            int, List[Optional[Position]]]:
            n = len(items)
            c_items = (CItem * n)(*map(lambda it: CItem(it.width, it.height), items))
            c_positions = (CPosition * n)()
            result = func(pallet_width, pallet_height, n, c_items, c_positions, *args)
            positions = list(map(lambda p: Position(p.x, p.y, p.rotated) if p.placed else None, c_positions))
            return result, positions

        return _call
