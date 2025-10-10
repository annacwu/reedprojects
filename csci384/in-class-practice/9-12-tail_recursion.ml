let rec factorial (n: int): int = 
  if n = 0 then 1
  else n * factorial (n-1)

let power (n: int) (m: int) : int =
  let rec helper x acc =
    if x = 0 then acc
    else helper (x-1) (acc * n)
  in helper m 1

let first_column (mat: int list list) : int list =
  let get_head (lst: int list): int =
    match lst with 
      | [] -> 0
      | head :: tail -> head in
  List.map get_head mat