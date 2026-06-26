n = int(input())
d = dict()
cnt = dict()
maxid = -1
maxcnt = -1
for _ in range(n):
    line = list(map(int, input().split()))
    if line[1] == 0:
        continue
    for i in line[2:]:
        cnt[i] = cnt.get(i, 0) + 1
        if cnt[i] > maxcnt:
            maxcnt = cnt[i]
            maxid = i
        if i not in d:
            d[i] = set()
        d[i].add(line[0])
print(maxid)
print(*sorted(list(d[maxid])))