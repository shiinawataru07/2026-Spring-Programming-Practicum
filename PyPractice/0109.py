n = int(input())
line = input().split()
def func(x):
    return 1 if int(x[3]) - int(x[2]) - int(x[1]) - int(x[0]) > 0 else 0
print(sum(map(func, line)))