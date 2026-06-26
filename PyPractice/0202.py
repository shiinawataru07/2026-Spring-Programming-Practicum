line = input().split()
a, b, ch, flag = int(line[0]), int(line[1]), line[2], int(line[3])
def f(x, y):
    if flag == 1:
        return ch
    elif x == 0 or x == a - 1 or y == 0 or y == b - 1:
        return ch
    else:
        return ' '
mp = [[f(x, y) for y in range(b)] for x in range(a)]
for row in mp:
    print(''.join(row))