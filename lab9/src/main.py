#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Implementation of Goal Stack Planning(GSP) in the Blocks World Domain.
"""

from parser import *
from state import State
import sys


# Goal Stack Element Types
ACTION = 0
CONJUNCT = 1
CLAUSE = 2

class GSP():
    
    def __init__(self):
        self.state = State()
        self.plan = []
        self.stack = []


    def readInput(self, filename):
        parser = Parser(filename)
        self.N, self.start, self.goal = parser.parseFile()


    def pushSet(self, elem_type, elem):
        self.stack.append([elem_type, elem])
        if elem_type == CONJUNCT:
            for clause in elem:
                self.stack.append([CLAUSE, clause])

    def printStack(self, filename="stack.tex"):
        # print("Stack:", file=open(filename, "a"))
        # print("\\item \\textbf{Stack:}  \\\\ \\noindent\\fbox{%\n\\parbox{\\textwidth}{%", file=open(filename, "a"))
        for elem in self.stack:
            elem_type = elem[0] 
            clause = elem[1]
            if elem_type != CONJUNCT:
                print("(",clause[0], sep='', end=' ', file=open(filename, "a"))
                print(*clause[1], sep=' ', end='', file=open(filename, "a"))
                print(")", end="\\\\\n", file=open(filename, "a"))
            else:
                for c in clause:
                    print("(",c[0], sep='', end=' ', file=open(filename, "a"))
                    print(*c[1], sep=' ', end='', file=open(filename, "a"))
                    print(")", end='$\\wedge$' if clause[-1] != c else '\\\\\n', file=open(filename, "a"))
        print("}%\n}", file=open(filename, "a"))
        print(file=open(filename, "a"))

    def printTextStack(self, filename="stack.txt"):
        print("Stack:", file=open(filename, "a"))
        # print("\\item \\textbf{Stack:}  \\\\ \\noindent\\fbox{%\n\\parbox{\\textwidth}{%", file=open(filename, "a"))
        for elem in self.stack:
            elem_type = elem[0] 
            clause = elem[1]
            if elem_type != CONJUNCT:
                print("(",clause[0], sep='', end=' ', file=open(filename, "a"))
                print(*clause[1], sep=' ', end='', file=open(filename, "a"))
                print(")", end="\n", file=open(filename, "a"))
            else:
                for c in clause:
                    print("(",c[0], sep='', end=' ', file=open(filename, "a"))
                    print(*c[1], sep=' ', end='', file=open(filename, "a"))
                    print(")", end='$ ^ $' if clause[-1] != c else '\n', file=open(filename, "a"))
        print("}%\n}", file=open(filename, "a"))
        print(file=open(filename, "a"))

    def gsp(self, outputfile="output.txt"):
        # start state
        self.state.make(self.start)
        # Push GOAL into empty stack
        self.printStack()
        stack = self.stack
        self.pushSet(CONJUNCT, self.goal)

        # While stack not empty
        while len(stack) != 0:
            x = stack.pop()
            
            # print("\\item \\textbf{Stack:} pop()", file=open("stack.tex", "a"))
            # print("\\\\ \\noindent\\fbox{%\n\\parbox{\\textwidth}{%", file=open("stack.tex", "a"))
            
            self.printStack()
            # print(x)
            # print(self.state.table)
            # qwe = input()

            # If ACTION popped, add to PLAN
            if x[0] == ACTION:
                action = x[1]
                self.plan.append(action)
                self.state.progress(action)
            
            # Else if CONJUNCT popped
            elif x[0] == CONJUNCT:
                clauses = x[1]
                if self.state.checkPredicates(clauses) == False:
                    self.pushSet(x[0], x[1])

            # Else if CLAUSE popped
            else:
                
                # Check if clause satisfied
                clause = x[1]
                predicate, args = clause[0], clause[1]
                if self.state.check(predicate, args):
                    continue
                
                # if x is unsatisfied goal predicate
                action = self.chooseAction(clause)
                if action is None:
                    print("NO ACTION FOUND!")
                    return -1
                self.pushSet(ACTION, action)
                self.pushSet(CONJUNCT, self.state.getPreconditions(action))
                # print(f"Pushed {action[0]} {action[1]} and preconditions", file=open("stack.tex", "a"))
        
        parser = Parser()
        print(parser.parsePlan(self.plan), file=open(outputfile, mode="w"))

    
    def chooseAction(self, clause):
        effect, args = clause[0], clause[1]
        if effect == 'on':
            return ['stack', args]
        
        elif effect == 'ontable':
            return ['putdown', args]
        
        elif effect == 'hold':
            x = args[0]
            for stack in self.state.table:
                if x in stack:
                    if (stack[0]==x) and (len(stack)==1):
                        return ['pick', args]
                    else:
                        return ['unstack', [stack[-1], stack[-2]]]
            return None
        
        elif effect == 'clear':
            x = args[0]
            if self.state.arm == x:
                return ['putdown', [x]]
            for stack in self.state.table:
                if x in stack:
                    # y = stack[stack.index(x)-1]
                    # print("Ustack", x, y)
                    return ['unstack', [stack[-1], stack[-2]]]
            return None
        
        elif effect == 'AE':
            x = self.state.arm
            return ['putdown', [x]]

        return None

            

if __name__ == '__main__':
    
    if len(sys.argv) < 2:
        print("usage: ./run.sh <path-to-input-file> <path-to-output-file(optional)>")
        exit()
    
    outputfile = sys.argv[2] if len(sys.argv) >= 3 else "output.txt"
    
    # Goal Stack Planning on given input file
    gsp = GSP()
    gsp.readInput(sys.argv[1])
    gsp.gsp(outputfile)
