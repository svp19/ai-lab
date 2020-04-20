#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines actions for class State
"""
EMPTY = False


## BINDER
def progress(self, action):
    ''' Performs action to transform state'''
    name, args = action[0], action[1]
    call = {
        'stack': self.stack,
        'unstack': self.unstack,
        'pick': self.pick,
        'putdown': self.putdown
    }
    return call[name](*args)


## ACTIONS
def stack(self, x, y):
    
    for stack in self.table:
        if x in stack:
            if (stack[0]==x) and (len(stack)==1):
                self.pick(x)
            else:
                self.unstack(stack[-1], stack[-2])
    
    for block_stack in self.table:
        if y == block_stack[-1]:
            block_stack.append(x)
            self.arm = EMPTY
            return True
    return False


def unstack(self, x, y):
    for block_stack in self.table:
        if x == block_stack[-1] and y == block_stack[-2]:
            self.arm = block_stack.pop()
            return True
    return False


def pick(self, x):
    for block_stack in self.table:
        if (block_stack[0] == x) and (len(block_stack) == 1):
            self.arm = block_stack.pop()
            self.table = list(filter(None, self.table))
            return True
    return False


def putdown(self, x):
    if self.arm:
        self.table.append([x])
        self.arm = EMPTY
        return True
    return False