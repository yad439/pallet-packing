from typing import List, Optional, Tuple

from common import Item, Position
from concat_baseline.concat_baseline import Rectangle, assign_coordinates, pallet_placement, rects_flatten


def concat_placement(pallet_width: int, pallet_height: int, items: List[Item]) -> Tuple[int, List[Optional[Position]]]:
    rects = [Rectangle(item.width, item.height)
             for item in items
             ]

    pallet = Rectangle(pallet_width, pallet_height, [], (pallet_width, pallet_height))

    rest_rects = pallet_placement(pallet, rects)
    assert len(rest_rects) + len(rects_flatten(pallet.placed)) == len(rects)

    solve_xywh = assign_coordinates(0, 0, pallet.w, pallet.h, pallet.placed)

    results = [None] * len(items)
    for i, item in enumerate(items):
        for j, (x, y, w, h) in enumerate(solve_xywh):
            if item.width == w and item.height == h:
                results[i] = Position(x, y, False)
                del solve_xywh[j]
                break
            if item.width == h and item.height == w:
                results[i] = Position(x, y, True)
                del solve_xywh[j]
                break
    assert not solve_xywh, f'solve_xywh={solve_xywh}'

    return pallet.free_square, results
