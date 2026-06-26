n = int(input())
lst = []
for i in range(n):
    sex, h = input().split()
    h = float(h)
    lst.append((sex, h))
def mykey(x):
    return (-x[1] if x[0] == 'female' else -10, x[1])
lst.sort(key=mykey)
print(*['%.2f' % x[1] for x in lst])