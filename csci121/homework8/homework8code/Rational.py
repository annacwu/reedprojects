
def GCD(a,b):
    """ Computes and returns the greatest common divisor. """ 
    if a < b:
        a,b = b,a
    if b < 0:
        b *= -1
    while b > 0:
        a,b = b,a%b
    return a

class Rational:
    """ A class defining a rational number object. """

    def __init__(self,n,d):
        """ Used to construct a ratiomal number when given
            the numerator `n` and denominator `d` of its
            fractional amount.
        """
        
        # Normalize it so the sign is up top.
        if d < 0:
            n *= -1
            d *= -1
        # Reduce the fraction.
        g = GCD(n,d)
        self.num = n // g
        self.den = d // g

    def numerator(self):
        """ Gets the numerator. """
        return self.num

    def denominator(self):
        """ Gets the denominator. """
        return self.den

    def equals(self,other):
        """ Checks for equality of one Rational with another. """
        ns = self.numerator()
        ds = self.denominator()
        no = other.numerator()
        do = other.denominator()
        return (ns * do == no * ds)

    def stringOf(self):
        """ Converts Rational to a string. """
        n = self.numerator()
        d = self.denominator()
        ntext = str(n)
        dtext = str(d)
        if d == 1:
            return ntext
        else:
            return ntext + "/" + dtext

    def output(self):
        """ Outputs a Rational as an output line. """
        print(self.stringOf())

    def sumWith(self,other):
        """ Computes the sum of one Rational with another. """
        ns = self.numerator()
        ds = self.denominator()
        no = other.numerator()
        do = other.denominator()
        return Rational(ns*do + no*ds, ds*do)

    def productWith(self,other):
        """ Computes the product of one Rational with another. """
        ns = self.numerator()
        ds = self.denominator()
        no = other.numerator()
        do = other.denominator()
        return Rational(ns*no, ds*do)
