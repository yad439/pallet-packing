import os
import subprocess
from typing import List, Optional, Tuple, Union

from c_interop import EXE_FILE
from common import Item, Position


def nfd(pallet_width: int, pallet_height: int, items: List[Item]) -> Tuple[int, List[Optional[Position]]]:
    return _run_external_program(pallet_width, pallet_height, items, EXE_FILE,
                                 f'{pallet_width} {pallet_height} NFD tmp/input.txt')


def simulated_annealing(pallet_width: int, pallet_height: int, items: List[Item], cooling_coef: float,
                        num_of_iters: int) -> Tuple[int, List[Optional[Position]]]:
    return _run_external_program(pallet_width, pallet_height, items, EXE_FILE,
                                 f'{pallet_width} {pallet_height} SA {cooling_coef} {num_of_iters} tmp/input.txt')


def _run_external_program(pallet_width: int, pallet_height: int, items: List[Item], args: Union[str, List[str]],
                          input_str: str) -> Tuple[int, List[Optional[Position]]]:
    if not os.path.isdir('tmp'):
        os.mkdir('tmp')
    with open('tmp/input.txt', 'w') as file:
        for item in items:
            print(item.width, item.height, file=file)
    output = subprocess.run(args, input=input_str, capture_output=True, text=True).stdout
    sizes_str, positions_str = output.split('|')

    packed = []
    for line in sizes_str.split('\n'):
        if not line or line.isspace():
            continue
        w, h = map(int, line.split(' '))
        packed.append((w, h))
    positions = []
    for line in positions_str.split('\n'):
        if not line or line.isspace():
            continue
        x, y = map(int, line.split(' '))
        positions.append((x, y))

    results: List[Optional[Position]] = [None for _ in range(len(items))]
    for i in range(len(items)):
        for j in range(len(packed)):
            if packed[j][0] == items[i].width and packed[j][1] == items[i].height:
                results[i] = Position(positions[j][0], positions[j][1], False)
                packed.pop(j)
                positions.pop(j)
                break
            elif packed[j][1] == items[i].width and packed[j][0] == items[i].height:
                results[i] = Position(positions[j][0], positions[j][1], True)
                packed.pop(j)
                positions.pop(j)
                break

    area = pallet_width * pallet_height
    for i in range(len(items)):
        if results[i] is not None:
            area -= items[i].width * items[i].height
    return area, results
