line = list(map(int, input().split()))
lst = [abs(line[i] - line[i + 1]) for i in range(1, line[0])]
lst.sort()
if lst == list(range(1, line[0])):
    print("Jolly")
else:
    print("Not jolly")