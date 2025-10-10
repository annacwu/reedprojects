(* STARTER CODE *)

type bstree =
  | Lf
  | Br of int * bstree * bstree

let rec insert (t : bstree) (i : int) : bstree =
  match t with
  | Lf -> Br (i, Lf, Lf)
  | Br (n, left, right) ->
    if i = n then t
    else if i < n then Br (n, insert left i, right)
    else Br (n, left, insert right i)

let insert_all (t : bstree) (is : int list) : bstree =
  List.fold_left insert t is

type arith =
  | Plus of arith * arith
  | Times of arith * arith
  | Negate of arith
  | Var of string
  | Num of int

let lisp_split_string (str : string) : string list =
  let rec helper str =
    if String.length str = 0 then []
    else if List.mem (String.get str 0) ['('; ')']
    then String.make 1 (String.get str 0) ::
           helper (String.sub str 1 (String.length str - 1))
    else
      match helper (String.sub str 1 (String.length str - 1)) with
      | [] -> [String.make 1 (String.get str 0)]
      | "(" :: rest -> String.make 1 (String.get str 0) :: "(" :: rest
      | ")" :: rest -> String.make 1 (String.get str 0) :: ")" :: rest
      | first :: rest -> (String.make 1 (String.get str 0) ^ first) :: rest in
  List.concat_map helper
    (List.filter
       (fun s -> String.length s > 0)
       (String.split_on_char ' ' str))

(* HOMEWORK SOLUTIONS START BELOW *)

let rec min (bst: bstree) : int =
  match bst with
    | Br(v, Lf, Lf) -> v
    | Br(v, Lf, r) -> v
    | Br(v, l, r) -> min l
    | Lf -> failwith "This should never happen"

(* implements the other way of reconstructing the tree than the tests, but should still output a correct bst *)
let rec without (bst: bstree) (remove: int) : bstree = 
  match bst with 
  | Lf -> Lf
  | Br (v, l, r) -> if v < remove then Br (v, l, without r remove) else if v > remove then Br (v, without l remove, r) else 
    match l, r with 
    | (Lf, Lf) -> Lf
    | (l, Lf) -> l
    | (Lf, r) -> r
    | (l, r) -> Br (min r, l, without r (min r))

let rec flatten (bst: bstree) : int list = 
  match bst with
  | Lf -> []
  | Br (v, Lf, Lf) -> [v]
  | Br (v, Lf, r) -> v :: flatten r
  | Br (v, l, Lf) -> min l :: flatten (without bst (min l))
  | Br (v, l, r) -> min l :: flatten (without bst (min l)) 

let free_vars (exp: arith): string list =
  let rec remove_duplicates (lst: 'a list) : 'a list = match lst with
  | [] -> lst
  | head :: tail -> head :: remove_duplicates (List.filter ((<>) head) tail) in
  let rec free_vars_help (exp: arith) : string list =
    match exp with
      | Plus (x, y) -> free_vars_help(x) @ free_vars_help(y)
      | Times (n, m) -> free_vars_help(n) @ free_vars_help(m)
      | Negate (z) -> free_vars_help (z)
      | Var v -> [v]
      | Num i -> []
  in remove_duplicates(free_vars_help(exp))

let rec subst (str: string) (rep: arith) (exp: arith) : arith =
 match exp with 
      | Plus (x, y) -> Plus (subst str rep x, subst str rep y)
      | Times (n, m) -> Times (subst str rep n, subst str rep m)
      | Negate (z) -> Negate (subst str rep z)
      | Var v -> if v = str then rep else Var v
      | Num i -> subst str rep exp

type prop =
  | And of prop * prop
  | Or of prop * prop
  | Not of prop
  | Sym of string

(* has an extra parens wrapping the whole expression but generally seems to work *)
let rec pprint (p: prop) : string = match p with
  | And (x, y) -> "(" ^ pprint x ^ " && " ^ pprint y ^ ")"
  | Or (a, b) -> "(" ^ pprint a ^ " || " ^ pprint b ^ ")"
  | Not (z) -> "(~" ^ pprint z ^ ")"
  | Sym s -> "(" ^ s ^ ")"

type 'a mtree = 
  | E
  | Branch of 'a * 'a mtree list

let breadth_first (tr: 'a mtree) : 'a list = 
  let rec bf_help (lst: 'a mtree list) (acc: 'a list) = match lst with 
  | [] -> List.rev acc
  | E :: rest -> bf_help rest acc
  | Branch (v, children) :: rest -> bf_help (rest @ children) (v :: acc) in
  match tr with 
    | E -> []
    | Branch (v, children) -> bf_help children [v]

(* this function essentially passes the tests except for the extra white space at the start of the output *)
let rec serialize (tr: string mtree) : string = match tr with
  | E -> ""
  | Branch (v, [E]) -> " " ^ v 
  | Branch (v, []) -> " " ^ v
  | Branch (v, children) -> (" (" ^ v ^ String.concat "" (List.map serialize children) ^ ")")

let rec deserialize_list (node: string list) : (string mtree * string list) = 
    let rec children_branches (nodes: string list) : (string mtree list * string list) = match nodes with
      | [] -> [], nodes
      | "(" :: next -> 
        let (tree, lst) = deserialize_list next in 
        let (ch_tree, ch_lst) = children_branches lst in (tree :: ch_tree, ch_lst)
      | ")" :: next -> [E], next
      | x :: next -> let (treelst, nodelst) = children_branches next in ((Branch(x, []) :: treelst), nodelst) in
    let rec new_branch (node: string list) : (string mtree * string list) =  match node with
    | [] -> E, node
    | "(" :: children -> new_branch children 
    | ")" :: children -> E, children
    | v :: children -> let (tree, lst) = children_branches children in (Branch (v, tree), lst)
  in new_branch node

let deserialize (node: string) : string mtree = 
  let tr_list = lisp_split_string node in 
  let (res, _) = deserialize_list tr_list in res
