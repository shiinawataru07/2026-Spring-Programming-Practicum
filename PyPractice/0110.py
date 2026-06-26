n, m = map(int, input().split())
print(1, end='')
for i in range(2, n + 1):
    state = True
    for j in range(1, min(i, m) + 1):
        if i % j == 0:
            state = not state
    if not state:
        print(',', i, sep='', end='')