#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines state in Blocks World Domain.
"""
import predicates as PREDICATES
import preconditions as PRECONDITIONS
import actions as ACTIONS

EMPTY = False
    
class State():

    def __init__(self):
        self.arm = EMPTY
        self.table = []

    ## PREDICATES
    check   =   PREDICATES.check
    on      =   PREDICATES.on
    ontable =   PREDICATES.ontable
    hold    =   PREDICATES.hold
    clear   =   PREDICATES.clear
    AE      =   PREDICATES.AE


    ## PRECONDITIONS
    getPreconditions    =   PRECONDITIONS.getPreconditions
    checkPreconditions  =   PRECONDITIONS.checkPreconditions
    checkPredicates     =   PRECONDITIONS.checkPredicates

    ## ACTIONS 
    progress    =   ACTIONS.progress
    stack       =   ACTIONS.stack
    unstack     =   ACTIONS.unstack
    pick        =   ACTIONS.pick
    putdown     =   ACTIONS.putdown 

    def make(self, clauses):
        '''Set state according to predicates'''
        on_list = []
        for clause in clauses:
            predicate, args = clause[0], clause[1]
            if predicate == 'ontable':
                self.table.append([args[0]])
            elif predicate == 'hold':
                self.arm = args[0]
            elif predicate == 'AE':
                self.arm = EMPTY
            elif predicate == 'on':
                on_list.append(clause)

        i = -1
        while len(on_list) != 0:
            i = (i+1)%len(on_list)
            clause = on_list[i]

            predicate, args = clause[0], clause[1]
            x, y = args[0], args[1]
            
            for stack in self.table:
                if y in stack:
                    stack.insert(stack.index(y) + 1, x)
                    on_list.remove(clause)


if __name__ == '__main__':
    state = State()
    state.table = [['c'], ['a', 'b'], ['e', 'f', 'g']]
    state.arm = 'd'
    # print(state.check('on', ['b', 'a']))
    # print(state.check('ontable', ['a']))
    # print(state.check('hold', ['d']))
    # print(state.check('clear', ['g']))
    # print(state.check('AE', []))
    # print(state.progress(['putdown', ['d']]))
    # print(state.table)
    # print(state.arm)

    