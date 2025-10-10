let rec fib (n: int) : int = 
  if n = 0 then 0
  else if n = 1 then 1
  else fib(n-1) + fib(n-2)

let rec nth (lst: 'a list) (i: int): 'a = 
  if i = 0 then match lst with 
    | [] -> failwith "Index does not exist"
    | head :: tail -> head
  else match lst with 
    | [] -> failwith "Index does not exist"
    | head :: tail -> nth (tail) (i - 1) 

let rec sum (lst: int list): int = match lst with
  | [] -> 0
  | head :: tail -> head + sum(tail)

let rec length (lst : 'a list) : int = match lst with
  | [] -> 0
  | head :: tail -> 1 + length tail

let rec append (lst1: 'a list) (lst2: 'a list): 'a list = match lst1 with
  | [] -> lst2
  | head :: tail -> head :: append tail lst2