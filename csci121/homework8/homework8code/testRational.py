from Rational import Rational

a = Rational(1, -3)
print("a =",a.stringOf())

b = Rational(-3, -6)
print("b = ",end='')
b.output()

c = a.sumWith(b)
print("a + b =",c.stringOf())

d = a.productWith(b)
print("a * b = ",end='')
d.output()
