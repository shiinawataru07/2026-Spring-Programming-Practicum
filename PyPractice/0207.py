n, m = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(n)]
b = [list(map(int, input().split())) for _ in range(n)]
mp = [[a[i][j] + b[i][j] for j in range(m)] for i in range(n)]
for row in mp:
    print(' '.join(map(str, row)))