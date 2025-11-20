(** Interpret an OCaml-lite program. *)

open Ast

(** A value is the result of evaluting a program. *)
type value = 
  | VInt of int (** An integer value. *)
  | VBool of bool (** A boolean value. *)
  | VUnit (** A unit value. *)
  | VString of string (** A string value. *)
  | VBuiltin 
  | VClosure of string * expr * context * string option

(** A context in which to evaluate an expression. *)
and context = (id * value) list


let rec var_to_str : (id * value) -> string = function
  | (id, v) -> "(" ^ id ^ ", " ^ value_to_str v ^ ")"

and context_to_str (_c : context) : string = String.concat " " (List.map var_to_str _c)

(** Represent a value as a string. *)
and value_to_str : value -> string = function 
  | VInt i -> string_of_int i
  | VBool b -> string_of_bool b
  | VUnit -> "()"
  | VString s -> s
  | VBuiltin -> "built in"
  | VClosure(x, e , c, r) -> (match r with 
    | None -> "let " ^ x ^ " = " ^ expr_to_str e ^ "in " ^ context_to_str c 
    | Some s -> "let " ^ s ^ " " ^ x ^ " = " ^ expr_to_str e ^ "in " ^ context_to_str c )


exception RuntimeError of string
(** A program may throw a runtime error due to bad typing. This typically
    shouldn't happen in programs which pass typechecking, but such programs can
    still throw runtime errors if there are no matching branches in a match
    expression. *)

(** A list of builtin functions which are handled specially. *)
let builtins : id list = [ "string_of_int"; "int_of_string"; "print_string" ]

(** Interpret an expression. *)
let rec interp_expr (env : context) : expr -> value = function
  | EApp (_e1, _e2) -> 
      let v1 = interp_expr env _e1 in 
      let v2 = interp_expr env _e2 in 
      (match v1 with 
        | VBuiltin -> (match _e1, v2 with
          | EVar "print_string", VString s -> let () = print_string s in VBuiltin
          | EVar "string_of_int", VInt i -> let x = string_of_int i in VString(x)
          | EVar "int_of_string", VString s -> let x = int_of_string s in VInt(x)
          | _ -> raise (RuntimeError "No other built in functions"))
        | VClosure(x, e, cxt, rflag) -> 
            let new_env = (x, v2) :: cxt in 
            let final_env = (match rflag with
              | None -> new_env
              | Some s -> (s, v1) :: new_env)
            in interp_expr final_env e
        | _ -> raise (RuntimeError "Expected closure"))
  | EBinop (_l, _o, _r) -> 
    let left = interp_expr env _l in
    let right = interp_expr env _r in
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
        | BEq, VString s1, VString s2 -> VBool(s1 = s2)
        | BCat, VString s1, VString s2 -> VString(s1 ^ s2)
        | _ -> raise (RuntimeError "Uncaught binary operator type error")
          )
  | EUnop (_o, _a) -> 
    let arg = interp_expr env _a in
    (match _o, arg with
        | UNegate, VInt i -> VInt(-(i))
        | UNot, VBool true -> VBool(false)
        | UNot, VBool false -> VBool(true)
        | _ -> raise (RuntimeError "Uncaught unary operator type error"))
  | EVar v -> 
    if List.mem v builtins then VBuiltin 
    else 
      let v1 = List.assoc_opt v env in( match v1 with
        | None -> raise( RuntimeError (v ^ " not found in " ^ context_to_str env))
        | Some v -> v)
  | EConst c -> (match c with 
    | CInt i -> VInt i
    | CBool b -> VBool b
    | CUnit -> VUnit
    | CString s -> VString s)
  | ECond(_e1, _e2, _e3) -> let cond = interp_expr env _e1 in 
    (match cond with
    | VBool true -> interp_expr env _e2
    | VBool false -> interp_expr env _e3
    | _ -> raise (RuntimeError "Uncaught conditional type error"))
  | ELet(_id, _ps, _t, _e1, _e2) -> (match _ps with
    | [] -> let v1 = interp_expr env _e1 in 
    let new_env: context = (_id, v1) :: env in
    let v2 = interp_expr new_env _e2 in v2
    | ps -> let v1 = curry_anon ps _e1 env None in 
    interp_expr ((_id, v1) :: env) _e2)
  | ELetRec(_id, _ps, _t, _e1, _e2) -> (match _ps with 
    | [] -> let v1 = interp_expr env _e1 in 
    let new_env: context = (_id, v1) :: env in
    let v2 = interp_expr new_env _e2 in v2
    | ps -> let v1 = curry_anon  ps _e1 env (Some _id)in 
    interp_expr ((_id, v1) :: env) _e2)
  | EAnon(_ps, _, _e) -> curry_anon _ps _e env None

(* Helper to curry a let binding into anonymous functions *)
and curry_anon (ps: params) (e: expr) (env: context) (rname: string option) : value =
  match ps with
    | [] -> interp_expr env e
    | [Param(x, _)] ->
        VClosure(x, e, env, rname)
    | Param(x, _) :: rest ->
        let curried = EAnon(rest, None, e) in
        VClosure(x, curried, env, rname)

(** Interpret a top-level let binding or type definition. *)
let interp_binding (env : context) : binding -> context = function
  | BLet(_id, _ps, _, _e1) -> (match _ps with
    | [] -> let v1 = interp_expr env _e1 in 
      let new_env: context = (_id, v1) :: env in new_env
    | ps -> let v1 = curry_anon  ps _e1 env None in 
    (_id, v1) :: env) 
  | BLetRec(_id, _ps, _, _e1) -> (match _ps with
    | [] -> let v1 = interp_expr env _e1 in 
      let new_env: context = (_id, v1) :: env in new_env
    | ps -> let v1 = curry_anon  ps _e1 env (Some _id) in 
    (_id, v1) :: env) 

(** Interpret a program. This just means interpreting all top-level bindings in
    order. *)
let rec interpret (prog : program) (env: context) : unit =
  match prog with 
  | head :: tail -> 
    let res = interp_binding env head in interpret tail res
  | [] -> ()