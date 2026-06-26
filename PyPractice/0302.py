line = input()
lastch = ' '
maxl = 0
maxch = ' '
l = 0
for ch in line:
    if ch != lastch:
        l = 1
        lastch = ch
    else:
        l += 1
    if l > maxl:
        maxl = l
        maxch = ch
print(maxch, maxl)