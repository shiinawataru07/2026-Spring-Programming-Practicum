n = int(input())
def jc(n):
    if n == 0:
        return 1
    else:
        return n * jc(n - 1)
print(sum([jc(x) for x in range(1, n + 1)]))