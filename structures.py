from typing import Generic, TypeVar, Union

T = TypeVar('T')


class LinkedList(Generic[T]):
    class Node(Generic[T]):
        def __init__(self, data: T, prev_node, next_node):
            self.data = data
            self.prev = prev_node
            self.next = next_node

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

    def __iter__(self) -> Iterator:
        return LinkedList.Iterator(self.first)

    def is_empty(self) -> bool:
        return self.first is None

    def add(self, elem: T):
        if self.first is None:
            self.first = LinkedList.Node(elem, None, None)
            self.last = self.first
        else:
            new = LinkedList.Node(elem, self.last, None)
            self.last.next = new
            self.last = new

    def insert(self, node: Node, elem: T):
        nxt = node.next
        new = LinkedList.Node(elem, node, nxt)
        node.next = new
        if node is not self.last:
            nxt.prev = new
        else:
            self.last = new

    def remove(self, node: Node) -> Union[Node, None]:
        if node is None:
            return None
        next_node = node.next
        prev_node = node.prev
        if prev_node is not None:
            prev_node.next = next_node
        if next_node is not None:
            next_node.prev = prev_node
        if node is self.first:
            self.first = node.next
        if node is self.last:
            self.last = node.prev
        return prev_node
