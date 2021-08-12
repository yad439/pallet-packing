import collections


class Rectangle():
    def __init__(self, w, h, placed=None, free_wh=None):
        self.wh = w, h
        self.placed = placed or []
        self.free_wh = free_wh or (0,0)
        
    @property
    def w(self):
        return self.wh[0]
    
    @property
    def h(self):
        return self.wh[1]
    
    def transposed(self):
        return Rectangle(self.h, self.w, 
                         [r.transposed() for r in self.placed],
                        (self.free_wh[1], self.free_wh[0]),
                        )
        
    @staticmethod    
    def placed_build(W, H, rect):
        # build rectangle of size (W, H) with placed rectangle rect
        if not W or not H:
            return
        
        w, h = rect.wh
        
        if (W, H) == (w, h):
            return Rectangle(W, H, [rect])  
        
        elif (W, H) == (h, w):
            return Rectangle(W, H, [rect.transposed()]) 
        
        H_h = H - h, W >= w
        W_w = W - w, H >= h
        H_w = H - w, W >= h
        W_h = W - h, H >= w
        cases = [H_h, W_w, H_w, W_h]
        residue = [c[0] for c in cases if c[1] and c[0] >= 0]
        if not residue:
            return None

        min_size = min(residue)

        if H_h[0] == min_size and H_h[1]:
            placed_r = Rectangle(w, H, 
                                [rect],
                                (w, H - h))
            free_wh = (W - w, H)
        elif W_w[0] == min_size and W_w[1]:
            placed_r = Rectangle(W, h, 
                                [rect],
                                (W - w, h))
            free_wh = (W, H - h)
        elif H_w[0] == min_size and H_w[1]:
            placed_r = Rectangle(h, H, 
                                 [rect.transposed()],
                                (h, H - w))
            free_wh = (W - h, H)
        elif W_h[0] == min_size and W_h[1]:
            placed_r = Rectangle(W, w, 
                                [rect.transposed()],
                                (W - h, w))            
            free_wh = (W, H - w)
        else:
            assert False, 'impossible'
        
        out = Rectangle(W, H, [placed_r], free_wh)
        return out

        
    def place(self, rect):
        W, H = self.free_wh      
        r = Rectangle.placed_build(W, H, rect)
        if not r:
            return False
        self.placed.append(r.placed[0])
        self.free_wh = r.free_wh
        #print(f'place {rect.wh}: free {W,H} -> {self.free_wh}')
        return True     
            
    @staticmethod
    def concat(rects, by='w'):
        w = list(map(lambda r : r.w, rects))
        h = list(map(lambda r : r.h, rects)) 
        
        if 'w' == by:
            max_w = max(w)
            placed_r = [
                Rectangle.placed_build(max_w, r.h, r)
                for r in rects
            ]
            out = Rectangle(max_w, sum(h), placed_r)
        else:
            max_h = max(h)
            placed_r = [
                Rectangle.placed_build(r.w, max_h, r)
                for r in rects
            ]
            out = Rectangle(sum(w), max_h, placed_r) 
        
        return out

    @staticmethod
    def min_concat(W, H, rect1, rect2):
        rect2T = Rectangle(rect2.h, rect2.w, rect2.placed, rect2.free_wh)
        concat_cases = [
            Rectangle.concat([rect1, rect2], by='w'),
            Rectangle.concat([rect1, rect2], by='h'),
            Rectangle.concat([rect1, rect2T], by='w'),
            Rectangle.concat([rect1, rect2T], by='h'),
        ]
        if W < H:
            W, H = H, W
        concat_cases = [r for r in concat_cases if max(r.wh) <= W and min(r.wh) <= H]
        if not concat_cases:
            return
        return min(concat_cases, key=lambda r : r.free_square)
    
    
    @property
    def square(self):
        return self.w * self.h
    
    @property
    def free_square(self):
        out = self.free_wh[0] * self.free_wh[1]
        for r in self.placed:
            out += r.free_square
        return out
    
    
    def free_print(self):
        if self.free_wh[0] and self.free_wh[1]:
            print(self.free_wh)
        for r in self.placed:
            r.free_print()
    
    @property
    def fullness(self): 
        return (1 - self.free_square / self.square) * 100
    
    def __repr__(self):
        return f'Rectangle(w={self.w}, h={self.h}, childs={len(self.placed)}, free_wh={self.free_wh}, fullness={self.fullness}%)'
    
    
def equal_side_concat_step(rects, W, H, order='descending'): 
    w = list(map(lambda r : r.w, rects))
    h = list(map(lambda r : r.h, rects)) 
    side_cnt = collections.Counter([s for s in w + h if s <= max(W, H)])
    side_repeats = [ side
                     for side, cnt in side_cnt.most_common()
                     if cnt > 1
                    ]
    side_repeats.sort(reverse=('descending' == order))
    
    single_rects = list(rects)
    side_to_rects = {}
    for side in side_repeats:
        side_to_rects[side] = [r for r in single_rects
                               if side in r.wh]
        single_rects = [r for r in single_rects 
                        if side not in r.wh]
         
    # TODO: Если прямоугольник совпадает каждой стороной с другими -> варианты по какой стороне объединять            
    concat_rects = []
    for side, side_rects in side_to_rects.items():
        if 1 == len(side_rects):
            single_rects.append(side_rects[0])
            continue
        for r in side_rects:
            if side != r.w:
                r.wh = r.h, r.w
        # TODO: 1d упаковка вдоль H или W
        # далее идет упаковка вдоль максимальной возможной стороны
        side_rects.sort(key=lambda r : r.h, reverse=True)
        concat_side = max(H, W) if side <= min(H, W) else min(H, W)
        while side_rects:
            rects_for_concat = []
            rest_rects = []
            sum_h = 0
            for r in side_rects:
                if sum_h + r.h <= concat_side:
                    rects_for_concat.append(r)
                    sum_h += r.h
                else:
                    rest_rects.append(r)
            if len(rects_for_concat) == 1:
                single_rects.append(rects_for_concat[0])
            elif len(rects_for_concat) > 1:
                concat_rects.append(Rectangle.concat(rects_for_concat, by='w')) 
            else:
                single_rects.extend(rest_rects)
                break
                #assert False, f'side_rects={side_rects}, rest_rects={rest_rects}, max_h={max_h}'
            side_rects = rest_rects
        
    return single_rects, concat_rects


