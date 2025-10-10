let rec coprime (num1: int) (num2: int) : bool =
  if num2 = 1 then true
  else
    let remainder = num1 mod num2 in
    if remainder = 0 then false
    else coprime num2 remainder


let range (first: int) (last: int) : int list =
  let rec make_list (lst: int list) (app: int) : int list = 
    if app > last then lst 
    else match lst with
      | [] -> app :: make_list [] (app+1)
      | head :: tail -> app :: make_list tail (app + 1)
  in make_list [] first


let rec remove_duplicates (lst: 'a list) : 'a list = match lst with
  | [] -> lst
  | head :: tail -> head :: remove_duplicates (List.filter ((<>) head) tail)


(* helper for various problems *)
let rec length (lst : 'a list) : int = match lst with
  | [] -> 0
  | head :: tail -> 1 + length tail

  
let prefix_sum (lst: int list) : int list = 
  let rec helper (lst: int list) (inc: int) (stop: int) =
    if stop = 0 then []
    else let rec do_summing (lst: int list) (i: int) : int = 
      if i = 0 then 0
      else match lst with 
        | [] -> 0
        | head :: tail -> head + do_summing tail (i - 1)
    in match lst with 
      | [] -> lst
      | head :: tail -> do_summing lst (inc) :: helper lst (inc + 1) (stop - 1)
  in helper lst 1 (length(lst))


let suffix_sum (lst: int list) : int list = 
  let helper (lst: int list) (stop: int) =
    if stop = 0 then lst
    else let rec do_summing (lst: int list) (i: int) = 
      if i = 0 then lst 
      else match lst with 
        | [] -> lst
        | head :: tail -> (List.fold_left (+) head tail) :: do_summing tail (i - 1)
    in do_summing lst (stop - 1)
  in helper lst (length(lst))


let rec table (f: 'a -> 'b -> 'c) (lst1: 'a list) (lst2: 'b list): ('a * 'b * 'c) list list  = 
  let rec helper (head1: 'a) (lst2: 'b list) (ctr: int) = 
    if ctr = 0 then []
    else match lst2 with
      | [] -> []
      | head2 :: tail2 -> (head1, head2, f head1 head2) :: helper head1 tail2 (ctr - 1)
  in match lst1 with
    | [] -> []
    | head :: tail -> helper head lst2 (length(lst2)) :: table f tail lst2


(* this function is NOT efficient but it IS functional. i have implemented it in perhaps the slowest way possible. i am ashamed of it. *)
let binaries (len: int) : string list =

  (* raises 2^x *)
  let rec exp (x: int) = 
    if x = 0 then 1
    else 2 * exp(x - 1)

  (* adds leading zeros to match the given length *)
  in let rec add_zeros (str: string) : string =
    if String.length(str) < len then add_zeros ("0" ^ str)
    else str

  (* convert an integer into its reversed binary representation as a list of characters *)
  in let rec convert_to_bin (num: int): string list = 
    if num = 0 then []
    else string_of_int (num mod 2) :: convert_to_bin (num / 2)

  (* for each number 0-2^len, get its binary representation and append it to list *)
  in let count = exp len
  in let rec generate (i: int): string list =
    if i = count then []
    (* reverse the list to get the correct binary representation and turn it to a string to add it to the list *)
    else String.concat "" (List.rev (convert_to_bin i)) :: generate (i + 1)
  in let lst = generate 0 in List.map add_zeros lst


let rec combinations (k: int) (elems: 'a list) : 'a list list =
  let prepend (head: 'a) (next: 'a list): 'a list = head :: next in
  if k = 0 then [ [] ]
  else match elems with
    | [] -> []
    | head :: tail ->
        let start = List.map (prepend head) (combinations (k - 1) tail) in
        let continue = combinations k tail in
        start @ continue
  
