"""
Problem Set 1 : Block Ciphers and Key Recovery Security Module
"""

import json
import sys, os, itertools

from playcrypt.primitives import *
from playcrypt.tools import *
from playcrypt.ideal.block_cipher import *

r"""
Problem 1 [100 points]
Let E be a blockcipher  E:{0, 1}^k x {0, 1}^n --> {0, 1}^n
and E_I be its inverse.
Define F: {0, 1}^k+n x {0, 1}^n --> {0, 1}^n as shown below.

Notes:
Sizes in comments are bits, sizes in code are in bytes (bits / 8).
In the code K1\in{0,1}^k and K2,M\in{0,1}^n
"""
 
def F(K, M):
    """
    Blockcipher F constructed from blockcipher E.

    :param K: blockcipher key
    :param M: plaintext message
    :return: ciphertext
    """
    K1 = K[:k_bytes]
    K2 = K[k_bytes:]

    C = E(K1, xor_strings(K2, M))
    return C

"""
(a) [50 points] Give a 1-query adversary A1 that has advantage
                Adv^kr_F(A1) = 1 and running time O(T_E + k + n).
"""

def A1(fn):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for.

    :param fn: This is the oracle supplied by GameKR, you can call this
    oracle to get an "encryption" of the data you pass into it.
    :return: return the a string that represents a key guess.
    """

    # Guesses
    M = "\x00"*n_bytes 
    K1 = "\x00"*k_bytes 

    #Query
    C = fn(M)

    # Invert C to get K2 xor M, which is just K2 since M=0
    K2 = E_I(K1, C)

    return K1 + K2


"""
(b) [50 points] Give a 3-query adversary A3 that has advantage Adv^kr_F(A3) = 1
                and running time O(2^k * (T_E + k + n)).
"""

def A3(fn):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for.

    :param fn: This is the oracle supplied by GameKR, you can call this
    oracle to get an "encryption" of the data you pass into it.
    :return: return the a string that represents a key guess.
    """

    # Messages
    M1 = "\x00"*n_bytes 
    M2 = "\x01"*n_bytes 
    M3 = "\x02"*n_bytes 
    K2 = "\x00"*n_bytes 
    #Query
    C1 = fn(M1)
    C2 = fn(M2)
    C3 = fn(M3)

    # Guess all possible K1 values
    for K1 in ["".join(tuple) for tuple in itertools.product(''.join(chr(i) for i in range(256)), repeat=k_bytes)]:
        # Calculate what K2 has to be for each message
        K2_M1 = xor_strings(E_I(K1, C1), M1)
        K2_M2 = xor_strings(E_I(K1, C2), M2)
        K2_M3 = xor_strings(E_I(K1, C3), M3)
        if K2_M1 == K2_M2 == K2_M3:
            K2 = K2_M1
            break

    return K1 + K2
    

"""
==============================================================================================
The following lines are used to test your code, and should not be modified.
==============================================================================================
"""

from playcrypt.games.game_kr import GameKR
from playcrypt.simulator.kr_sim import KRSim

if __name__ == '__main__':

    # Arbitrary choices of k, n.
    k = 128
    n = 64
    # Block & key size in bytes.
    k_bytes = k//8
    n_bytes = n//8
    EE = BlockCipher(k_bytes, n_bytes)
    E = EE.encrypt
    E_I = EE.decrypt
    g1 = GameKR(1, F, k_bytes+n_bytes, n_bytes)
    s1 = KRSim(g1, A1)
    print("The advantage of your adversary A1 is approximately " + str(s1.compute_advantage(20)))

    # Smaller choices of k, n.
    k = 8
    n = 64
    k_bytes = k//8
    n_bytes = n//8
    EE = BlockCipher(k_bytes, n_bytes)
    E = EE.encrypt
    E_I = EE.decrypt
    g3 = GameKR(3, F, k_bytes+n_bytes, n_bytes)
    s3 = KRSim(g3, A3)
    print("The advantage of your adversary A3 is approximately " + str(s3.compute_advantage(20)))