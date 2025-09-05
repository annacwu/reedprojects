
class LLNode:
    """Defines the value-containing node object for a linked list."""
    def __init__(self, value):
        self.value = value
        self.next = None

class LinkedList:
    """Defines a linked list object, consisting of a linked set of LLNode objects."""

    def __init__(self):
        """Initializes a linked list to be empty."""
        self.first = None

    def prepend(self, value):
        """Adds a node with `value` to the front of a linked list."""
        newNode = LLNode(value)
        newNode.next = self.first
        self.first = newNode

    def output(self):
        """Output the linked list's values, line by line. """
        current = self.first
        while current is not None:
            print(current.value)   # Print the contents of each node.
            current = current.next

    def append(self, value):
        "Adds a node with `value` to the end of a linked list."""
        if self.first is None:
            self.first = LLNode(value)
        else:
            current = self.first
            # Scan looking for the last node.
            while current.next is not None:
                current = current.next
            current.next = LLNode(value)

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

    def delete(self, value):
        """Removes `value` from the linked list if there."""
        previous = None  # Track the node one behind.
        current  = self.first
        # Scan all the nodes looking for the value.
        while current is not None and current.value != value:
            previous = current
            current = current.next
        # Stop scan when we've found it or scanned them all.
        if current is not None:   # If it was found...
            if previous is None:  # If it was found at the front...
                # Remove from the front.
                self.first = current.next
            else:                 # If it was one of the later nodes...
                # Skip past it with the previous node's link.
                previous.next = current.next

    def __str__(self):
        return self.asString()

    def __repr__(self):
        return self.asString()

    def __bool__(self):
        return not self.isEmpty()

    def sum(self):
        sum = 0
        current = self.first
        while current is not None:
            sum += current.value      # Count each node.
            current = current.next
        return sum

    def count(self, value):
        count = 0
        current = self.first
        while current is not None:
            if current.value == value: 
                count += 1
            current = current.next
        return count

    def apply(self, f):
        current = self.first
        while current is not None:
            current.value = f(current.value)
            current = current.next

    def deleteAll(self, value):
        previous = None  
        current  = self.first
        if current.next is None and current.value == value: 
            self.first = current.next
        else: 
            while current is not None:
                if current.value == value:
                    if previous is None: 
                        self.first = current.next
                        current = self.first
                    else:                
                        previous.next = current.next
                        current = previous.next
                else:
                    previous = current
                    current = current.next

class Sorted(LinkedList):

    def append(self, value):
        
        if self.first is None:
            self.first = LLNode(value)
        else:
            current = self.first
            if value < current.value:
                newNode = LLNode(value)
                newNode.next = current
                self.first = newNode
            else:
                while current.next is not None: 
                    if current.next.value > value:
                        break
                    current = current.next
                newNode = LLNode(value)
                newNode.next = current.next
                current.next = newNode
    