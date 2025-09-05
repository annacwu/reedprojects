def count_palindromes(limit):
   count = 0
   reverse = ""
   number = 0
   n = 0
   i = n
 
   while(n <= limit):
       i = n
       while(i != 0):
           number = i % 10
           reverse = reverse + str(number)
           i = i // 10
           if(n == int(reverse)):
               count = count + 1
       n = n + 1
       reverse = ""
   return count + 1
