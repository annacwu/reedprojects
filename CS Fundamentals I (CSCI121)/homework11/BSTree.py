from operator import index


class BSTNode:
    def __init__(self,k,v):
        self.key = k
        self.value = v
        self.left = None
        self.right = None

    def help_apply(self, f):
        self.value = f(self.value)
        if self.left != None: 
            self.left.help_apply(f)
        if self.right != None: 
            self.right.help_apply(f)

class BSTree:
    def __init__(self):
        self.root = None

    def is_empty(self):
        return (self.root == None)

    def get_value(self, k): # tree search
        n = self.root
        while n != None:
            if n.key == k:
                return n.value
            elif n.key > k:
                n = n.left
            else:
                n = n.right
        return None

    def set_value(self, k, v): # tree insertion
        p = None
        n = self.root

        while n != None:
            p = n
            if k == n.key:
                n.value = v
                return
            elif k < n.key:
                n = n.left
            else:
                n = n.right

        e = BSTNode(k,v)
        if p == None:
            self.root = e
        elif k < p.key:
            p.left = e
        else:
            p.right = e

    def min_key(self):
        if self.root == None:
            return None
        else:
            n = self.root
            while n.left != None:
                n = n.left
            return n.key

    def size(self): # tree traversal

        def subtree_size(n):
            if n == None:
                return 0
            else:
                return 1 + subtree_size(n.left) + subtree_size(n.right)

        return subtree_size(self.root)

    def as_list(self):

        def subtree_list(n):
            if n == None:
                return []
            else:
                return subtree_list(n.left) + [(n.key,n.value)] + subtree_list(n.right)

        return subtree_list(self.root)

    def as_string(self):
        kvs = self.as_list()
        if len(kvs) == 0:
            return "{}"
        else:
            s = "{"
            for kv in kvs[:-1]:
                s += repr(kv[0])+": "+repr(kv[1])+", " 
            kv = kvs[-1]
            s += repr(kv[0])+": "+repr(kv[1])+"}"
            return s

    def __repr__(self):
        return self.as_string()

    __str__ = __repr__

    def __bool__(self):
        return not self.is_empty()

    def __contains__(self,k):
        return (self.get_value(k) != None)

    def __len__(self):
        return (self.size())

    def __getitem__(self,k):
        return self.get_value(k)

    def __setitem__(self,k,v):
        return self.set_value(k,v)

    def search_path(self, k):
        n = self.root
        path = []
        while n != None:
            if n.key == k:
                return path
            elif n.key > k:
                n = n.left
                path.append("L")
            else:
                n = n.right
                path.append("R")
        return path

    def apply(self, f):
        if self.root != None:
            self.root.help_apply(f)
        """
        added a helper function to node class so that we
        don't need to take an additional node paramenter
        (this was done with the help of a tutor)
        """
        
def selectionSort(someList):
    for i in range(len(someList)):
        index = i

        for j in range(index + 1, len(someList)):
            if someList[j] < someList[index]:
                index = j

        someList[i], someList[index] = someList[index], someList[i]

def countSort(someList, m):
    count = []
    while len(count) < m:
        count.append(0)
    for i in someList: 
        count[i] += 1
    ind = 0
    i = 0
    while ind < len(count):
        while count[ind] > 0:
            someList[i] = ind
            i += 1
            count[ind] -= 1
        ind += 1