def exact_concat(rects, W, H):
    merge_rects = list(rects)
    while True:
        single_rects, concat_rects = equal_side_concat_step(merge_rects, W, H, order='descending')
        #print(f'single_rects={single_rects} \n concat_rects={concat_rects} \n')
        merge_rects = single_rects + concat_rects
        if not concat_rects:
            break
    
    while True:
        single_rects, concat_rects = equal_side_concat_step(merge_rects, W, H, order='ascending')
        merge_rects = single_rects + concat_rects
        if not concat_rects:
            break
    return merge_rects


def pallet_exact_side_placement(rects, pallet, side='max'):
    W, H = pallet.free_wh
    side = max(W, H) if 'max' == side else min (W, H)

    rest_rects = []
    for r in rects:
        if side in r.wh:
            if pallet.place(r):
                continue
        rest_rects.append(r)
    return rest_rects   


def exact_placement(rects, pallet):
    rest_rects = list(rects)
    while rest_rects:
        rest_rects1 = pallet_exact_side_placement(rest_rects, pallet, side='max')
        rest_rects2 = pallet_exact_side_placement(rest_rects1, pallet, side='min')
        if len(rest_rects) == len(rest_rects2):
            break
        rest_rects = rest_rects2
    return rest_rects


def rects_flatten(rects):
    out = []
    for r in rects:
        if r.placed:
            out.extend(rects_flatten(r.placed))
        else:
            out.append(r)
    return out


def min_residue(WH, rects):
    W, H = WH
    min_r = None
    min_residue_wh = WH
    for r in rects:
        placed_r = Rectangle.placed_build(W, H, r)
        if placed_r:
            residue_wh = placed_r.placed[0].free_wh
            if residue_wh[0] * residue_wh[1] < min_residue_wh[0] * min_residue_wh[1]:
                min_residue_wh = residue_wh
                min_r = r
    return min_r, min_residue_wh


def min_residue_placement(pallet, rects):
    max_r, min_residue_wh = min_residue(pallet.free_wh, rects)
    if not max_r or min_residue_wh[0] * min_residue_wh[1] > max_r.square:
        return rects
    
    rest_rects = [r for r in rects if r is not max_r]
    
    r, _ = min_residue(min_residue_wh, rest_rects)
    if not r:
        pallet.place(max_r)
        return rest_rects
        
    return rects


def find_concat_pair(W, H, rects):
    min_loss = 1
    min_values = None
    for i in range(len(rects)):
        for j in range(i + 1, len(rects)):
            cur_concat = Rectangle.min_concat(W, H, rects[i], rects[j])
            if not cur_concat:
                continue
            cur_loss = cur_concat.free_square / min(rects[i].square, rects[j].square)
            if cur_loss < min_loss:
                min_loss = cur_loss
                min_values = (i, j, cur_concat)
    return min_values  


def free_placement(pallet, rects):
    rest_rects = list(rects)
    while rest_rects:
        W, H = pallet.free_wh
        if not W * H:
            break
        concat_rects = exact_concat(rest_rects, W, H)
        #print(f'concat_rects={concat_rects}')
        rest_rects = exact_placement(concat_rects, pallet)
        #print(f'exact_placement: rest_rects={rest_rects}, concat_rects={concat_rects}, pallet={pallet}')
        if len(rest_rects) == len(concat_rects):
            rest_rects2 = min_residue_placement(pallet, rest_rects)
            if len(rest_rects2) == len(rest_rects):
                find_values = find_concat_pair(W, H, rest_rects)
                if not find_values:
                    #print(f'not find_concat_pair for rest_rects={rest_rects}')
                    break
                i, j, concat_r = find_values
                assert pallet.place(concat_r)
                del rest_rects[j]
                del rest_rects[i]
            else:
                rest_rects = rest_rects2
        rest_rects = rects_flatten(rest_rects)
    
    return rest_rects


def pallet_placement(pallet, rects):
    rest_rects = free_placement(pallet, rects)
    
#     placed = len(rects) - len(rest_rects)
#     if placed:
#         print(f'pallet: {pallet}, placed: {placed} \n')
       
    for r in pallet.placed:
        rest_rects = pallet_placement(r, rest_rects)
        
    
    return rest_rects


def assign_coordinates(x, y, W, H, rects):
    out_xywh = []
    for r in rects:
        if W == r.h or H == r.w:
            r = r.transposed()
        if not r.placed:
            out_xywh.append((x, y, r.w, r.h))
            #print(f'append {x,y,r.w,r.h}')
        else:
            out_xywh.extend(assign_coordinates(x, y, r.w, r.h, r.placed))
        if W == r.w:
            y += r.h
            H -= r.h
        elif H == r.h:
            x += r.w
            W -= r.w
        else:
            assert False, f'WH={W,H}, r={r}'
            
    return out_xywh
