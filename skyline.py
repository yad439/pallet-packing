from structures import LinkedList


def skylineDecode(H, W, items, permutation):
    skyline = LinkedList()
    skyline.add((0, 0))

    itemsList = LinkedList()
    for item in permutation:
        itemsList.add((items[item][1], items[item][0], item))
    results = [None for _ in range(len(items))]
    count = 0

    while not itemsList.isEmpty():
        count += 1
        if count > 30:
            break
        gap = None
        level = H + 1
        for current in skyline:
            if current.data[0] < level:
                level = current.data[0]
                gap = current

        if level == H:
            break

        width = gap.next.data[1] - gap.data[1] if gap.next is not None else W - gap.data[1]
        height1 = gap.prev.data[0] - gap.data[0] if gap.prev is not None else H - gap.data[0]
        height2 = gap.next.data[0] - gap.data[0] if gap.next is not None else H - gap.data[0]

        bestItem = None
        score = -1
        for currentItem in itemsList:
            if currentItem.data[0] <= H - gap.data[0] and currentItem.data[1] <= width:
                curScore = int(currentItem.data[0] == height1) + int(currentItem.data[0] == height2) + int(
                        currentItem.data[1] == width)
                if curScore > score:
                    score = curScore
                    bestItem = currentItem

        if score == -1:
            if height1 <= height2:
                if gap is not skyline.first:
                    prev = skyline.remove(gap)
                    if prev is not None and prev.next is not None and prev.data[0] == prev.next.data[0]:
                        skyline.remove(prev.next)
                else:
                    gap.data = gap.data[0] + height2, 0
                    skyline.remove(gap.next)
            else:
                if gap is not skyline.first:
                    prev = skyline.remove(gap)
                    next = prev.next
                    assert next is gap.next
                    next.data = next.data[0], gap.data[1]
                else:
                    gap.data = gap.data[0] + height2, 0
                    skyline.remove(gap.next)
        elif score == 0:
            results[bestItem.data[2]] = (True, gap.data[1], gap.data[0], False)

            skyline.addAfter(gap, (gap.data[0], gap.data[1] + bestItem.data[1]))
            gap.data = gap.data[0] + bestItem.data[0], gap.data[1]

            itemsList.remove(bestItem)
        elif score == 1:
            if bestItem.data[0] == height1:
                results[bestItem.data[2]] = (True, gap.data[1], gap.data[0], False)
                gap.data = gap.data[0], gap.data[1] + bestItem.data[1]
            elif bestItem.data[0] == height2:
                next = gap.next
                if next is not None:
                    results[bestItem.data[2]] = (True, next.data[1] - bestItem.data[1], gap.data[0], False)
                    next.data = next.data[0], next.data[1] - bestItem.data[1]
                else:
                    results[bestItem.data[2]] = (True, W - bestItem.data[1], gap.data[0], False)
                    skyline.add((gap.data[0] + bestItem.data[0], W - bestItem.data[1]))
            else:
                results[bestItem.data[2]] = (True, gap.data[1], gap.data[0], False)
                gap.data = gap.data[0] + bestItem.data[0], gap.data[1]

            itemsList.remove(bestItem)
        elif score == 2:
            results[bestItem.data[2]] = (True, gap.data[1], gap.data[0], False)
            if bestItem.data[0] != height1:
                skyline.remove(gap.next)
                gap.data = gap.data[0] + bestItem.data[0], gap.data[1]
            elif bestItem.data[0] != height2:
                skyline.remove(gap)
            else:
                gap.data = gap.data[0], gap.data[1] + bestItem.data[1]

            itemsList.remove(bestItem)
        elif score == 3:
            nxt = gap.next
            skyline.remove(gap)
            skyline.remove(nxt)

            itemsList.remove(bestItem)
            results[bestItem.data[2]] = (True, gap.data[1], gap.data[0], False)

    packed = 0
    for i in range(len(items)):
        if results[i] is None:
            results[i] = (False, 0, 0, False)
        else:
            packed += items[i][0] * items[i][1]

    return H * W - packed, results
