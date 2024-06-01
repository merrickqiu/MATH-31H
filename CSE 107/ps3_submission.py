import sys, os, itertools, random

from playcrypt.primitives import *
from playcrypt.tools import *
from playcrypt.ideal.block_cipher import BlockCipher


"""
Problem 1 [100 points]
Let E: {0,1}^k x {0,1}^n -> {0,1}^n be a block cipher (with inverse E_I).
Define Enc: {0, 1}^k x {0, 1}^(mn) --> {0, 1}^((m+1)*n) as shown below.
The message space of Enc is the set of all strings M whose length is an
integer multiple of n. 

Notes:
Sizes in comments are bits, sizes in code are in bytes (bits / 8).
In the code K in{0,1}^k.
"""

def Enc(K, M):
    """
    Encryption algorithm Enc constructed from function family F.

    :param K: blockcipher key
    :param M: plaintext message
    :return: ciphertext
    """
    assert len(M) % n_bytes == 0
    M = split(M,n_bytes)
    C = random_string(n_bytes)
    R = string_to_int(C)
    for i in range(len(M)): 
        Wi = int_to_string((R + i + 1) % (2**n), n_bytes)
        C += E(K, xor_strings(M[i], Wi))
    return C

"""
(1) [20 points] Give a decryption algorithm Dec such that SE = (K,Enc,Dec) is a 
    symmetric encryption scheme satisfying the correct decrypiton condition of Slide 3.
"""

def Dec(K,C):
    """
    You must fill in this method. This is the decryption algorithm that the problem is
    asking for.

    :param K: This is the secret key for the decryption algorithm. It is an K-bit string
    :param C: This is the ciphertext to decrypt. You may assume that C is a bitstring whose length is a multiple of n. 
    :return: return a plaintext string.
    """
    assert len(C) % n_bytes == 0
    C = split(C, n_bytes)
    R = string_to_int(C[0])
    M = ""
    for i in range(1, len(C)): 
        Wi = int_to_string((R + i) % (2**n), n_bytes)
        M += xor_strings(E_I(K, C[i]), Wi)

    return M
    
"""
(2) [80 points] Give a 1-query adversary A that has advantage Adv^ind-cpa_SE(A) = 1
                and running time O(T_E + k + n).
"""

def A1(fn):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for.

    :param fn: This is the LR oracle supplied by GameIND-CPA, you can call this
    oracle with two messages to get an "encryption" of either the left or right message.
    :return: return 1 to indicate your adversary believes it is the right world
    and return 0 to indicate that your adversary believes it is in the left world.
    """

    blocks = 3
    zeros = "\x00"*n_bytes*blocks
    alternating = ""
    for i in range(blocks):
        alternating += "\x00"*(n_bytes-1)
        if i % 2 == 0:
            alternating += "\x00"
        else:
            alternating += "\x01"
            
    C = fn(zeros, alternating)
    C = split(C, n_bytes)
    if C[1] == C[2] or C[2] == C[3]:
        return 1
    else:
        return 0

"""
========================================================================================
Code below this line is used to test your solution and should not be changed.
========================================================================================
"""
from playcrypt.games.game_lr import GameLR
from playcrypt.simulator.lr_sim import LRSim

if __name__ == '__main__':
    k=128
    n=128
    k_bytes = k//8
    n_bytes = n//8

    EE = BlockCipher(k_bytes, n_bytes) # DO NOT use EE in your code
    E = EE.encrypt
    E_I = EE.decrypt

    for _ in range(100):
        K = random_string(k_bytes)
        m = random.randrange(20)
        M = random_string(m*n_bytes)
        C = Enc(K, M)
        if M != Dec(K, C):
            print ("Your decryption function is incorrect.")
            break

    g1 = GameLR(1, Enc, k_bytes)
    s1 = LRSim(g1, A1)
    adv1 = s1.compute_advantage(1000)
    print("The advantage of your adversary A1 is approximately " + str(adv1))
