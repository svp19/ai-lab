#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Implementation of Value iteration and Policy iteration for 
    MDP - High-Low Card Game. Read more at:  
    http://inst.cs.berkeley.edu/~cs188/sp10/section_handouts/section5_solutions.pdf 
"""

from helpers import *
from rewards import *  
import numpy as np
import sys


P = np.array([]) # Probability Matrix
S = np.array([]) # States


def iter_update(V, gamma=1):
    V_new = np.zeros(S.shape)
    policy = np.zeros(S.shape)

    # Find new Value for each state S    
    for s in S-2:
        L_val = np.sum([ P[s_new] * (R(s, LOW, s_new) + gamma*V[s_new]) for s_new in S-2 ]) # if choose LOW
        H_val = np.sum([ P[s_new] * (R(s, HIGH, s_new) + gamma*V[s_new]) for s_new in S-2 ]) # if choose HIGH
        V_new[s] = max(L_val, H_val) 
        policy[s] = np.argmax([L_val, H_val])

    return V_new, policy


def value_iteration(num_iters=100, epsilon=0.05, gamma=1):
    # Init values to zero
    V = np.zeros(S.shape)
    V_new, policy = iter_update(V, gamma)

    # while np.linalg.norm(V_new-V) > epsilon:
    while num_iters:
        V = V_new
        V_new, policy = iter_update(V, gamma)
        num_iters -= 1
        print(f"V: {V}, V':{V_new}\tPolicy: {policy}")


def policy_iteration(gamma=1):
    # Init values to zero
    V = np.zeros(S.shape)
    policy = np.zeros(S.shape)
    V_new, policy_new = iter_update(V, gamma)
    print(f"V: {V}, V':{V_new}\tPolicy: {policy_new}")
    
    # while np.linalg.norm(V_new-V) > epsilon:
    while np.array_equal(policy, policy_new):
        V, policy = V_new, policy_new
        V_new, policy_new = iter_update(V, gamma)
        print(f"V: {V}, V':{V_new}\tPolicy: {policy_new}")


if __name__ == '__main__':
    
    P, S = readInput(sys.argv[1])
    value_iteration(gamma=0.5)
    # policy_iteration(gamma=1)