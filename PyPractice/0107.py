import math
n, x, y = map(int, input().split())
print(max(n - math.ceil(y / x), 0))