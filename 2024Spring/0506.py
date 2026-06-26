hs = {'S':3, 'H':2, 'C':1, 'D':0}
ds = {'2':15, 'A':14, 'K':13, 'Q':12, 'J':11, 'T':10, '9':9, '8':8, '7':7, '6':6, '5':5, '4':4, '3':3}
def mykey(p):
    if p == 'BJ':
        return (100, 1)
    if p == 'LJ':
        return (99, 1)
    return (ds[p[0]], hs[p[1]])

try:
    while True:
        pd = input().split()
        pd.sort(key=mykey, reverse=True)
        for i in range(len(pd)):
            if pd[i][1] == 'J':
                if i == len(pd) - 1 or pd[i + 1][1] != 'J':
                    print(pd[i])
                else:
                    print(pd[i], end=' ')
            else:
                if i == len(pd) - 1 or pd[i][0] != pd[i + 1][0]:
                    print(pd[i])
                else:
                    print(pd[i], end=' ')
        print()
except EOFError:
    pass