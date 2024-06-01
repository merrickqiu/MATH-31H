import sys, random

from playcrypt.tools import *
from playcrypt.new_tools import K_rsa

from playcrypt.games.game_ufcma_sign import GameUFCMASign
from playcrypt.simulator.ufcma_sign_sim import UFCMASignSim

def ADD(a,b):
    return a+b
def MULT(a,b):
    return a*b
def INT_DIV(a,N):
    return (a//N, a%N)
def MOD(a,N):
    return a%N
def EXT_GCD(a,N):
    return egcd(a,N)
def MOD_INV(a,N):
    res = modinv(a,N)
    if res == None:
        raise ValueError("Inverse does not exist.")
    return res
def MOD_EXP(a,n,N):
    return exp(a,n,N)


"""
Problem 1 [50 points]
Let K_rsa be a RSA generator with security parameter k >= 64. Consider the
digital signature scheme DS = (K, S, V) whose component algorithms are below,
where the message m lenth is 2n.
"""

def K1():
    (N, p, q, e, d) = K_rsa(k)
    pk = (N, e)
    sk = (N, d)
    return (pk, sk)

def S1(sk, m):
    """
    :param sk: The secret key sk = (N, d) used to sign messages
    :param M: The message to be signed, lenth is 2n
    :return: return the signature of message M
    """
    (N, d) = sk
    if len(m) != 2*n_bytes:
        return False
    m = split(m, n_bytes)
    x1 = int_to_string(1, 1) + m[0]
    x2 = int_to_string(2, 1) + m[1]
    y = MOD(MULT(H(N,x1), H(N,x2)), N)
    return MOD_EXP(y, d, N)

def V1(pk, m, s):
    """
    :param pk: The public key pk = (N, e) used to verify signatures
    :param M: The message to be verified, lenth is 2n
    :param S: The signature to be verified
    :return: return 1 if the signature is valid, 0 otherwise
    """
    (N, e) = pk
    if len(m) != 2*n_bytes:
        return False
    m = split(m, n_bytes)
    x1 = int_to_string(1, 1) + m[0]
    x2 = int_to_string(2, 1) + m[1]
    y = MOD(MULT(H(N,x1), H(N,x2)), N)
    if MOD_EXP(s, e, N) == y:
        return 1
    return 0

"""
Present an O(k^2+n)-time adversary A1 making at most 3 queries to its Sign oracle and
achieving Adv^{uf-cma}_DS(A) = 1.
"""

def A1(Sign, pk):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for. It should return a message and a signature.

    :param Sign:  Signing oracle
    :param pk:  Public key pk
    :return:  A message and a signature

    Note that the (message, signature) pair is returned rather than passed into
    a verify() oracle.
    """
    (N, e) = pk
    zeros = n_bytes*"\x00"
    ones = n_bytes*"\x01"
    twos = n_bytes*"\x02"

    M1 = zeros + ones
    M2 = zeros + twos
    M3 = ones + twos

    sign1 = Sign(M1)
    sign2 = Sign(M2)
    sign3 = Sign(M3)

    return (ones + ones, MOD(sign1*sign3, N) * MOD_INV(sign2, N))


"""
Problem 2 [50 points]
Let p be a prime of bit length k >= 8 such that (p - 1)/2 is also prime, and
let g be a generators of the group G = Z_p^*. Let q = p - 1. Consider the
digital signature scheme DS = (K, S, V) whose component algorithms are below,
where the message m is in Z_q^*.
"""

def K2():
    x = random_Z_N(q)
    X = MOD_EXP(g, x, p)
    y = random_Z_N(q)
    Y = MOD_EXP(g, y, p)
    return (X, Y), (x, y)

def S2(sk, m):
    """
    :param sk: The secret key sk = (x, y) used to sign messages
    :param M: The message to be signed, must be in Z_q^*
    :return: return the signature of message M
    """
    (x, y) = sk
    if EXT_GCD(m, q)[0] != 1:
        return False
    if m > q - 1 or m < 0:
        return False
    z = MOD(y + x * m, q)
    return z

def V2(pk, m, z):
    """
    :param pk: The public key pk = (X, Y) used to verify messages
    :param M: The message to be verified, must be in Z_q^*
    :param sig: The signature to be verified
    :return: return 1 if the signature is valid and 0 otherwise
    """
    (X, Y) = pk
    if EXT_GCD(m, q)[0] != 1:
        return 0
    if m > q - 1 or m < 0:
        return 0
    if z > q - 1 or z < 0:
        return 0
    if MOD(Y * MOD_EXP(X, m, p), p) == MOD_EXP(g, z, p):
        return 1
    return 0

"""
Present an O(k^2)-time adversary A2 making at most two queries to its Sign oracle and
achieving Adv^{uf-cma}_DS(A) = 1.
"""

def A2(Sign, pk):
    """
    You must fill in this method. This is the adversary that the problem is
    asking for. It should return a message and a signature.

    :param Sign:  Signing oracle
    :param pk:  Public key pk
    :return:  A message and a signature

    Note that the (message, signature) pair is returned rather than passed into
    a verify() oracle.
    """
    (X, Y) = pk

    M1 = 1
    M2 = 3

    sign1 = Sign(M1)
    sign2 = Sign(M2)

    return (5, MOD(2*sign2 - sign1, q))



hmap = {}
def sampleHashFunction(N, M):
    if (N, M) not in hmap:
        hmap[(N, M)] = random_Z_N_star(N)
    return hmap[(N, M)]

"""  
==============================================================================================
The following lines are used to test your code, and should not be modified.
==============================================================================================
"""
if __name__ == '__main__':
    
    k = 64
    n = 128
    n_bytes = n // 8

    H = sampleHashFunction

    g1 = GameUFCMASign(S1, V1, 0, K1)
    s1 = UFCMASignSim(g1, A1)

    print("The advantage of your adversary A1 is approx. " + str(s1.compute_advantage()))

    p = random.randint(2**(k - 1), 2**k)
    while not is_prime(p) or not is_prime((p-1)//2):
        p = random.randint(2**(k - 1), 2**k)
    q = p-1
    g = random_Z_N_star(p)
    while (MOD_EXP(g, (p-1)//2, p) == 1) or (MOD_EXP(g, 2, p) == 1):
        g = random_Z_N_star(p)

    g2 = GameUFCMASign(S2, V2, 0, K2)
    s2 = UFCMASignSim(g2, A2)

    print("The advantage of your adversary A2 is approx. " + str(s2.compute_advantage()))
