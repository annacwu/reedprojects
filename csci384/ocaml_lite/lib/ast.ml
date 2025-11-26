(** This module defines a number of types which represent OCaml-lite code. *)

type id = string
(** An identifer in OCaml-lite. *)

type typ =
  | Int
  | Bool
  | Unit
  | String
  | Func of typ * typ
  | Tup of typ * typ
  | Mono of id
  (* type bound by quantifier, and type in which the bound type appears *)
  | Poly of id * typ

let rec typ_to_str (_t : typ) : string = match _t with
  | Int -> "Int"
  | Bool -> "Bool"
  | Unit -> "()"
  | String -> "String"
  | Func (t1,t2) -> typ_to_str t1 ^ "->" ^ typ_to_str t2
  | Tup (t1,t2) -> typ_to_str t1 ^ "->" ^ typ_to_str t2
  | Mono (x) -> x
  | Poly (x, t) -> "forall " ^ x ^ ". " ^ typ_to_str t

type param = Param of id * typ option
(** A parameter appearing an an argument list. This will be needed once we add
    functions. *)

(** Get the name of a parameter. *)
let param_name : param -> id = function 
  | Param(name, _) -> name

(** Represent a parameter as a string. *)
let param_to_str : param -> string = function
  | Param(name, t) -> (match t with 
    | Some tp ->  "(" ^ name ^ " : " ^ typ_to_str tp ^ ")"
    | None -> name ^ ")" 
    )

type params = param list
(** The list parameters to a function. *)

(** Represent a set of parameters as a string. *)
let params_to_str (ps : params) : string =
  String.concat " " (List.map param_to_str ps)

(** Literals **) 
type constant = 
  | CInt of int
  | CBool of bool
  | CUnit
  | CString of string

(** Represent a literal as a string. *)
let constant_to_str : constant -> string = function
  | CInt i -> string_of_int i
  | CBool b -> string_of_bool b
  | CUnit -> "()"
  | CString s -> s

(** Binary operations. *)
type binop =
  | BAdd  (** + *)
  | BSub  (** - *)
  | BMul  (** * *)
  | BDiv  (** / *)
  | BMod  (** mod *)
  | BAnd (** && **)
  | BOr (** || **)
  | BLt (** < **)
  | BEq (** = **)
  | BCat (** ^ *)

(** Represent a binary operation as a string. *)
let binop_to_str : binop -> string = function
  | BAdd -> "+"
  | BSub -> "-"
  | BMul -> "*"
  | BDiv -> "/"
  | BMod -> "mod"
  | BAnd -> "&&"
  | BOr -> "||"
  | BLt -> "<"
  | BEq -> "="
  | BCat -> "^"

(** Unary operation. *)
type unop = 
  | UNegate  (** ~ **)
  | UNot (** not **)

(** Represent a unary operation as a string. *)
let unop_to_str : unop -> string = function 
  | UNegate -> "~"
  | UNot -> "not"

(** OCaml-lite expressions. *)
type expr =
  | EApp of expr * expr  (** e1 e2 *)
  | EBinop of expr * binop * expr  (** e1 <op> e2 *)
  | EUnop of unop * expr  (** <op> e *)
  | EVar of id  (** x *)
  | EConst of constant  (** c *)
  | ETup of expr * expr
  | ECond of expr * expr * expr (** if/then/else e *)
  | ELet of id * params * typ option * expr * expr
  | ELetRec of id * params * typ option * expr * expr
  | EAnon of params * typ option * expr

(** Represent an expression as a string. *)
let rec expr_to_str : expr -> string = function
  | EApp (e1, e2) -> "(" ^ expr_to_str e1 ^ ") (" ^ expr_to_str e2 ^ ")"
  | EBinop (l, o, r) ->
      "(" ^ expr_to_str l ^ ") " ^ binop_to_str o ^ " (" ^ expr_to_str r ^ ")"
  | EUnop (o, a) -> unop_to_str o ^ " (" ^ expr_to_str a ^ ")"
  | EVar v -> v
  | EConst c -> constant_to_str c
  | ETup (e1, e2) -> "(" ^ expr_to_str e1 ^ "," ^ expr_to_str e2 ^ ")"
  | ECond (e1, e2, e3) -> "if " ^ expr_to_str e1 ^ " then " ^ expr_to_str e2 ^ " else " ^ expr_to_str e3 
  | ELet (id, ps, t, e1, e2) -> (match t with 
    | None -> "let " ^ id ^ params_to_str ps ^ " = " ^ expr_to_str e1 ^ " in " ^ expr_to_str e2
    | Some typ -> "let " ^ id ^ params_to_str ps ^ " : " ^ typ_to_str typ ^ " = " ^ expr_to_str e1 ^ " in " ^ expr_to_str e2)
  | ELetRec (id, ps, t, e1, e2) -> (match t with 
    | None -> "let rec " ^ id ^ params_to_str ps ^ " = " ^ expr_to_str e1 ^ " in " ^ expr_to_str e2
    | Some typ -> "let rec " ^ id ^ params_to_str ps ^ " : " ^ typ_to_str typ ^ " = " ^ expr_to_str e1 ^ " in " ^ expr_to_str e2)
  | EAnon (ps, t, e) -> (match t with
    | None -> "fun " ^ params_to_str ps ^ " => " ^ expr_to_str e
    | Some typ -> "fun " ^ params_to_str ps ^ " : " ^ typ_to_str typ ^ " => " ^ expr_to_str e)
     

(** A top-level binding.  We don't have bindings in the language yet, but my test
    code is more consistent if we artificially wrap expressions in a single
    top-level binding from the beginning. *)
type binding = BLet of id * params * typ option * expr | BLetRec of id * params * typ option * expr

(** Represent a binding as a string. *)
let binding_to_str : binding -> string = function
  | BLet (id, ps, mty, e) ->
      let tystr =
        match mty with None -> "" | Some t -> ":" ^ typ_to_str t
      in
      "let " ^ id ^ " " ^ params_to_str ps ^ tystr ^ " = (" ^ expr_to_str e
      ^ ")"
  | BLetRec (id, ps, mty, e) ->
      let tystr =
        match mty with None -> "" | Some t -> ":" ^ typ_to_str t
      in
      "let rec " ^ id ^ " " ^ params_to_str ps ^ tystr ^ " = (" ^ expr_to_str e
      ^ ")" 

type program = binding list
(** An OCaml-lite program. *)

(** Represent a program as a string. *)
let program_to_str (p : program) : string =
  String.concat ";; " (List.map binding_to_str p) 