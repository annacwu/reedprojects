open Ast

exception TypeError of string

(**********************************************************
 * Setup - data structures for the typechecking algorithm *
 **********************************************************)

type context = (id * typ) list
type constraints = (typ * typ) list

let vartype_to_str : (id * typ) -> string = function
  | (id, t) -> "(" ^ id ^ ", " ^ typ_to_str t ^ ")"

let ctype_to_str : (typ * typ) -> string = function 
  | (t1, t2) -> "(" ^ typ_to_str t1 ^ ", " ^ typ_to_str t2 ^ ")" 

let context_to_str (_c : context) : string = String.concat " " (List.map vartype_to_str _c)

let constraints_to_str (_cs : constraints) : string = String.concat " " (List.map ctype_to_str _cs) 

(***************
 * Helper functions *
 ***************)

(* helper to generate new type variables *)
let fresh_var =
  let next_index = ref 0 in
  fun () ->
    next_index := !next_index + 1;
    "t" ^ string_of_int !next_index

let rec filter p ls = match ls with
  | [] -> []
  | h :: t -> if p h then h :: filter p t else filter p t

(* helper to find free variables *)
let rec free_vars (t : typ) : id list = 
  (* let _ = print_endline("checking for free variables in " ^ typ_to_str t) in *)
  match t with 
  | Mono(id) -> 
    (* let _ = print_endline("returning " ^ id) in  *)
  [id]
  | Func(t1, t2) -> free_vars t1 @ free_vars t2
  | Tup(t1, t2) -> free_vars t1 @ free_vars t2
  (* we don't want to say that one of the free variables is the one in the polytype *)
  | Poly(id, t) -> filter (fun v -> v <> id) (free_vars t)
  | _ -> []
  
let rec remove_duplicates (lst: 'a list) : 'a list = match lst with
  | [] -> lst
  | head :: tail -> head :: remove_duplicates (List.filter ((<>) head) tail)


(***************
 * Unification *
 ***************)
let rec unify (cs : constraints) : constraints = 
  (* let _ = print_endline("trying to unify these constraints: " ^ constraints_to_str cs) in *)
  match cs with
    | [] -> 
      (* let _ = print_endline("returning empty") in  *)
      []
    | (typ1, typ2) :: rest -> 
      let t1 = replace_poly typ1 in 
      let t2 = replace_poly typ2 in 
      (match t1, t2 with 
      | Mono(id1), Mono(id2) -> if id1 = id2 then unify rest else let sub = unify (map id1 (Mono(id2)) rest) in (Mono(id1), Mono(id2)) :: sub
      | Mono(id1), t -> 
        if occurs_check id1 t
        then 
          (* let _ = print_endline("failed in occurs check") in *)
          raise (TypeError "Infinite unification error") 
        else let sub = unify (map id1 t rest) in (Mono(id1), t) :: sub
      | t, Mono(id2) ->  
        if occurs_check id2 t 
        then 
          (* let _ = print_endline("failed in occurs check") in *)
          raise (TypeError "Infinite unification error") 
        else let sub = unify (map id2 t rest) in (Mono(id2), t) :: sub
      | Func(x1, y1), Func(x2, y2) -> unify (rest @ [(x1, x2) ; (y1, y2)])
      | Tup(x1, y1), Tup(x2, y2) -> unify (rest @ [(x1, x2) ; (y1, y2)])
      | t1, t2 -> 
        if t1 = t2 then unify rest 
        else 
          (* let _ = print_endline("failed in t1=t2") in *)
          raise (TypeError ("Cannot unify types " ^ typ_to_str t1 ^ " and " ^ typ_to_str t2))
        )

and replace_poly (t : typ) : typ = match t with 
  | Poly(x, t) -> 
    let new_var = Mono(fresh_var()) in 
    replace_poly (map_help x new_var t)
  | _ -> t


and map_help (var: id) (rep: typ) (t : typ) : typ = match t with
  | Mono(id) -> if id = var then rep else Mono(id) 
  | Func(x, y) -> Func(map_help var rep x, map_help var rep y)
  | Tup(x, y) -> Tup(map_help var rep x, map_help var rep y) 
  | _ -> t

and map (var: id) (rep: typ) (cs: constraints) : constraints = match cs with
    | (t1, t2) :: rest -> 
      let m1 = map_help var rep t1 in
      let m2 = map_help var rep t2 in
      (m1, m2) :: map var rep rest
    | _ -> []

and occurs_check (var : id) (t : typ) : bool = match t with
    | Mono(id) -> if var = id then true else false
    | Func(x, y) -> if not (occurs_check var x) then occurs_check var y else true
    | Tup(x, y) -> if not (occurs_check var x) then occurs_check var y else true 
    | _ -> false



(* helper to figure out if the given monotype has a mapping *)
let rec sub_help (id : id) (constraints : constraints) : typ option =
    match constraints with
    | [] -> None  
    | (t1, t2) :: rest ->
      (match t1 with
      | Mono(v) -> 
        if v = id then Some t2 
        else sub_help id rest 
      | _ -> sub_help id rest  
      )

let rec substitute (t : typ) (sub : constraints) : typ = 
  (* let _ = print_endline("substituting " ^ typ_to_str t) in *)
  match t with
  | Mono(id) -> 
    let rep = sub_help id sub in 
    (match rep with 
    (* recursively replace it a bunch *)
    | Some t -> 
      substitute t sub
    (* it didn't have a replacement so it is the same *)
    | None -> Mono(id) 
    )
  | Func(t1, t2) -> 
    Func(substitute t1 sub, substitute t2 sub)
  | Tup(t1, t2) -> 
    Tup(substitute t1 sub, substitute t2 sub)
  | _ -> t 

let rec check_free_context (env: context) (id : id) : bool = 
  (* let _ = print_endline("checking if " ^ id ^ " is free in " ^ context_to_str env) in *)
  match env with
  | [] -> false
  | (_, t) :: rest -> 
    if List.mem id (free_vars t ) then true 
    else check_free_context rest id

let rec check_all_context (env: context) (free: id list) : bool = match free with 
    | [] -> false
    | v :: rest -> 
      if check_free_context env v then true else check_all_context env rest

let rec generalize (free_vars : id list) (t : typ) : typ = match free_vars with
  | [] -> t
  | v :: rest -> 
    let term = generalize rest t in 
    Poly(v, term)

(*************************
 * Constraint Generation *
 *************************)

(* helper to get the new context with the type of each param in it *)
let rec typecheck_params (_ps : params) (env: context) : context = 
  (* let _ = print_endline("params: " ^ params_to_str _ps) in *)
  match _ps with
  | [] -> env
  | Param(id, Some t) :: rest -> 
    (* let _ = print_endline("param: " ^ param_to_str (Param(id, t))) in *)
    let new_env: context = (id, t) :: env in
    typecheck_params rest new_env 
  | Param(id, None) :: rest -> 
    let new_env: context = (id, Mono(fresh_var())) :: env in
    typecheck_params rest new_env 

(* helper to build up the function type for a variable *)
let rec make_function_typ (_ps : params) (acc: typ) (env: context) : typ = match _ps with
    [] -> 
      (* let _ = print_endline("returning function type: " ^ typ_to_str (acc)) in  *)
    acc
  | Param(_, Some t) :: rest -> 
    (* let _ = print_endline("adding function param: " ^ param_to_str (Param(id, Some t))) in
    let _ = print_endline("making function type: " ^ typ_to_str (acc)) in  *)
    (Func(t, make_function_typ rest acc env))
  | Param(id, None) :: rest -> 
    (* let _ = print_endline("adding function param: " ^ param_to_str (Param(id, None))) in
    let _ = print_endline("making function type: " ^ typ_to_str (acc)) in  *)
    let var_type = List.assoc_opt id env in (match var_type with
    | Some t -> 
      (* let _ = print_endline("creating type: " ^ typ_to_str (Func(t, make_function_typ rest acc env))) in *)
      (Func(t, make_function_typ rest acc env))
    | None -> 
      (* let _ = print_endline("could not find: " ^ id ^ " in " ^ context_to_str env) in  *)
      raise (TypeError ("could not find " ^ id ^ " in context")))

   

let rec typecheck_expr (_e : expr) (env: context) : constraints * typ = match _e with
  | EApp(_e1,_e2) -> 
    let c1, t1 = typecheck_expr _e1 env in
    let c2, t2 = typecheck_expr _e2 env in
    let res = Mono(fresh_var()) in
    let ftype = Func(t2, res) in 
    let cs : constraints = c1 @ c2 @ [(t1, ftype)] in cs, res 
  | EBinop(_l, _o, _r) -> 
    let c1, tleft = typecheck_expr _l env in
    let c2, tright = typecheck_expr _r env in
    (match _o with
      (* put in these two constraints: [(t0 *EVEN IF ITS A SPECIFIC TYPE*, int) (t1, int)]  because then something like bool ~ int should fail in unification*)
      | BAdd -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Int 
      | BSub -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Int 
      | BMul -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Int 
      | BDiv -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Int 
      | BMod -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Int 
      | BAnd -> let cs : constraints = c1 @ c2 @ [(tleft, Bool);(tright, Bool)] in cs, Bool 
      | BOr -> let cs : constraints = c1 @ c2 @ [(tleft, Bool);(tright, Bool)] in cs, Bool 
      | BLt -> let cs : constraints = c1 @ c2 @ [(tleft, Int);(tright, Int)] in cs, Bool 
      | BEq -> let cs : constraints = c1 @ c2 @ [(tleft, tright)] in cs, Bool 
      | BCat -> let cs : constraints = c1 @ c2 @ [(tleft, String);(tright, String)] in cs, String
      )
  | EUnop(_o, _a) -> 
    let c, arg = typecheck_expr _a env in (
      match _o with
      | UNegate -> let cs : constraints = c @ [(arg, Int)] in cs, Int
      | UNot -> let cs : constraints = c @ [(arg, Bool)] in cs, Bool
    )
  | EVar(v) -> 
    let t = List.assoc v env in 
    (* need fresh variables for polytype when looking it up *)
    let gen = replace_poly t in 
    [], gen
  | EConst(_c) -> (match _c with 
    | CInt _ -> [], Int
    | CBool _ -> [], Bool
    | CUnit -> [], Unit
    | CString _ -> [], String
    )
  | ETup(_e1, _e2) -> 
    let c1, t1 = typecheck_expr _e1 env in
    let c2, t2 = typecheck_expr _e2 env in 
    (c1 @ c2, Tup(t1, t2))
  | ECond(_e1, _e2, _e3) -> 
    let c1, cond = typecheck_expr _e1 env in
    let c2, body = typecheck_expr _e2 env in
    let c3, elsebody = typecheck_expr _e3 env in
    let cs : constraints = c1 @ c2 @ c3 @ [(body, elsebody); (cond, Bool)] in cs, body
  | ELet(_id, _ps, _t, _e1, _e2) -> 
    let cs, ftype = typecheck_expr (EAnon(_ps, _t, _e1)) env in
    (* steps: 
      typecheck parameters to put their variable types in the context
      evaluate e1, which should generate the constraints for the types
      we have to unify the constraints
      and then generalize (assign poly type when possible w free var stuf) and thats the return type of the let binding
      *)
      let map = unify(cs) in 
      (* substitute the type we want to finish unifying into environment and type *)
      let sub_env = List.map (fun (id, t) -> (id, substitute t map)) env in
      (* let _ = print_endline("mapping: " ^ constraints_to_str map) in  *)
      let sub_type = substitute ftype map in 
      (* let _ = print_endline("got subtype: " ^ typ_to_str sub_type) in *)
      let free_v = remove_duplicates (free_vars sub_type) in 
      (* see if the variable is free in the context, and if it is (returned true) then add to context as is *)
      let new_env = if (check_all_context env free_v) 
        then let temp : context = (_id, sub_type) :: sub_env in temp 
      (* otherwise, add the generalized type to context *)
      else let temp : context = (_id, generalize free_v sub_type) :: sub_env in temp in
      let c2, t = typecheck_expr _e2 new_env in c2, t 
  | ELetRec(_id, _ps, _t, _e1, _e2) -> 
      let rec_type = (match _t with
        | Some t -> t  
        | None -> Mono(fresh_var())) 
      (* don't eanon for rec because its a little different with the recursive call *)
      in let ps_env = typecheck_params _ps env in  
      let func_type = make_function_typ _ps rec_type ps_env in
      let func_env = (_id, func_type) :: ps_env in 
      let c1, body_type = typecheck_expr _e1 func_env in
      let cs = c1 @ [(body_type, rec_type)] in
      let map = unify(cs) in
      (* let _ = print_endline("mapping: " ^ constraints_to_str map) in  *)
      (* substitute the type we want to finish unifying *)
      let sub_env = List.map (fun (id, t) -> (id, substitute t map)) env in
      let sub_type = substitute func_type map in 
      (* let _ = print_endline("got subtype: " ^ typ_to_str sub_type) in *)
      let free_v = remove_duplicates (free_vars sub_type) in 
      (* see if the variable is free in the context, and if it is (returned true) then add to context as is *)
      let new_env = if (check_all_context func_env free_v) 
        then let temp : context = (_id, sub_type) :: sub_env in temp 
      (* otherwise, add the generalized type to context *)
      else let temp : context = (_id, generalize free_v sub_type) :: sub_env in temp in
      let c2, t = typecheck_expr _e2 new_env in c2, t 
  | EAnon(_ps, _t, _e) -> 
    (* let _ = print_endline("typechecking params") in *)
    let ps_env = typecheck_params _ps env in 
    (* let _ = print_endline("typechecking expression") in *)
    let cs, t2 = typecheck_expr _e ps_env in 
    (* let _ = print_endline("making function type, starting from: " ^ typ_to_str t2) in *)
    let ftype = make_function_typ _ps t2 ps_env in 
    (* let _ = print_endline("got function type: " ^ typ_to_str ftype) in  *)
    (match _t with 
      | Some t -> 
        (* let _ = print_endline("got function type: " ^ typ_to_str ftype) in   *)
        cs @ [(t2, t)], ftype
      | None -> 
         (* let _ = print_endline("got here " ^ typ_to_str ftype) in  *)
        cs, ftype)

(*********************************
 * Top-level type inference code *
 *********************************)

let rec typecheck (p : program) (env: context): unit =
  let ready_env = ("print_string", Func (String, Unit)) :: ("string_of_int", Func(Int, String)) :: ("int_of_string", Func (String, Int)) :: env in
  match p with
  | BLet(id, ps, typ, expr) :: rest -> 
    (* let _ = print_endline("\n binding: " ^ binding_to_str (BLet(id, ps, typ, expr))) in *)
    let cs, ftype = typecheck_expr (EAnon(ps, typ, expr)) ready_env in
    let map = unify(cs) in
    (* let _ = print_endline("mapping: " ^ constraints_to_str map) in  *)
    let sub_env = List.map (fun (id, t) -> (id, substitute t map)) env in
    (* let _ = print_endline("mapping: " ^ constraints_to_str map) in  *)
    let sub_type = substitute ftype map in 
    (* let _ = print_endline("got subtype: " ^ typ_to_str sub_type) in *)
    let free_v = remove_duplicates (free_vars sub_type) in 
    (* see if the variable is free in the context, and if it is (returned true) then add to context as is *)
    let new_env = if (check_all_context env free_v) 
      then let temp : context = (id, sub_type) :: sub_env in temp 
    (* otherwise, add the generalized type to context *)
    else let temp : context = (id, generalize free_v sub_type) :: sub_env in temp in
    let res = typecheck rest new_env in res
  | BLetRec(id, ps, typ, expr) :: rest -> 
    let rec_type = (match typ with
      | Some t -> t  
      | None -> Mono(fresh_var())) 
    in let ps_env = typecheck_params ps ready_env in  
    let func_env = (id, make_function_typ ps rec_type ps_env) :: ps_env in 
    let c1, ftype = typecheck_expr expr func_env in
    let cs = c1 @ [(ftype, rec_type)] in
    let map = unify(cs) in
    let sub_env = List.map (fun (id, t) -> (id, substitute t map)) env in
    let sub_type = substitute ftype map in 
    (* let _ = print_endline("got subtype: " ^ typ_to_str sub_type) in *)
    let free_v = remove_duplicates (free_vars sub_type) in 
    (* see if the variable is free in the context, and if it is (returned true) then add to context as is *)
    let new_env = if (check_all_context env free_v) 
      then let temp : context = (id, sub_type) :: sub_env in temp 
    (* otherwise, add the generalized type to context *)
    else let temp : context = (id, generalize free_v sub_type) :: sub_env in temp in
    let res = typecheck rest new_env in res
  | [] -> ()