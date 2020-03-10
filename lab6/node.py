import enum
import numpy as np

class T(enum.Enum):
    AND = 0
    OR = 1
    SOLVED = 2

class Node():
    
    def __init__(self, indices, nodeType=T.OR):
        self.type = nodeType
        self.cost = float("Inf")
        self.parent = None
        self.children = []
        self.indices = indices
        self.bestChild = None
        self.explored = False
 
    
    def addChild(self, child):
        self.children.append(child)


    def makeChildren(self):
        '''
         Assuming self is OR Node, add all possible AND node solutions, with their children OR Nodes
        '''
        indices = self.indices
        N = len(indices)
        for i in indices[:-1]:
            newNode = Node(indices=indices, nodeType=T.AND)
            print("AND")

            print("L:", np.arange(0, i+1))
            # Create left OR Node
            leftNode = Node(indices=np.arange(0, i+1), nodeType=T.OR)
            leftNode.parent = newNode
            newNode.addChild(leftNode)
            
            print("R:", np.arange(i+1, N))
            # Create right OR Node
            rightNode = Node(indices=np.arange(i+1, N), nodeType=T.OR)
            rightNode.parent = newNode
            newNode.addChild(rightNode)
            self.addChild(newNode)

