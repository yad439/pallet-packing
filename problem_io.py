from typing import Iterable, List, Tuple

from common import Item


def save_instance(path: str, pallet_width: int, pallet_height: int, items: Iterable[Item]) -> None:
    with open(path, 'w') as file:
        print(pallet_width, pallet_height, file=file, sep='\t')
        for item in items:
            if item.mass is None:
                print(item.width, item.height, file=file, sep='\t')
            else:
                print(item.width, item.height, item.mass, file=file, sep='\t')


def load_instance(path: str) -> Tuple[int, int, List[Item]]:
    with open(path) as file:
        pallet_width, pallet_height = map(int, file.readline().split('\t'))
        items = []
        for line in file:
            if not line or line.isspace():
                continue
            itm = list(map(int, line.split('\t')))
            items.append(Item(*itm))
        return pallet_width, pallet_height, items
