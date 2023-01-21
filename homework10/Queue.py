class Node:
    def __init__(self, value):
        self.value = value
        self.next = None

class Queue:

    def __init__(self):
        self.first = None
        self.last = None

    def enqueue(self, value):
        if self.first is None:
            self.first = Node(value)
            self.last = self.first
        else:
            current = self.first
            while current.next is not None:
                current = current.next
            current.next = Node(value)
            self.last = current.next

    def head(self):
        if self.first is not None:
            return self.first.value
        else: 
            return None

    def dequeue(self):
        current  = self.first
        self.first = current.next
        return current.value
