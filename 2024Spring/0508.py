n = int(input())
st = set()
for _ in range(n):
    lst = input().split('-')
    for i in range(len(lst)):
        st.add('-'.join(lst[:i + 1]))
m = int(input())
for _ in range(m):
    line = input()
    if line in st:
        print('Yes')
    else:
        print('No')