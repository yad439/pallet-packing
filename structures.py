class LinkedList:
    class Node:
        def __init__(self, data, prev, next):
            self.data = data
            self.prev = prev
            self.next = next

    class Iterator:
        def __init__(self, node):
            self.node = node

        def __next__(self):
            if self.node is None:
                raise StopIteration
            node = self.node
            self.node = node.next
            return node

    def __init__(self):
        self.first = None
        self.last = None

    def __iter__(self):
        return LinkedList.Iterator(self.first)

    def isEmpty(self):
        return self.first is None

    def add(self, elem):
        if self.first is None:
            self.first = LinkedList.Node(elem, None, None)
            self.last = self.first
        else:
            new = LinkedList.Node(elem, self.last, None)
            self.last.next = new
            self.last = new

    def addAfter(self, node, elem):
        nxt = node.next
        new = LinkedList.Node(elem, node, nxt)
        node.next = new
        if node is not self.last:
            nxt.prev = new
        else:
            self.last = new

    def remove(self, node):
        if node is None:
            return None
        next = node.next
        prev = node.prev
        if prev is not None:
            prev.next = next
        if next is not None:
            next.prev = prev
        if node is self.first:
            self.first = node.next
        if node is self.last:
            self.last = node.prev
        return prev
