line = list(input())
cnt = dict()
for ch in line:
    cnt[ch] = cnt.get(ch, 0) + 1
line.sort(key=lambda x: (-cnt[x], x))
print(line[0], cnt[line[0]])