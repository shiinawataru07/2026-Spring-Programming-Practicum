n, ta, tb = map(int, input().split())
sa = list(map(int, input().split()))
sb = list(map(int, input().split()))
ia = 0
ib = 0
wa = 0
wb = 0
def awinb(a, b):
    return a - b == 5 or a - b == -3 or a - b == -2 
for _ in range(n):
    if sa[ia] != sb[ib]:
        if awinb(sa[ia], sb[ib]):
            wa += 1
        else:
            wb += 1
    ia = (ia + 1) % ta
    ib = (ib + 1) % tb
if wa > wb:
    print("A")
elif wa < wb:
    print("B")
else:
    print("draw")