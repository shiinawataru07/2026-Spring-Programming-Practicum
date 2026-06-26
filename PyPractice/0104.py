tmp = False
n = int(input())
for test in (3, 5, 7):
    if n % test == 0:
        if not tmp:
            tmp = True
            print(test, end='')
        else:
            print(' ', test, sep='', end='')
if not tmp:
    print('n')