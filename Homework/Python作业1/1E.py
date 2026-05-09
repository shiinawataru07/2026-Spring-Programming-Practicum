import math
area = int(input())
for i in range(math.ceil(math.sqrt(area)), 0, -1) :
    if area % i == 0 :
        print(2 * (i + area // i))
        break