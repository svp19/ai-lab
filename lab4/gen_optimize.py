import sys
import random
import numpy as np


class GeneticAlgorithm(object):
    def __init__(self, filename=None):
        if not filename:
            return
        with open(filename, "r") as file:
            lines = file.readlines()
        
        isEuclidean = lines[0]
        self.N = int(lines[1])

        for i in range(2, self.N+2):
            c = [float(x) for x in lines[i].rstrip().split(' ')]

        distances = []
        for i in range(self.N+2, 2*self.N + 2):
            d = [float(x) for x in lines[i].rstrip().split(' ')]
            distances.append(d)
        self.distances = np.array(distances)

    def heuristic(self, tour):
        return sum(self.distances[tour[i-1]][tour[i]] for i in range(len(tour)))
        
    def greedyOptim(self, r):
        tours = []
        for start in range(self.N):
            sol = []
            openSet = set( range(self.N) )
            u = start
            for _ in range(self.N):
                v = max(openSet, key=lambda x: self.distances[u][x])
                sol.append(v)
                openSet.remove(v)
                u = v
            tours.append(sol)
        if r == 1:
            return max(tours, key=lambda x: self.heuristic(x))
        
        tours.sort(key=lambda x: self.heuristic(x))
        return tours[:r]

    def optimize(self, filename, iter=200):
        self.__init__(filename)
        node = self.greedyOptim(1)
        self.bestTour = node
        self.bestCost = self.heuristic(node)
        
        for _ in range(iter):
            temp = 1000
            while True:
                newNode = node.copy()
                i = random.randint(0, self.N-1)
                j = random.randint(0, self.N-1)
                newNode[i], newNode[j] = newNode[j], newNode[i]

                deltaH = self.heuristic(node) - self.heuristic(newNode)
                if deltaH > 0: # found a better node so update best and break
                    node = newNode
                    newCost = self.heuristic(node) 
                    if newCost < self.bestCost:
                        self.bestCost = newCost
                        self.bestTour = node
                        print(self.bestCost)
                    break

                else:
                    p = 1/(1 + np.exp( -deltaH/200 ) )
                    # print(p)
                    if random.random() < p:
                        node = newNode
                
                temp = 0.999*temp

optimizer = SimulatedAnnealing()
optimizer.optimize(sys.argv[1], int(10e7) )
