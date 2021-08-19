import random
from collections import deque
import heapq


def guillotine_cutting(W, H, n_rect=10, min_side=3, side_choice='equal', seed=1):
    # each rectangle is cut randomly by width or height
    random.seed(seed)
    items_xywh = deque([(0, 0, W, H)])
    pass_cnt = 0
    while len(items_xywh) < n_rect:
        old_x, old_y, old_w, old_h = items_xywh.popleft()
        if old_w > min_side*2 or old_h > min_side*2:
            pass_cnt = 0
            w_prop = 0.5 if 'equal' == side_choice else old_w / (old_w + old_h)
            side = 'w' if random.random() < w_prop else 'h'
            if 'w' == side and old_w > min_side*2: #cut by width
                new_w1 = random.randint(min_side, old_w - min_side)
                items_xywh.append((old_x, old_y, new_w1, old_h))
                items_xywh.append((old_x + new_w1, old_y, 
                                   old_w - new_w1, old_h))
            elif 'h' == side and old_h > min_side*2:
                new_h1 = random.randint(min_side, old_h - min_side)
                items_xywh.append((old_x, old_y, old_w, new_h1))
                items_xywh.append((old_x, old_y + new_h1, 
                                   old_w, old_h - new_h1))
            else:
                items_xywh.append((old_x, old_y, old_w, old_h))
        else:
            items_xywh.append((old_x, old_y, old_w, old_h))
            pass_cnt += 1
        if pass_cnt >= len(items_xywh):
            break
    return list(items_xywh)
    

def priority_item(item_xywh):
    x,y,w,h = item_xywh
    return -max(w,h), item_xywh

def guillotine_cutting_max(W, H, n_rect=10, min_side=3, seed=1):
    # cutting occurs at the rectangle with max side
    random.seed(seed)
    heap_items_xywh = []
    heapq.heappush(heap_items_xywh, priority_item((0, 0, W, H)))

    while len(heap_items_xywh) < n_rect:
        _, (old_x, old_y, old_w, old_h) = heapq.heappop(heap_items_xywh)
        if max(old_w, old_h) < min_side*2:
            break
        side = 'w' if old_w >= old_h else 'h'
        
        if 'w' == side: #cut by width
            new_w1 = random.randint(min_side, old_w - min_side)
            heapq.heappush(heap_items_xywh, priority_item(
                           (old_x, old_y, new_w1, old_h)))
            heapq.heappush(heap_items_xywh, priority_item(
                           (old_x + new_w1, old_y, 
                            old_w - new_w1, old_h)))
        else:
            new_h1 = random.randint(min_side, old_h - min_side)
            heapq.heappush(heap_items_xywh, priority_item(
                           (old_x, old_y, old_w, new_h1)))
            heapq.heappush(heap_items_xywh, priority_item(
                           (old_x, old_y + new_h1, 
                            old_w, old_h - new_h1)))


    return list(map(lambda x : x[1], heap_items_xywh))


def bound_mean(x, x_min, x_max):
    xc = sum(x) / len(x)
    if xc > x_max:
        xc = x_max - 1
    elif xc < x_min:
        xc = x_min + 1
    return xc


def mass_generate(items_xywh, W, H, x_tol, y_tol, max_mass):
    xci = [x + w / 2 for x,y,w,h in items_xywh]
    yci = [y + h / 2 for x,y,w,h in items_xywh]
    
    xc = bound_mean(xci, W / 2 - x_tol, W / 2 + x_tol)  
    yc = bound_mean(yci, H / 2 - y_tol, H / 2 + y_tol) 
    
    mass_2end = [random.randint(1,5) for _ in range(len(items_xywh) - 2)]
    smx_2end = sum([m * x for m, x in zip(mass_2end, xci[2:])])
    smy_2end = sum([m * y for m, y in zip(mass_2end, yci[2:])])
    sm_2end = sum(mass_2end)

    bx = xc * sm_2end - smx_2end
    by = yc * sm_2end - smy_2end

    ax0 = xci[0] - xc
    ax1 = xci[1] - xc

    ay0 = yci[0] - yc
    ay1 = yci[1] - yc
    
    # ax0 * m0 + ax1 * m1 = bx
    # ay0 * m0 + ay1 * m1 = by
    delta = ax0 * ay1 - ay0 * ax1
    m0 = (bx * ay1 - by * ax1) / delta
    m1 = (ax0 * by - ay0 * bx) / delta
    
    if m0 <= 0 or m1 <= 0:
        return
    mass = [m0, m1] + mass_2end
    
    #check
    xc_check = sum([m * x for m, x in zip(mass, xci)]) / sum(mass)
    yc_check = sum([m * y for m, y in zip(mass, yci)]) / sum(mass)
    assert abs(xc - xc_check) < 1e-3
    assert abs(yc - yc_check) < 1e-3
    
    return mass
    
    
