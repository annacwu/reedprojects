(* 

DUP:

-----------------------------------
< DUP, S |> x > -> S |> x, x

DROP:

----------------------
< DROP, S |> x > -> S

OVER: 

-------------------------------
< OVER, S |> x, y > -> S |> x, y, x

SWAP: 

---------------------------
< SWAP, S |> x, y > -> S |> y, x

*)

type command = CONST of int | ADD | SUB | MUL | DIV
             | DUP | DROP | OVER | SWAP

(* i recognize that this isn't quite what we want because im not recursively doing things
  but this works for if you input the two consts in the list and then the operation you want to do on them! *)
let interpret (program: command list) : int list =
  let rec process_command (stack: int list) (c: command) : int list =
    match c with 
      | CONST i -> i :: stack
      | ADD -> (match stack with
        | head :: tail -> (match tail with
          | head2 :: tail2 -> head + head2 :: tail2
          | [] -> [])
       | [] -> [])
      | SUB ->( match stack with
        | head :: tail ->( match tail with
          | head2 :: tail2 -> head - head2 :: tail2
          | [] -> [])
        | [] -> [])
      | MUL -> (match stack with
        | head :: tail -> (match tail with
          | head2 :: tail2 -> head * head2 :: tail2
          | [] -> [])
        | [] -> [])
      | DIV -> (match stack with
        | head :: tail -> (match tail with
          | head2 :: tail2 -> head / head2 :: tail2
          | [] -> [])
        | [] -> [])
      | DUP -> (match stack with
        | head :: tail -> head :: stack
        | [] -> [])
      | DROP -> (match stack with
        | head :: tail -> tail
        | [] -> [] )
      | OVER -> (match stack with
        | head :: tail ->  (match tail with
          | head2 :: tail2 -> head2 :: stack
          | [] -> [])
        | [] -> []  )
      | SWAP -> (match stack with
        | head :: tail ->  (match tail with
          | head2 :: tail2 -> head2 :: head :: tail
          | [] -> [])
        | [] -> []  )
  in List.fold_left process_command [] program
let add = [CONST (2); DUP; ADD; ]
let dup = [CONST(2); DUP]