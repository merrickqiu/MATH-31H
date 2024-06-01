import sys, os, itertools, json

from playcrypt.primitives import *
from playcrypt.tools import *
from playcrypt.ideal.function_family import *

from playcrypt.games.game_prf import GamePRF
from playcrypt.simulator.world_sim import WorldSim

"""
Problem 1 [100 points]
Let G: {0, 1}^k x {0, 1}^l -> {0, 1}^l be a family of functions.
Let integer r >= 1. Define G^(r): {0, 1}^k x {0, 1}^2l -> {0, 1}^2l, as Follows:
"""

def G_r(r):
    def Gr(K, X):
        """
        You must not change this method.

        :param K: blockcipher key
        :param M: plaintext message
        :return: ciphertext
        """
        L = X[:l_bytes]
        R = X[l_bytes:]
        for _ in range(r):
            temp = L
            L = R
            R = xor_strings(G(K,R), temp)
        return L+R
    return Gr

"""
(a) [40 points] Give a 1-query adversary A1 that has advantage
                Adv^prf_G_1(A1) = 1 - 2^-l and running time O(T_G + k + l).
"""

def A1(fn):
    """
    You must fill in this method. We will define variables k, l, k_bytes, l_bytes,
    and G for you.

    :param fn: This is the oracle supplied by GamePRF.
    :return: return 1 to indicate your adversary believes it is the real world
    and return 0 to indicate that your adversary believes it is in the random
    world.
    """
    x = "\x00"*l_bytes + "\x00"*l_bytes
    output = fn(x)
    return output[:l_bytes:] == "\x00"*l_bytes 

"""
(b) [60 points] Give a 2-query adversary A2 that has advantage
                Adv^prf_G_2(A2) = 1 - 2^-l and running time O(T_G + k + l).
"""

def A2(fn):
    """
    You must fill in this method. We will define variables k, l, k_bytes, l_bytes,
    and G for you.

    :param fn: This is the oracle supplied by GamePRF.
    :return: return 1 to indicate your adversary believes it is the real world
    and return 0 to indicate that your adversary believes it is in the random
    world.
    """
    x1 = "\x00"*l_bytes + "\x00"*l_bytes
    x2 = "\xFF"*l_bytes + "\x00"*l_bytes
    output1 = fn(x1)
    output2 = fn(x2)
    return xor_strings(output1[:l_bytes:], output2[:l_bytes:]) == "\xFF"*l_bytes
    

if __name__ == '__main__':
    print("When k=128, l=120:")
    k=128
    l=120
    k_bytes = k//8
    l_bytes = l//8
    G = FunctionFamily(k_bytes, l_bytes, l_bytes).evaluate
    g1 = GamePRF(1, G_r(1), k_bytes, 2*l_bytes, 2*l_bytes)
    s1 = WorldSim(g1, A1)
    adv1 = s1.compute_advantage(1000)
    print("The advantage of your adversary A1 is approximately " + str(adv1))

    g2 = GamePRF(2, G_r(2), k_bytes, 2*l_bytes, 2*l_bytes)
    s2 = WorldSim(g2, A2)
    adv2 = s2.compute_advantage(1000)
    print("The advantage of your adversary A2 is approximately " + str(adv2))

    print("When k=128, l=8:")
    k=128
    l=8
    k_bytes = k//8
    l_bytes = l//8
    G = FunctionFamily(k_bytes, l_bytes, l_bytes).evaluate
    g1 = GamePRF(1, G_r(1), k_bytes, 2*l_bytes, 2*l_bytes)
    s1 = WorldSim(g1, A1)
    adv1 = s1.compute_advantage(1000)
    print("The advantage of your adversary A1 is approximately " + str(adv1))

    g2 = GamePRF(2, G_r(2), k_bytes, 2*l_bytes, 2*l_bytes)
    s2 = WorldSim(g2, A2)
    adv2 = s2.compute_advantage(1000)
    print("The advantage of your adversary A2 is approximately " + str(adv2))
