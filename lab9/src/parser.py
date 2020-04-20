#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Parser to read input and return State in Blocks World Domain.
"""


class Parser():
    
    def __init__(self, filename=None):
        if filename is not None:
            self.read(filename)


    def read(self, filename):
        with open(filename) as file:
            self.lines = file.readlines()


    def parseFile(self):
        N = int(self.lines[0].split()[0].strip())
        start = self.parseLine(line_no=1)
        goal = self.parseLine(line_no=2)
        return N, start, goal


    def parseLine(self, line_no):
        predicates = self.lines[line_no].strip().split('^')
        predicates_clean = []
        for predicate in predicates:
            predicate = predicate.replace('(', '').replace(')', '').split(' ')
            predicate_clean = [predicate[0], predicate[1:]]
            predicates_clean.append(predicate_clean)
        predicates_clean.reverse()
        return predicates_clean


    def parsePlan(self, plan):
        output = []
        for x in plan:
            action, args = x[0], x[1]
            output.append(f"({action} {' '.join(args)})".strip(' '))
        return '\n'.join(output)
