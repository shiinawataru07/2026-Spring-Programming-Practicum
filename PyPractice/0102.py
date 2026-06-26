import math
n, x, y = map(int, input().split())
z = math.ceil(y / x)
print(max(0, n - z))