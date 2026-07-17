def combine(f,g):
	def h(x):
		return f(g(x))
	return h
def square(x):
	return x * x
def double(x):
	return x + x
n = int(input())
f = combine(square,double)
#提示： f(x) = square(double(x))
print(f(n))
g = combine(f,double)
#提示: g(x) = f(double(x))
print(g(n))