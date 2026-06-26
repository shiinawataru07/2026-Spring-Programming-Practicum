n = int(input())
lst = []
for i in range(n):
    line = input().split()
    id, age = line[0], int(line[1])
    lst.append((id, age, i))
def mykey(x):
    return (x[1] if x[1] >= 60 else 0, -x[2])
lst.sort(key=mykey, reverse=True)
for id, _, _ in lst:
    print(id)