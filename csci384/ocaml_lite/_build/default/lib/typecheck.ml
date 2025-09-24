open Ast

exception TypeError of string

(**********************************************************
 * Setup - data structures for the typechecking algorithm *
 **********************************************************)

(* You'll want to fill in some code here once we get to bindings. *)

(***************
 * Unification *
 ***************)

(* Unification will come up once we add polymorphism. *)

(*************************
 * Constraint Generation *
 *************************)

(* Also a unification section. *)

(*********************************
 * Top-level type inference code *
 *********************************)

let typecheck_expr (_e : expr) : typ =
  failwith "FILL IN: typecheck an expression"

let typecheck (p : program) : unit =
  match p with
  | [ BLet (_, _, _, EApp (_, EApp (_, _expr))) ] ->
      failwith
        "FILL IN: call typecheck_expr and make sure it returns an integer"
  | _ -> failwith "Unexpected program format"
