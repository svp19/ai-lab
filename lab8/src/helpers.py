#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines helper functions for MDP - High-Low Card Game.
"""

import numpy as np


def readInput(filename):
    '''
        Returns transition probability matrix (np.array) read from file.
    '''
    P = np.loadtxt(filename)
    S = states(P.shape[0])
    return P, S


def states(num_states):
    '''
        Returns states, ie, cards numbered from 2, 3, ... (num_states + 1).
    '''
    return np.arange(2, num_states + 2)
