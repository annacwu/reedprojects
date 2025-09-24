(** This module defines a number of types which represent OCaml-lite code. *)

type id = string
(** An identifer in OCaml-lite. We'll use this when we add let-bindings to the
    language. *)

type typ
(** A type (int, string, etc.). For now we only have integers so we don't need
    any types, but soon we'll add booleans and we'll need to be able to
    distinguish between integer and boolean expressions. Later on we'll add more
    types. *)

let typ_to_str (_t : typ) : string = failwith "FILL IN"

type param
(** A parameter appearing an an argument list. This will be needed once we add
    functions. *)

(** Get the name of a parameter. *)
let param_name : param -> id = fun _ -> failwith "Undefined: param_name"

(** Represent a parameter as a string. *)
let param_to_str : param -> string = fun _ -> failwith "Undefined: param_to_str"

type params = param list
(** The list parameters to a function. *)

(** Represent a set of parameters as a string. *)
let params_to_str (ps : params) : string =
  String.concat " " (List.map param_to_str ps)

(** Literals. For now we have only integer literals. *)
type constant = CInt of int

(** Represent a literal as a string. *)
let constant_to_str : constant -> string = function CInt i -> string_of_int i

(** Binary operations. *)
type binop =
  | BAdd  (** + *)
  | BSub  (** - *)
  | BMul  (** * *)
  | BDiv  (** / *)
  | BMod  (** mod *)

(** Represent a binary operation as a string. *)
let binop_to_str : binop -> string = function
  | BAdd -> "+"
  | BSub -> "-"
  | BMul -> "*"
  | BDiv -> "/"
  | BMod -> "mod"

(** Unary operation. *)
type unop = UNegate  (** ~ *)

(** Represent a unary operation as a string. *)
let unop_to_str : unop -> string = function UNegate -> "~"

(** OCaml-lite expressions. *)
type expr =
  | EApp of expr * expr  (** e1 e2 *)
  | EBinop of expr * binop * expr  (** e1 <op> e2 *)
  | EUnop of unop * expr  (** <op> e *)
  | EVar of id  (** x *)
  | EConst of constant  (** c *)

(** Represent an expression as a string. *)
let rec expr_to_str : expr -> string = function
  | EApp (e1, e2) -> "(" ^ expr_to_str e1 ^ ") (" ^ expr_to_str e2 ^ ")"
  | EBinop (l, o, r) ->
      "(" ^ expr_to_str l ^ ") " ^ binop_to_str o ^ " (" ^ expr_to_str r ^ ")"
  | EUnop (o, a) -> unop_to_str o ^ " (" ^ expr_to_str a ^ ")"
  | EVar v -> v
  | EConst c -> constant_to_str c

(** A top-level binding. We don't have bindings in the language yet, but my test
    code is more consistent if we artificially wrap expressions in a single
    top-level binding from the beginning. *)
type binding = BLet of id * params * typ option * expr

(** Represent a binding as a string. *)
let binding_to_str : binding -> string = function
  | BLet (id, ps, mty, e) ->
      let tystr =
        match mty with None -> "" | Some _ -> failwith "No types yet!"
      in
      "let " ^ id ^ " " ^ params_to_str ps ^ tystr ^ " = (" ^ expr_to_str e
      ^ ")"

type program = binding list
(** An OCaml-lite program. *)

(** Represent a program as a string. *)
let program_to_str (p : program) : string =
  String.concat ";; " (List.map binding_to_str p)
