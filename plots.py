import matplotlib.patches as patches
import matplotlib.pyplot as plt


def draw_pallet(pallet_width, pallet_height, items, positions):
    fig, ax = plt.subplots()
    pallet = patches.Rectangle((0, 0), pallet_width, pallet_height, linewidth=5, facecolor='none', edgecolor='blue')
    ax.add_patch(pallet)
    ax.set_xlim(-1, pallet_width + 1)
    ax.set_ylim(-1, pallet_height + 1)
    # ax.grid(True)
    for i in range(len(items)):
        if positions[i][0]:
            w = items[i][1] if positions[i][3] else items[i][0]
            h = items[i][0] if positions[i][3] else items[i][1]
            item = patches.Rectangle((positions[i][1], positions[i][2]), w, h, linewidth=2, facecolor='gray',
                                     edgecolor='black')
            ax.add_patch(item)
    fig.show()
