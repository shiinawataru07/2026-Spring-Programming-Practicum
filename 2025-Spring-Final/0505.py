import copy

def unwrap_single(x):
    if isinstance(x, int):
        return x
    if len(x) == 1:
        return unwrap_single(x[0])
    if isinstance(x, list):
        return [unwrap_single(i) for i in x]
    if isinstance(x, tuple):
        return tuple(unwrap_single(i) for i in x)
T = int(input())
for _ in range(T):
    obj = eval(input())
    backup = copy.deepcopy(obj)
    result = unwrap_single(obj)
    assert obj == backup, "输入对象被修改"
    print(result)