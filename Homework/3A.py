def op(x) :
    n = len(x)
    if n == 0 :
        return ''
    return op(x[1:(n+1)//2]) + x[0] + op(x[(n+1)//2 :n])
print(op(input()))