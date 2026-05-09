x, y, op = input().split()
if op == "+" :
    print(int(x) + int(y))
elif op == "-" :
    print(int(x) - int(y))
elif op == "*" :
    print(int(x) * int(y))
elif op == "/" :
    if(int(y) == 0) :
        print("Divided by zero!")
    else :
        print(int(x) // int(y))
else :
    print("Invalid operator!")