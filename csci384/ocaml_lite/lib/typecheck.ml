open Ast

exception TypeError of string

(**********************************************************
 * Setup - data structures for the typechecking algorithm *
 **********************************************************)

type context = (id * typ option) list

let vartype_to_str : (id * typ option) -> string = function
  | (id, Some t) -> "(" ^ id ^ ", " ^ typ_to_str t ^ ")"
  | _ -> failwith "should have types rn"

let context_to_str (_c : context) : string = String.concat " " (List.map vartype_to_str _c)

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

(* helper to get the new context with the type of each param in it *)
let rec typecheck_params (_ps : params) (env: context) : context = match _ps with
  | [] -> env
  | Param(id, t) :: rest -> 
    (* let _ = print_endline("param: " ^ param_to_str (Param(id, t))) in *)
    let new_env: context = (id, t) :: env in
    typecheck_params rest new_env 

(* helper to build up the function type for a variable *)
let rec make_function_typ (_ps : params) (acc: typ) : typ = match _ps with
    [] -> acc
  | Param(_, Some t) :: rest -> 
    (* let _ = print_endline("adding function param: " ^ param_to_str (Param(id, Some t))) in
    let _ = print_endline("making function type: " ^ typ_to_str (acc)) in  *)
    (Func(t, make_function_typ rest acc))
  | _ -> failwith "expecting params to be typed for now"

let rec typecheck_expr (_e : expr) (env: context) : typ = match _e with
  | EApp(_e1,_e2) -> 
    let t1 = typecheck_expr _e1 env in
    let t2 = typecheck_expr _e2 env in
    (match t1 with 
    | Func(t3, t4) -> if t2 = t3 then t4 else  raise (TypeError ("Expected type " ^ typ_to_str t3))
    | _ -> raise (TypeError "Expected function type"))
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
  | EVar(v) -> 
    let t = List.assoc v env in 
    (match t with 
      | Some typ -> typ 
      | _ -> raise (TypeError "Undefined variable"))
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
  | ELet(_id, _ps, _t, _e1, _e2) -> 
    let ps_env = typecheck_params _ps env in
    (match _t with
    | None -> 
      let t1 = typecheck_expr _e1 ps_env in 
      let new_env: context = (_id, Some t1) :: ps_env in
      let t2 = typecheck_expr _e2 new_env in t2
    | Some typ -> 
      let t1 = typecheck_expr _e1 ps_env in 
      (* if there are parameters, we know it is a function *)
      if _ps <> [] then 
        let func_env = (_id, Some (make_function_typ _ps typ)) :: ps_env in
        if t1 = typ 
        then 
        let t2 = typecheck_expr _e2 func_env in t2
        else raise (TypeError "Unexpected expression type") 
      else if t1 = typ then 
      let new_env: context = (_id, _t) :: ps_env in
      let t2 = typecheck_expr _e2 new_env in t2
      else raise (TypeError "Unexpected expression type") )
  | ELetRec(_id, _ps, _t, _e1, _e2) -> 
    let ps_env = typecheck_params _ps env in (* this is lowk irrelevant rn bc no untyped rec calls *)
    (match _t with
    | None -> 
      let t1 = typecheck_expr _e1 ps_env in 
      let new_env: context = (_id, Some t1) :: ps_env in
      let t2 = typecheck_expr _e2 new_env in t2
    | Some typ ->
      let func_env = (_id, Some (make_function_typ _ps typ)) :: env in
      let ps_env = typecheck_params _ps func_env in 
      let t1 = typecheck_expr _e1 ps_env in 
      if t1 = typ then 
        let t2 = typecheck_expr _e2 ps_env in t2
        else raise (TypeError "Unexpected expression type") )
  | EAnon(_ps, _t, _e) -> 
    let ps_env = typecheck_params _ps env in 
    let t2 = typecheck_expr _e ps_env in (match _t with 
      | None -> failwith("Should require type for now")
      | Some t1 -> if t2 = t1 then make_function_typ _ps t1 else raise (TypeError ("Expected " ^ typ_to_str t1)))


let rec typecheck (p : program) (env: context): unit =
  let ready_env = ("print_string", Some (Func (String, Unit))) :: ("string_of_int", Some (Func (Int, String))) :: ("int_of_string", Some (Func (String, Int))) :: env in
  match p with
  | BLet(id, ps, typ, expr) :: rest -> 
    (* let _ = print_endline("\n binding: " ^ binding_to_str (BLet(id, ps, typ, expr))) in *)
    let ps_env = typecheck_params ps ready_env in
    (match typ with
    | None -> 
      let t = typecheck_expr expr ps_env in 
      let new_env: context = (id, Some t) :: ps_env in
      let res = typecheck rest new_env in res
    | Some t-> 
      (* let _ = print_endline("\n expr type: " ^ typ_to_str (typecheck_expr expr ps_env)) in 
      let _ = print_endline("manual type: " ^ typ_to_str t ^ "\n") in  *)
      let t1 = typecheck_expr expr ps_env in 
      if ps <> [] then 
        (* let _ = print_endline("goes in ps <> [] next ") in *)
        let func_env = (id, Some (make_function_typ ps t)) :: ps_env in
        if t1 = t then let res = typecheck rest func_env in res else raise (TypeError ("Expected type " ^ typ_to_str t)) 
      else if t1 = t then
        (* let _ = print_endline("goes in t=t1 here next ") in  *)
        let new_env: context = (id, typ) :: ps_env in
        let res = typecheck rest new_env in res
        else 
        (* let _ = print_endline("failing in typecheck with the t1=t else") in  *)
        raise (TypeError "Unexpected expression type") ) 
  | BLetRec(id, ps, typ, expr) :: rest -> 
    let ps_env = typecheck_params ps env in
    (match typ with
    | None -> 
      let t = typecheck_expr expr ps_env in 
      let new_env: context = (id, Some t) :: ps_env in
      let res = typecheck rest new_env in res
    | Some t-> 
       let func_env = (id, Some (make_function_typ ps t)) :: ps_env in
       let ps_env = typecheck_params ps func_env in
      (* let _ = print_endline("\n expr type: " ^ typ_to_str (typecheck_expr expr ps_env)) in  *)
      (* let _ = print_endline("manual type: " ^ typ_to_str t ^ "\n") in  *)
      let t1 = typecheck_expr expr ps_env in 
      if t1 = t then 
        let res = typecheck rest func_env in res
        else (* let _ = print_endline("failing in typecheck with the t1=t else") in  *)
        raise (TypeError "Unexpected expression type") ) 
  | [] -> ()