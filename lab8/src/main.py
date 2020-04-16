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
    V = V_new = np.ones(S.shape)
    policy = np.zeros(S.shape)
    policy_new = np.ones(S.shape)
    
    # while np.linalg.norm(V_new-V) > epsilon:
    while not np.array_equal(policy, policy_new):
        
        # From previous iter
        V, policy = V_new, policy_new
        print(f"V: {V.T}, \tPolicy: {policy_new}")
        
        # Re-init
        V_new = np.zeros(S.shape)
        policy_new = np.zeros(S.shape)

        # Policy Evaluation
        A = np.multiply(gamma*np.ones(shape=(3,1)), P) - np.identity(3)
        B = []
        for s in S-2:
            B.append(
                np.sum(
                    [ P[s_new] * (R(s, policy[2], s_new)) for s_new in S-2 ] # if choose LOW
                )
            )
        B = -np.array(B).reshape(3, 1)
        # V_new = np.linalg.solve(A, -B.T)
        V_new = np.matmul(np.linalg.pinv(A), B)
        V_new = V_new.reshape(1,3)[0]

        # Policy Improvement
        for s in S-2:
            L_val = np.sum([ P[s_new] * (R(s, LOW, s_new) + gamma*V_new[s_new]) for s_new in S-2 ]) # if choose LOW
            H_val = np.sum([ P[s_new] * (R(s, HIGH, s_new) + gamma*V_new[s_new]) for s_new in S-2 ]) # if choose HIGH
            policy_new[s] = np.argmax([L_val, H_val])


if __name__ == '__main__':
    
    P, S = readInput(sys.argv[1])
    value_iteration(gamma=0.5)
    # policy_iteration(gamma=1)