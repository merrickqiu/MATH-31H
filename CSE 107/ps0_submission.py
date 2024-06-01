"""
Problem Set 0 : Introducing PlayCrypt 

General comment on all the problems below:

- You must complete the code of the functions P1, P2. If you wish, you
  may also add helper functions (but those should not be necessary
  for this assignment).

- PlayCrypt includes a suite of potentially useful functions under Tools
  (https://ucsdcse107.github.io/playcrypt/playcrypt.tools.html) and 
  Primitives (https://ucsdcse107.github.io/playcrypt/playcrypt.primitives.html).
  The source code is also public at https://github.com/UCSDCSE107/playcrypt.
  These resources will be useful for subsequent assignments.

- The testing code at the bottom of this file is similar to (but not 
  the same as) what is run on the Gradescope autograder. You can thus
  test locally before submitting on Gradescope. You may also submit 
  multiple times on Gradescope.


----------------- please write text here (if relevant) ------------------

List your references and/or the people you discussed with here:

Alice, Bob

-------------------------------------------------------------------------
"""


import json
import sys, os, itertools

from playcrypt.primitives import *
from playcrypt.tools import *


def P1():
  """
  Return a list consisting of the first ten prime numbers in order.
  """
  return [2,3,5,7,11,13,17,19,23,29]


def P2(a, b, N):
  """
  Assume that a, b are integers and that N is a positive integer.
  Return the integer (a+b) mod N.
  """
  return (a+b)%N


"""
=========================================================================
The following lines may be used to locally test your code. 
Note that Gradescope is running more precise checks.
If you are unfamiliar with python, the code after the following if-main 
line is what is run when this file is executed.
=========================================================================
"""

if __name__ == '__main__':
  first_ten_primes = P1()
  print("\nYour first ten primes are " + str(first_ten_primes) + ".")

  # One example of a simple check.
  if len(first_ten_primes) < 7 or len(first_ten_primes) > 13:
    print("This is the wrong number of primes.")

  # Another example of a simple check.
  if 4 in first_ten_primes:
    print("Not all of these numbers are prime.")

  a1 = random.randint(-100, 100)
  b1 = random.randint(-100, 100) 
  N1 = random.randint(1, 50) 
  a2 = random.randint(-100, 100)
  b2 = random.randint(-100, 100) 
  N2 = random.randint(1, 50)

  # Examples of modular arithmetic.
  sol1 = P2(a1, b1, N1)
  sol2 = P2(a2, b2, N2)
  print("\nYour P2 is computing " + str(a1) + " + " + str(b1) + " mod " + str(N1) + " = " + str(sol1))
  print("and " + str(a2) + " + " + str(b2) + " mod " + str(N2) + " = " + str(sol2) + ".")
