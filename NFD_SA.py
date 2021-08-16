import subprocess
from typing import Union, List, Tuple, Optional


class Item:
    def __init__(self, width: int, height: int, mass: Union[int, None] = None):
        self.width = width
        self.height = height
        self.mass = mass

    def __repr__(self) -> str:
        return f'Item({repr(self.width)}, {repr(self.height)}, {repr(self.mass)})'

    def __str__(self) -> str:
        return f'Item({str(self.width)}, {str(self.height)}, {str(self.mass)})'


class Position:
    def __init__(self, x: Union[int, float], y: Union[int, float], rotated: bool):
        self.x = x
        self.y = y
        self.rotated = rotated

    def __repr__(self) -> str:
        return f'Position({repr(self.x)}, {repr(self.y)}, {repr(self.rotated)})'

    def __str__(self) -> str:
        return f'Position({str(self.x)}, {str(self.y)}, {str(self.rotated)})'


def nfd(pallet_width: int, pallet_height: int, items: List[Item]) -> Tuple[int, List[Optional[Position]]]:
    with open('tmp/input.txt', 'w') as file:
        for item in items:
            print(item.width, item.height, file=file)
    output = subprocess.run("bin/workshop.exe", input=f"{pallet_width} {pallet_height} NFD tmp/input.txt",
                            capture_output=True, text=True).stdout
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

    area=pallet_width*pallet_height
    for i in range(len(items)):
        if results[i] is not None:
            area-=items[i].width*items[i].height
    return area, results

def simulated_annealing(pallet_width: int, pallet_height: int, cooling_coef : float, num_of_iters : int, items: List[Item]) -> Tuple[int, float, int, List[Optional[Position]]]:
    with open('tmp/input.txt', 'w') as file:
        for item in items:
            print(item.width, item.height, file=file)
    output = subprocess.run("bin/workshop.exe", input=f"{pallet_width} {pallet_height} SA {cooling_coef} {num_of_iters} tmp/input.txt",
                            capture_output=True, text=True).stdout
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

    area=pallet_width*pallet_height
    for i in range(len(items)):
        if results[i] is not None:
            area-=items[i].width*items[i].height
    return area, results

items = [
    Item(1, 3),
    Item(2, 1),
    Item(4, 2),
    Item(3, 4),
    Item(2, 3),
    Item(3, 3),
    Item(3, 3),
    Item(1, 4),
    Item(4, 1),
    Item(1, 4),
    Item(2, 4),
    Item(2, 2),
    Item(4, 3),
    Item(1, 3),
    Item(3, 4)
]

res = nfd(10, 10, items)
res = simulated_annealing(10, 10, 0.99, 1000000, items)
print(res[0])
print(res[1])