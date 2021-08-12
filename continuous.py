from typing import List, Optional, Sequence, Tuple

from common import Item, Position


def continuous_score(pallet_width: int, pallet_height: int, items: Sequence[Item], overlap_coefficient: float,
                     overlap_constant: float, stick_coefficient: float, vec: Sequence[float]) -> float:
    n = len(items)

    _, rectangles = _unpack(items, vec)

    sticks = [(rect[2] - rect[0]) * (rect[3] - rect[1]) - _area(rect, (0, 0, pallet_width, pallet_height)) for rect in
              rectangles]
    take = [stick * stick_coefficient < item.width * item.height for stick, item in zip(sticks, items)]
    overlaps = (_area(rectangles[i], rectangles[j]) * take[i] * take[j] for i in range(n) for j in range(i))

    return pallet_width * pallet_height - sum(
            (item.width * item.height) * taken for item, taken in zip(items, take)) + sum(
            (overlap + overlap_constant) * (overlap > 0) for overlap in overlaps) * overlap_coefficient + sum(
            stick * taken for stick, taken in zip(sticks, take)) * stick_coefficient


def continuous_decode(pallet_width: int, pallet_height: int, items: Sequence[Item], vec: Sequence[float]) \
        -> Tuple[int, List[Optional[Position]]]:
    positions, rectangles = _unpack(items, vec)
    take = [abs(_area(rect, (0, 0, pallet_width, pallet_height)) - (rect[2] - rect[0]) * (rect[3] - rect[1])) < 0.01 for
            rect in rectangles]
    res = pallet_width * pallet_height - sum(
            item.width * item.height if taken else 0 for item, taken in zip(items, take))
    return res, [Position(*pos) if taken else None for pos, taken in zip(positions, take)]


def _unpack(items: Sequence[Item], vec: Sequence[float]):
    n = len(items)
    assert len(vec) == 3 * n
    positions = [(vec[2 * i], vec[2 * i + 1], bool(round(vec[2 * n + i]))) for i in range(n)]
    rectangles = [(position[0],
                   position[1],
                   position[0] + (item.width if not position[2] else item.height),
                   position[1] + (item.height if not position[2] else item.width))
                  for item, position in zip(items, positions)]
    return positions, rectangles


def _area(rect1: Tuple[float, float, float, float], rect2: Tuple[float, float, float, float]) -> float:
    dx = min(rect1[2], rect2[2]) - max(rect1[0], rect2[0])
    dy = min(rect1[3], rect2[3]) - max(rect1[1], rect2[1])
    return dx * dy if dx > 0 and dy > 0 else 0.0
