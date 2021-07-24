from typing import Iterable, Union

import matplotlib.patches as patches
import matplotlib.pyplot as plt

from common import Item, Position


def draw_pallet(pallet_width: int, pallet_height: int, items: Iterable[Item], positions: Iterable[Union[Position, None]]) -> None:
    fig, ax = plt.subplots()
    pallet = patches.Rectangle(
        (0, 0), pallet_width, pallet_height, linewidth=5, facecolor='none', edgecolor='blue')
    ax.add_patch(pallet)
    ax.set_xlim(-1, pallet_width + 1)
    ax.set_ylim(-1, pallet_height + 1)
    # ax.grid(True)
    for (item, position) in zip(items, position):
        if position is not None:
            w = item.width if not position.rotated else item.height
            h = item.height if not position.rotated else item.width
            item = patches.Rectangle((position.x, position.y), w, h, linewidth=2, facecolor='gray',
                                     edgecolor='black')
            ax.add_patch(item)
    fig.show()
