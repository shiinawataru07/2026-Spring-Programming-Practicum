ch = input()
if ch:
    print('YES' if ord(ch) % 2 == 1 else 'NO')
else:
    print('YES' if ord('\n') % 2 == 1 else 'NO')