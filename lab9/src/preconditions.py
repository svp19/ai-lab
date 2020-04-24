#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines preconditions for class State
"""

def getPreconditions(self, action):
    name, args = action[0], action[1]
    x = args[0] 
    y = args[1] if 'stack' in name else None
    
    precond_dict = {
        'stack': [
            [ 'AE', [] ], 
            [ 'clear', [y] ],
            [ 'clear', [x] ], 
        ],
        'unstack': [
            [ 'AE', [] ],
            [ 'clear', [x] ],
            [ 'on', [x, y] ],
        ],
        'pick': [
            [ 'AE', [] ],
            [ 'clear', [x] ],
            [ 'ontable', [x] ],
        ],
        'putdown': [
            [ 'hold', [x] ]
        ]
    }
    return precond_dict[name]


def checkPreconditions(self, action):
    preconditions = self.getPreconditions(action)
    return self.checkPredicates(preconditions)


def checkPredicates(self, clauses):
    for clause in clauses:
        predicate, args = clause[0], clause[1]
        if self.check(predicate, args) == False:
            return False
    return True
