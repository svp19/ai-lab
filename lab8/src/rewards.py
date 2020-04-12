#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Defines rewards for MDP - High-Low Card Game. 
"""

from actions import *

#Reward if game ends
PENALTY = -10

def R(current, action, new):
    '''
        Returns reward from current state to new state
        taking action given. Reward is PENALTY in case
        of a wrong guess that ends the game.
    '''
    if current == new:
        return 0
    
    elif (new > current) and (action == LOW):
        return PENALTY
    
    elif (new < current) and (action == HIGH):
        return PENALTY
    
    else:
        return new
