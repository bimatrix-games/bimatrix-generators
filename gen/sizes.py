import part
def sizes_T_fix_n(s, n):
    j = s
    l = len(part.part(j, n))
    for i in range(100, 2001, 100):
        while l < i:
            j += 1
            l = len(part.part(j, n))
        print i, "-->", j, "==>", l

def sizes_n_fix_T(s, T):
    j = s
    l = len(part.part(T, j))
    for i in range(100, 2001, 100):
        while l < i:
            j += 1
            l = len(part.part(T, j))
        print i, "-->", j, "==>", l
