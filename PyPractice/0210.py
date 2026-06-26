k, x = map(int, input().split(','))
def f(x):
    if x == k:
        return True
    return ((x - 1) % 2 == 0 and f((x - 1) // 2)) or ((x - 1) % 3 == 0 and f((x - 1) // 3))
if f(x):
    print('YES')
else:
    print('NO')