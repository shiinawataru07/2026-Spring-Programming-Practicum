p = 1
q = 2
n = int(input())
ans = 2.0
for _ in range(n - 1):
    p, q = q, p + q
    ans += q / p
print('%.4f' % ans)