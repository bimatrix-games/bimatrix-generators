#!/usr/bin/env python
import itertools
import random
import numpy as np
from scipy.stats import multivariate_normal
import getopt
import sys

# Output the game in gambit format
def write_game(fName, info, A, B):
    f = open(fName, 'w')
    f.write("NFG 1 D \"Using bm-gen\nGame Info\n")
    f.write(info)

    f.write("{ \"P1\" \"P2\"}")

    m = len(A)
    f.write("{ " + str(m) + " " + str(m) + "}\n\n")

    for i in range(0, m):
        for j in range(0, m):
            f.write(str(A[j][i]) + " " + str(B[j][i]) + " ")

# Generates all unique size k integer partitions for n
# Code from
# http://stackoverflow.com/questions/18503096/python-integer-partitioning-with-given-k-partitions
def partitionfunc(n,k,l=1):
    if k < 1:
        raise StopIteration
    if k == 1:
        if n >= l:
            yield (n,)
        raise StopIteration
    for i in range(l,n+1):
        for result in partitionfunc(n-i,k-1,i):
            yield (i,)+result

# Generates all permutations of size k integer partitions for n sorted lexicographically
def part(n, k):
    res = []
    for i in range(1, k+1):
        for x in partitionfunc(n, i):
            y = x + ((k - i)*(0,))
            for z in itertools.permutations(y):
                res.append(z)

    return sorted(set(res))

# Given two troop allocations for a single hill determine who wins
def winb(a1, a2):
    if a1 > a2:
        return 0
    elif a2 > a1:
        return 1
    else:
        return np.random.randint(2)

# Given two troop allocations across all hills returns a list indicating
# which player won each hill
def win(a1, a2):
    return [winb(a1[i], a2[i]) for i in range(0, len(a1))]

# Generates a valuation vector for both players chosen from a multivariate
# normal distribution with covariance of r
def gen_payoff_cov(n, r = 1):
    mean = [0.5, 0.5]
    cov = [[1,r],[r,1]]
    return multivariate_normal.rvs(cov = cov, size = n).T
    #return np.random.multivariate_normal(mean, cov, n).T

# Generates the valuation vector for both players chosen from a uniformly
# random distribution
def gen_payoff(n):
    return np.random.uniform(0, 1, (2,n))

# Given allocations of troops for both players, returns the payoff of
# both players for that allocation
def get_payoff(payoffs, a1, a2):
    w = win(a1, a2)
    p1 = 0
    p2 = 0
    
    for i in range(0, len(w)):
        if w[i] == 0:
            p1 += payoffs[0][i]
        else :
            p2 += payoffs[1][i]
    
    return (p1, p2)

#Generates a Colonel Blotto game with troop size T, number of hills n
#and player payoffs chosen from a multivariate normal distribution with
#covariance r
def generate_game(T, n, r):
    actions = part(T, n)
    #print actions
    m = len(actions)
    payoffs = gen_payoff_cov(n, r)
    #print(payoffs)
    A = np.empty([m, m])
    B = np.empty([m, m])
    for i in range(0, m):
        for j in range(0, m):
            p = get_payoff(payoffs, actions[i], actions[j])
            A[i][j] = p[0]
            B[i][j] = p[1]
    return A, B

if __name__ == '__main__':
    try:
        opts, args = getopt.getopt(sys.argv[1:], "n:T:r:f:c:")
    except getopt.GetoptError as err:
        print str(err)
        sys.exit(2)

    game_file = ""
    n = 1
    T = 10
    c = 0.7
    for o, a in opts:
        if o == "-f" :
            game_file = a
        elif o == "-r":
            s = int(a)
            np.random.seed(s)
            random.seed(s)
        elif o == "-n":
            n = int(a)
        elif o == "-T":
            T = int(a)
        elif o == "-c":
            c = float(a)
    np.set_printoptions(precision=3)
    info = "Game: Colonel Blotto Game\nTroops: " + str(T) + "\nHills: " + str(n) + "\" "
    A, B = generate_game(T, n, c)
    write_game(game_file, info, A, B)
