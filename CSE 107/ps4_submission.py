import sys, os, itertools, json

from playcrypt.games.game_cr import GameCR
from playcrypt.simulator.cr_sim import CRSim
from playcrypt.games.game_ufcma import GameUFCMA
from playcrypt.simulator.ufcma_sim import UFCMASim
from playcrypt.ideal.block_cipher import BlockCipher
from playcrypt.primitives import *
from playcrypt.tools import *


"""
Problem 2 [50 points]
Let E: {0,1}^k x {0,1}^l -> {0,1}^l be a block cipher (with inverse E_I) and let
T_E denote the time to compute E or E_I. Define the hash function
H2: {0,1}^k x D -> {0,1}^l as follows:
"""

def H2(K, M):
    """
    Hash function.

    :param K: Key used by the hash function, must be of size k_bytes
    :param M: Message hashed by the function, must be positive multiple of l_bytes
    """
    
    assert len(M) % l_bytes == 0
    M = split(M, l_bytes)
    C = '\x00' * l_bytes
    for i in range(len(M)):
        Wi = (E(K, xor_strings(C, M[i])))
        C = E(K, xor_strings(Wi, M[i]))
    return C


"""
[50 points] Show that H2 is not collision resistant by presenting an O(T_E + l)
    time adversary A2 with Adv^cr_H2(A2)=1.
"""

def A2(K):
    """
    You must fill in this method. We will define variables k, k_bytes, 
    l, l_bytes, E, and E_I for you.

    :param K: This is the key used as the seed to the provided hash function
    :return: Return 2 messages, M1 and M2, that your adversary believes collide
    """
    zeroBlock = "\x00"*l_bytes
    M1 = E_I(K, zeroBlock)
    M2 = M1*2
    return (M1, M2)


"""
Problem 3 [50 points]
Let E: {0, 1}^k x {0, 1)^n -> {0, 1)^n be a block cipher, n > 4.
Let D = { M \in {0, 1}* : 0 < |M| < n*2^n and |M| mod n = 0}.
Let T: {0, 1}^k x D -> {0, 1}^n be defined as follows:
"""

def T(K, M):
    if len(M) <= 0 or len(M)*8 > n*(2**n) or len(M) % n_bytes != 0:
        return None

    # M[1]...M[m] <- M; M[m+1] <- <m>
    M = split(M, n_bytes)
    m = len(M)
    M = [None] + M + [int_to_string(m, n_bytes)]

    # C[0] <- 0^n
    C = ["\x00" * n_bytes]

    # For i = 1,...,m+1 do C[i] <- E(K, C[i-1] xor M[i])
    for i in range(1, m + 2):
        C += [E(K, xor_strings(C[i - 1], M[i]))]

    # T <- C[m+1]; Return T
    return C[m + 1]

def V(K, M, t):
    if T(K, M) == t:
        return 1
    else:
        return 0


"""
[50 points] Show that T is an insecure MAC by presenting a O(n) time adversary A3 making
at most 2 queries to its tag oracle and achieving Adv^uf-cma_T(A3) = 1.
"""

def A3(tag):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for. Does not need to return anything.
    :param tag: This is an oracle supplied by GameUFCMA, you can call this
    oracle to get a "tag" for the data you pass into it.
    """
    zero = int_to_string(0, n_bytes)
    one = int_to_string(1, n_bytes)
    two = int_to_string(2, n_bytes)
    three = int_to_string(3, n_bytes)

    tag01 = tag(zero)
    tag01013 = tag(zero + one + tag01)

    return (zero + three + tag01013, tag01013)


"""
==============================================================================================
The following lines are used to test your code, and should not be modified.
==============================================================================================
"""
if __name__ == '__main__':

    k = 128
    k_bytes = k//8
    l = 128
    l_bytes = l//8
    EE = BlockCipher(k_bytes, l_bytes) # DO NOT use EE in your code
    E = EE.encrypt
    E_I = EE.decrypt

    g2 = GameCR(H2, k_bytes)
    s2 = CRSim(g2, A2)

    print ("\nWhen k=128, l=128:")
    print ("The advantage of your adversary A2 is ~" + str(s2.compute_advantage()))

    k = 256
    n = 128
    k_bytes = k//8
    n_bytes = n//8
    E = BlockCipher(k_bytes, n_bytes).encrypt
    g = GameUFCMA(2, T, V, k_bytes)
    s = UFCMASim(g, A3)

    print ("\nWhen k=256, n=128:")
    print ("The advantage of your adversary A3 is ~" + str(s.compute_advantage()))
