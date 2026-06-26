line = input()[::-1]
def op(ch):
    if ch.isupper():
        if ch == 'Z':
            return 'c'
        elif ch == 'Y':
            return 'b'
        elif ch == 'X':
            return 'a'
        else:
            return chr(ord(ch) + 3).lower()
    elif ch.islower():
        if ch == 'z':
            return 'C'
        elif ch == 'y':
            return 'B'
        elif ch == 'x':
            return 'A'
        else:
            return chr(ord(ch) + 3).upper()
lst = [op(ch) for ch in line]
print(''.join(lst))