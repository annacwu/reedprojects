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

let rec typecheck_expr (_e : expr) : typ = match _e with
  | EApp(_,_) -> failwith "Unexpected expr"
  | EBinop(_l, _o, _r) -> 
    let tleft = typecheck_expr _l in 
    let tright = typecheck_expr _r in
    (match _o with
      | BAdd -> if tleft = Int && tright = Int then Int else raise (TypeError "Expected type int")
      | BSub -> if tleft = Int && tright = Int then Int else raise (TypeError  "Expected type int")
      | BMul -> if tleft = Int && tright = Int then Int else raise (TypeError  "Expected type int")
      | BDiv -> if tleft = Int && tright = Int then Int else raise (TypeError  "Expected type int")
      | BMod -> if tleft = Int && tright = Int then Int else raise (TypeError  "Expected type int")
      | BAnd -> if tleft = Bool && tright = Bool then Bool else raise (TypeError  "Expected type bool")
      | BOr -> if tleft = Bool && tright = Bool then Bool else raise (TypeError  "Expected type bool")
      | BLt -> if tleft = Int && tright = Int then Bool else raise (TypeError "Expected type int" )
      | BEq -> if tleft = tright then Bool else raise (TypeError "Cannot compare different types" )
      )
  | EUnop(_o, _a) -> 
    let arg = typecheck_expr _a in (
      match _o with
      | UNegate -> if arg = Int then Int else raise (TypeError "Expected a bool")
      | UNot -> if arg = Bool then Bool else raise (TypeError "Expected a bool")
    )
  | EVar(_) -> failwith "No variables yet!"
  | EConst(_c) -> (match _c with 
    | CInt _ -> Int
    | CBool _ -> Bool
    | CUnit -> Unit
    )
  | ECond(_e1, _e2, _e3) -> 
    let cond = typecheck_expr _e1 in
    let body = typecheck_expr _e2 in
    let elsebody = typecheck_expr _e3 in
    if cond = Bool && body = elsebody then body else raise (TypeError "Incorrect type for if expression" )

let typecheck (p : program) : unit =
  match p with
  | [ BLet (_, _, _, EApp (_, EApp (_, _expr))) ] ->
      failwith
        "FILL IN: call typecheck_expr and make sure it returns an integer"
  | _ -> failwith "Unexpected program format"