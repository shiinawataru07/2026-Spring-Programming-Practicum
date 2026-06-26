m, n = map(int, input().split())
print(sum(filter(lambda x: x % 17 == 0, range(m, n + 1))))