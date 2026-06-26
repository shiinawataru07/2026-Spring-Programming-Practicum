n = int(input())
rmin = -1
rmax = -1
lmin = -1
lmax = -1
for r in range(n):
    line = ''.join(map(lambda x: '0' if x == '0' else '1', input().split()))
    if '0' in line:
        if rmin == -1:
            rmin = r
        idx = line.find('0')
        ridx = line.rfind('0')
        if lmin == -1:
            lmin = idx
        if lmax == -1:
            lmax = ridx
        rmax = r
print((rmax - rmin - 1) * (lmax - lmin - 1))