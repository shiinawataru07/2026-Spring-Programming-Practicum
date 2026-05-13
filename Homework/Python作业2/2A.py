row, col = map(int, input().split())
matrix = []
for _ in range(row):
    matrix.append(list(map(int, input().split())))
for c in range(col):
    for r in range(row):
        print(matrix[r][c], end='')
        print(' ' if r != row - 1 else '\n', end='')