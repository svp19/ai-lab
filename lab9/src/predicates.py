#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines predicates for class State
"""
EMPTY = False


## BINDER
def check(self, predicate, args):
    ''' Returns truth value of predicate'''
    call = {
        'on': self.on,
        'ontable': self.ontable,
        'hold': self.hold,
        'clear': self.clear,
        'AE': self.AE
    }
    return call[predicate](*args)


## PREDICATES
def on(self, x, y):
    '''Is x on y'''
    for stack in self.table:
        if (x in stack) and (y in stack):
            return ( stack.index(x) == (stack.index(y) + 1) )
    return False


def ontable(self, x):
    '''Is x on table'''
    for stack in self.table:
        if stack[0] == x:
            return True
    return False


def hold(self, x):
    ''' Does arm hold x'''
    return (self.arm == x)


def clear(self, x):
    '''is x clear'''
    if self.arm == x:
        return True
    for stack in self.table:
        if x in stack:
            if x == stack[-1]:
                return True
            break
    return False


def AE(self):
    '''Is arm empty'''
    return (self.arm == EMPTY)

