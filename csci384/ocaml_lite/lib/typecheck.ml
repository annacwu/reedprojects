open Ast

exception TypeError of string

(**********************************************************
 * Setup - data structures for the typechecking algorithm *
 **********************************************************)

type context = (id * typ option) list

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

let rec typecheck_expr (_e : expr) (env: context) : typ = match _e with
  | EApp(_e1,_e2) -> 
    let t1 = typecheck_expr _e1 env in
    let t2 = typecheck_expr _e2 env in
    (match t1 with 
    | Func(String, Unit) -> if t2 = String then Unit else raise (TypeError "print_string expects string")
    | Func(Int, String) -> if t2 = Int then String else raise (TypeError "string_of_int expects int")
    | Func(String, Int) -> if t2 = String then Int else raise (TypeError "int_of_string expects string")
    | _ -> raise (TypeError "No other functions yet"))
  | EBinop(_l, _o, _r) -> 
    let tleft = typecheck_expr _l env in 
    let tright = typecheck_expr _r env in
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
      | BCat -> if tleft = String && tright = String then String else raise (TypeError "Expected type string")
      )
  | EUnop(_o, _a) -> 
    let arg = typecheck_expr _a env in (
      match _o with
      | UNegate -> if arg = Int then Int else raise (TypeError "Expected a bool")
      | UNot -> if arg = Bool then Bool else raise (TypeError "Expected a bool")
    )
  | EVar(v) -> let t = List.assoc v env in (match t with Some typ -> typ | _ -> raise (TypeError "Undefined variable"))
  | EConst(_c) -> (match _c with 
    | CInt _ -> Int
    | CBool _ -> Bool
    | CUnit -> Unit
    | CString _ -> String
    )
  | ECond(_e1, _e2, _e3) -> 
    let cond = typecheck_expr _e1 env in
    let body = typecheck_expr _e2 env in
    let elsebody = typecheck_expr _e3 env in
    if cond = Bool && body = elsebody then body else raise (TypeError "Incorrect type for if expression" )
  | ELet(_id, _, _t, _e1, _e2) -> (match _t with
    | None -> 
      let t1 = typecheck_expr _e1 env in 
      let new_env: context = (_id, Some t1) :: env in
      let t2 = typecheck_expr _e2 new_env in t2
    | Some typ -> 
      let t1 = typecheck_expr _e1 env in 
      if t1 = typ 
      then 
      let new_env: context = (_id, _t) :: env in
      let t2 = typecheck_expr _e2 new_env in t2
      else raise (TypeError "Unexpected expression type") )


let rec typecheck (p : program) (env: context): unit =
  let ready_env = ("print_string", Some (Func (String, Unit))) :: ("string_of_int", Some (Func (Int, String))) :: ("int_of_string", Some (Func (String, Int))) :: env in
  match p with
  (* | [ BLet (_, _, _, EApp (_, EApp (_, _expr))) ] -> let _ = typecheck_expr _expr env in () *)
  | BLet(id, [], typ, expr) :: rest -> (match typ with
    | None -> 
      let t = typecheck_expr expr ready_env in 
      let new_env: context = (id, Some t) :: ready_env in
      let res = typecheck rest new_env in res
    | Some t-> 
      (* let _ = print_endline("\n expr type: " ^ typ_to_str (typecheck_expr expr env)) in  *)
      (* let _ = print_endline("manual type: " ^ typ_to_str t ^ "\n") in  *)
      let t1 = typecheck_expr expr ready_env in 
      if t1 = t
      then let new_env: context = (id, typ) :: ready_env in
      let res = typecheck rest new_env in res
      else 
        (* let _ = print_endline("failing in typecheck with the t1=t else") in  *)
        raise (TypeError "Unexpected expression type") ) 
  | [] -> ()
  | _ -> raise (TypeError "Unexpected program type structure")