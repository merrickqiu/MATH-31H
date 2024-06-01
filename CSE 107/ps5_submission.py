import sys, os, itertools, json

from playcrypt.tools import *
from playcrypt.games.game_ufcma import GameUFCMA
from playcrypt.simulator.ufcma_sim import UFCMASim


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
def kgen():
    return random_Z_N_star(p-1)

"""
M   gcd(M, p-1)     M in Z_{p-1}^*?
0   p-1             No
1   1               Yes
2   2               No
3   1               Yes
"""
"""
Problem 1 [100 points] Let q be a k-bit prime such that p = 2*q + 1 is also prime, and assume 
k >= 2048. Let g in Z_p^* be a generator of Z_p^*.(These are public quantities, known 
to all parties including the adversary.) We will define variables p, q and g for you. 
Consider the family of functions T : Z_{p-1}^* x Z_{p-1}^* -> Z_p^* defined below:
"""

def T(K, M):
    """
    :param K: The public key that is in Z_{p-1}^*
    :param M: The plaintext that must be in Z_{p-1}^*
    :return: return the output of the family of functions.
    """
    if not in_Z_N_star(M,p-1):
        raise ValueError("Message not in appropriate domain.")
    if not in_Z_N_star(K,p-1):
        raise ValueError("Key not in appropriate domain.")
    w = MOD(MULT(M,K), p-1)
    x = MOD_INV(w, p-1)
    Y = MOD_EXP(g,x,p)
    return Y

def V(K, M, t):
    if T(K, M) == t:
        return 1
    else:
        return 0


"""
The message M must be in Z_{p-1}^*, meaning only elements of Z_{p-1}^* are allowed as
messages. We let k be the bit-length of q.

Specify an O(k^3)-time adversary A making one Tag query such that
Adv_{T}^{uf-cma}(A) = 1.
"""

def A(tag):
    """
    You must fill in this method. This is the adversary that the problem is asking for.
    :param tag: This is an oracle supplied by GameUFCMA, you can call this
    oracle to get a "tag" for the data you pass into it.
    Note that messages passed into tag() must be in Z_{p-1}^*.
    :return: Return a forged (message, tag) pair
    """
    # print(MOD_EXP(MOD_EXP(2,57,59),57,59))
    # print(MOD_EXP(2,MOD(57*57, 58),59))
    # g_k = tag(1)
    # g_nk = MOD_INV(g_k,p)
    # return (p-2, g_nk)
    m = 3
    n = 69
    g_mk = tag(m)
    exponent = MOD(MOD_INV(n, p-1)* m,p-1)
    return (n, MOD_EXP(g_mk,exponent,p))


"""  
==============================================================================================
The following lines are used to test your code, and should not be modified.
==============================================================================================
"""
if __name__ == '__main__':
    sys.setrecursionlimit(2000)
    print("This check may run for a minute...")

    s="%s%s%s%s%s%s%s%s%s" % (
        "105769031300962262347041907699188531986468080875168500291924909352726",
        "523514845129707954505966025516424111093772977370588541844230910553861",
        "654594521893564682962587494595327088018194062158369048764578647394423",
        "878171592936776203779788139029433508667038014539146349807292798552653",
        "558648914249755271208710270667471762323995019920493552915437914409534",
        "037901781078867071136428476829769041285776781721175931798108515586171",
        "194767580162162682727491216460561250733810235821601248338156562332774",
        "779512075289235506520068278611847500719701594730286561581764577491123",
        "045250981621775358501843358439025539644963902870293437315451576463")

    s1="%s%s%s%s%s%s%s%s%s" % (
        "528845156504811311735209538495942659932340404375842501459624546763632",
        "617574225648539772529830127582120555468864886852942709221154552769308",
        "272972609467823414812937472976635440090970310791845243822893236972119",
        "390857964683881018898940695147167543335190072695731749036463992763267",
        "793244571248776356043551353337358811619975099602467764577189572047670",
        "189508905394335355682142384148845206428883908605879658990542577930855",
        "973837900810813413637456082302806253669051179108006241690782811663873",
        "897560376446177532600341393059237503598507973651432807908822887455615",
        "22625490810887679250921679219512769822481951435146718657725788231")

    p = int(s)
    q = int(s1)
    g = 5

    gm = GameUFCMA(1, T, V, None, kgen)
    s = UFCMASim(gm, A)

    print("The advantage of your adversary for problem 1 is ~" + str(s.compute_advantage(100)))
