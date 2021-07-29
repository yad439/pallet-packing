import collections

import matplotlib.pyplot as plt
import matplotlib.patches as patches


def draw_cutting(H, W, items_xywh, margin=0.1, ax=None):
    if not ax:
        fig, ax = plt.subplots()
        
    margin *= max(H, W)
    ax.set_aspect('equal')
    pallet = patches.Rectangle((0, 0), W, H, linewidth=5, facecolor='none',edgecolor='blue')
    ax.add_patch(pallet)
    ax.set_xlim(-margin, W + margin)
    ax.set_ylim(-margin, H + margin)
    # ax.grid(True)
    for x, y, w, h in items_xywh:
        rect = patches.Rectangle((x, y), w, h, linewidth=2, facecolor='gray',edgecolor='black')
        ax.add_patch(rect) 
    
    
def draw_items_cnt(items_wh, ax=None):
    if not ax:
        fig, ax = plt.subplots()
        
    w, h = list(zip(*items_wh))
    max_side = max(max(w), max(h))
    margin = 0.1 * max_side
    
    ax.set_aspect('equal')
    ax.set_xlim(-margin, max_side + margin)
    ax.set_ylim(-margin, max_side + margin)
    
    side_cnt = collections.Counter(w + h)
    item_cnt = collections.Counter(items_wh).most_common()
    cmap = plt.cm.get_cmap('Blues', item_cnt[0][1])
    cnt_set = set()
    for (w, h), cnt in item_cnt[::-1]:
        if side_cnt[h] > side_cnt[w] or h > w:
            w, h = h, w
        label = str(cnt) if cnt not in cnt_set else None
        cnt_set.add(cnt)
            
        rect = patches.Rectangle((0, 0), w, h, linewidth=2, fill=None, edgecolor=cmap(cnt), label=label)
        ax.add_patch(rect)
    ax.legend()
    
    
def draw_side_bar(items_wh, ax=None):
    if not ax:
        fig, ax = plt.subplots()
        
    w, h = list(zip(*items_wh))
    cnt = collections.Counter(w + h)
    x, y = list(zip(*cnt.most_common()))
    ax.bar(x,y)