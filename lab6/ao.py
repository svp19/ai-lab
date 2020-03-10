import numpy as np
from node import Node
from node import T

class AOSearch():

    def __init__(self, dims):
        self.dims = dims
        self.root = Node(indices=np.arange(len(dims)-1))
        self.root.makeChildren()


    def run(self):
        while(self.root.nodeType != T.SOLVED):
            node = traceMarkedPath()


    def traceMarkedPath(self, node):
        explore = None
        bestMinScore = float("inf")

        # If current node is OR, return min AND child node
        if node.nodeType == T.OR:
            for child in node.children:
                if h1(child) < bestMinScore:
                    explore = child
                    bestMinScore = h1(child)

            if child.explored:
                return traceMarkedPath(child)
            return child

        


    def h1(self, node):
        if len(node.indices) <= 0:
            return 0
        val = self.dims[0]
        for i in node.indices:
            print(i, self.dims[i])
            val = val * self.dims[i+1]
        return val/len(node.indices)
    

    def h2(self, node):
        if len(node.indices) <= 0:
            return 0
        val = self.dims[0]
        for i in node.indices:
            print(i, self.dims[i])
            val = val * self.dims[i+1]
        return val*len(node.indices)


AO = AOSearch([1, 2, 3, 4])
print(AO.h1(AO.root))