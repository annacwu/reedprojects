(** Interpret an OCaml-lite program. *)

open Ast

(** A value is the result of evaluting a program. *)
type value = 
  | VInt of int (** An integer value. *)
  | VBool of bool
  | VUnit

(** Represent a value as a string. *)
let value_to_str : value -> string = function 
  | VInt i -> string_of_int i
  | VBool b -> string_of_bool b
  | VUnit -> "()"

type context = (id * value) list
(** A context in which to evaluate an expression. This will become relevant when
    we add let bindings to the language. *)

exception RuntimeError of string
(** A program may throw a runtime error due to bad typing. This typically
    shouldn't happen in programs which pass typechecking, but such programs can
    still throw runtime errors if there are no matching branches in a match
    expression. *)

(** A list of builtin functions which are handled specially. *)
let builtins : id list = [ "string_of_int"; "int_of_string"; "print_string" ]

(** Interpret an expression. *)
let rec interp_expr (a : context) : expr -> value = function
  | EApp (_, _) -> failwith "Unexpected program structure"
  | EBinop (_l, _o, _r) -> 
    let left = interp_expr a _l in
    let right = interp_expr a _r in
    (match _o, left, right with
        | BAdd, VInt i1, VInt i2 -> VInt(i1 + i2)
        | BSub, VInt i1, VInt i2 -> VInt(i1 - i2)
        | BMul, VInt i1, VInt i2 -> VInt(i1 * i2)
        | BDiv, VInt i1, VInt i2 -> VInt(i1 / i2)
        | BMod, VInt i1, VInt i2 -> VInt(i1 mod i2) 
        | BAnd, VBool b1, VBool b2 -> VBool(b1 && b2)
        | BOr, VBool b1, VBool b2 -> VBool(b1 || b2)
        | BLt, VInt i1, VInt i2 -> VBool(i1 < i2)
        | BEq, VBool b1, VBool b2 -> VBool(b1 = b2)
        | BEq, VInt i1, VInt i2 -> VBool(i1 = i2)
        | BEq, VUnit, VUnit -> VBool(true)
        | _ -> raise (RuntimeError "Uncaught binary operator type error")
          )
  | EUnop (_o, _a) -> 
    let arg = interp_expr a _a in
    (match _o, arg with
        | UNegate, VInt i -> VInt(-(i))
        | UNot, VBool true -> VBool(false)
        | UNot, VBool false -> VBool(true)
        | _ -> raise (RuntimeError "Uncaught unary operator type error"))
  | EVar _ -> failwith "No variables yet!"
  | EConst c -> (match c with 
    | CInt i -> VInt i
    | CBool b -> VBool b
    | CUnit -> VUnit)
  | ECond(_e1, _e2, _e3) -> let cond = interp_expr a _e1 in 
    (match cond with
    | VBool true -> interp_expr a _e2
    | VBool false -> interp_expr a _e3
    | _ -> raise (RuntimeError "Uncaught conditional type error"))

(** Interpret a top-level let binding or type definition. We don't have either
    of these yet. *)
let interp_binding (_ : context) : binding -> context = failwith "Undefined"

(** Interpret a program. This just means interpreting all top-level bindings in
    order. This function currently assumes that our program has exactly the form
    "let _ = print_string (string_of_int (<expr>)) ;;". *)
let interpret (prog : program) : unit =
  match prog with
  | [ BLet (_, [], _, EApp (EVar ps, EApp (EVar soi, expr))) ] -> (
      if ps <> "print_string" || soi <> "string_of_int" then
        failwith "Unexpected program structure"
        else match interp_expr [] expr with
          | VInt i -> print_int i
          | VUnit -> print_string "()"
          | VBool b -> print_string(string_of_bool b)
      )
  | _ -> failwith "Unexpected program structure"