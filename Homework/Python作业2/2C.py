n, m = map(int, input().split())
mny = 0
price = {}
cnt = {}
for _ in range(m):
    name, p, c = input().split()
    price[name] = int(p)
    cnt[name] = int(c)
for _ in range(n):
    line = input().split()
    for name in line:
        if cnt[name] > 0:
            mny += price[name]
            cnt[name] -= 1
print(mny)