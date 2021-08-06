from typing import Iterable, List, Optional, Tuple

from common import Item, Position
from structures import LinkedList


def skyline_decode(pallet_width: int, pallet_height: int, items: List[Item],
                   permutation: Optional[Iterable[int]] = None) -> Tuple[int, List[Optional[Position]]]:
    if permutation is None:
        permutation = range(len(items))
    skyline = LinkedList()
    skyline.add((0, 0))

    items_list = LinkedList()
    for item in permutation:
        items_list.add((items[item].width, items[item].height, item))

    results: List[Optional[Position]] = [None for _ in range(len(items))]

    while not items_list.is_empty():
        gap = None
        level = pallet_height + 1
        for current in skyline:
            if current.data[1] < level:
                level = current.data[1]
                gap = current

        if level == pallet_height:
            break

        width = gap.next.data[0] - gap.data[0] if gap.next is not None else pallet_width - gap.data[0]
        height1 = gap.prev.data[1] - gap.data[1] if gap.prev is not None else pallet_height - gap.data[1]
        height2 = gap.next.data[1] - gap.data[1] if gap.next is not None else pallet_height - gap.data[1]

        best_item = None
        score = -1
        rotated = False
        for current_item in items_list:
            if current_item.data[1] <= pallet_height - gap.data[1] and current_item.data[0] <= width:
                cur_score = int(current_item.data[1] == height1) + int(current_item.data[1] == height2) + int(
                        current_item.data[0] == width)
                if cur_score > score:
                    score = cur_score
                    best_item = current_item
                    rotated = False
            if current_item.data[0] <= pallet_height - gap.data[1] and current_item.data[1] <= width:
                cur_score = int(current_item.data[0] == height1) + int(current_item.data[0] == height2) + int(
                        current_item.data[1] == width)
                if cur_score > score:
                    score = cur_score
                    best_item = current_item
                    rotated = True

        best_item_width = (best_item.data[0] if not rotated else best_item.data[1]) if best_item is not None else None
        best_item_height = (best_item.data[1] if not rotated else best_item.data[0]) if best_item is not None else None
        if score == -1:
            if height1 <= height2:
                if gap is not skyline.first:
                    prev = skyline.remove(gap)
                    if prev is not None and prev.next is not None and prev.data[1] == prev.next.data[1]:
                        skyline.remove(prev.next)
                else:
                    gap.data = 0, gap.data[1] + height2
                    skyline.remove(gap.next)
            else:
                if gap is not skyline.first:
                    prev = skyline.remove(gap)
                    next_point = prev.next
                    assert next_point is gap.next
                    next_point.data = gap.data[0], next_point.data[1]
                else:
                    gap.data = 0, gap.data[1] + height2
                    skyline.remove(gap.next)
        elif score == 0:
            results[best_item.data[2]] = Position(gap.data[0], gap.data[1], rotated)

            skyline.insert(gap, (gap.data[0] + best_item_width, gap.data[1]))
            gap.data = gap.data[0], gap.data[1] + best_item_height

            items_list.remove(best_item)
        elif score == 1:
            if best_item_height == height1:
                results[best_item.data[2]] = Position(gap.data[0], gap.data[1], rotated)
                gap.data = gap.data[0] + best_item_width, gap.data[1]
            elif best_item_height == height2:
                next_point = gap.next
                if next_point is not None:
                    results[best_item.data[2]] = Position(next_point.data[0] - best_item_width, gap.data[1], rotated)
                    next_point.data = next_point.data[0] - best_item_width, next_point.data[1]
                else:
                    results[best_item.data[2]] = Position(pallet_width - best_item_width, gap.data[1], rotated)
                    skyline.add((pallet_width - best_item_width, gap.data[1] + best_item_height))
            else:
                results[best_item.data[2]] = Position(gap.data[0], gap.data[1], rotated)
                gap.data = gap.data[0], gap.data[1] + best_item_height

            items_list.remove(best_item)
        elif score == 2:
            results[best_item.data[2]] = Position(gap.data[0], gap.data[1], rotated)
            if best_item_height != height1:
                skyline.remove(gap.next)
                gap.data = gap.data[0], gap.data[1] + best_item_height
            elif best_item_height != height2:
                skyline.remove(gap)
            else:
                gap.data = gap.data[0] + best_item_width, gap.data[1]

            items_list.remove(best_item)
        elif score == 3:
            next_point = gap.next
            skyline.remove(gap)
            skyline.remove(next_point)

            items_list.remove(best_item)
            results[best_item.data[2]] = Position(gap.data[0], gap.data[1], rotated)

    packed = 0
    for i in range(len(items)):
        if results[i] is not None:
            packed += items[i].width * items[i].height

    return pallet_height * pallet_width - packed, results
