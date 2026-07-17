def cons(x,y):
	s = a = b = None #防止作弊用
	def f(n) :
		if n == 0:
			return x
		elif n == 1:
			return y
		else:
			return "error"
	return f
s = input().split()
a,b = s[0],s[1]
pair = cons(a,b)
print(pair(int(input())))