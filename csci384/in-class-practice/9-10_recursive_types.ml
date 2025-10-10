type 'a my_option =
  | None
  | Val of 'a

let rec lookup (lst : ('a * 'b) list) (key: 'a) : 'b my_option =
  match lst with
    | [] -> None
    | head :: tail -> match head with 
      | (x, y) -> if x = key then Val y else lookup tail key

type 'a tree = Leaf | Branch of 'a * 'a tree * 'a tree

let rec fold (tr: 'a tree) (f: 'b -> 'a -> 'b) (start: 'b) : 'b =
  match tr with
  | Leaf -> start
  | Branch (x, l, r) -> 

