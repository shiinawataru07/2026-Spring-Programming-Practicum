n = int(input())
def func(x):
    if x < 10:
        return str(x)
    else:
        return chr(x - 10 + ord('A'))
for _ in range(n):
    line = input().split(',')
    p, m, q = int(line[0]), line[1], int(line[2])
    if m == '0':
        print('0')
        continue
    x = int(m, p)
    lst = []
    while x > 0:
        lst.append(x % q)
        x //= q
    print(''.join(func(i) for i in lst[::-1]))