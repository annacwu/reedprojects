class DLLNode:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.prev = None

class DLinkedList:
    def __init__(self):
        self.first = None
        self.last = None

    def prepend(self, value):
        newNode = DLLNode(value)
        newNode.next = self.first
        if newNode.next is not None:
            newNode.next.prev = newNode # assign old first a prev
        self.first = newNode
        if self.first.next is None: 
            self.last = newNode

    def append(self, value):
            if self.first is None:
                self.first = DLLNode(value)
                self.last = self.first
            else:
                current = self.last
                current.next = DLLNode(value)
                self.last = current.next
                current.next.prev = current
                
    def delete(self, value):
            previous = None 
            current  = self.first
            while current is not None and current.value != value:
                previous = current
                current = current.next
            if current is not None:  
                if previous is None: 
                    self.first = current.next
                    if self.first is not None: 
                        self.first.prev = None
                    else: 
                        self.last = None
                else:               
                    previous.next = current.next
                    if current.next is not None:
                        previous.next.prev = previous
                    if previous.next is None: 
                        self.last = previous

    def output(self):
        """Output the linked list's values, line by line. """
        current = self.first
        while current is not None:
            print(current.value)   # Print the contents of each node.
            current = current.next

    def contains(self, value):
        """Returns whether `value` is in the linked list."""
        current = self.first
        while current is not None:
            if current.value == value: # If it is found...
                return True
            current = current.next
        return False  # Scanned the whole list, not found.

    def asString(self):
        """Returns a string that is the linked list's sequence."""
        if self.first is None:
            return "<>"
        else:
            s = "<" + str(self.first.value)
            current = self.first.next
            while current is not None:
                s = s + ", " + str(current.value)
                current = current.next
            s = s + ">"
            return s

    def length(self):
        """Returns the number of nodes in the linked list."""
        count = 0
        current = self.first
        while current is not None:
            count = count + 1      # Count each node.
            current = current.next
        return count

    def isEmpty(self):
        """Returns whether the linked list is empty."""
        return (self.first is None)

    def display(self):
        """Outputs the linked list as a sequence."""
        print(self.asString())

    def __str__(self):
        return self.asString()

    def __repr__(self):
        return self.asString()

    def __bool__(self):
        return not self.isEmpty()
