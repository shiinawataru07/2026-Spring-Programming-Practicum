n, m, k = map(int, input().split())
k = (k - 1) // 2
mp = [list(map(int, input().split())) for _ in range(n)]
res = [[0 for _ in range(m)] for _ in range(n)]
for i in range(n):
    for j in range(m):
        maxnum = -1e7
        for x in range(max(0, i - k), min(n, i + k + 1)):
            maxnum = max(maxnum, max(mp[x][max(0, j - k):min(m, j + k + 1)]))
        res[i][j] = maxnum
for row in res:
    print(*row)