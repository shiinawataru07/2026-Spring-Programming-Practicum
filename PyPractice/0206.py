s, s1, s2 = input().split(',')
i1 = s.find(s1)
i2 = s.rfind(s2)
if i1 == -1 or i2 == -1 or i2 - i1 < len(s1):
    print(-1)
else:
    print(i2 - i1 - len(s1))