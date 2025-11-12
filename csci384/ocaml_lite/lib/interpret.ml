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

and context = (id * value) list
(** A context in which to evaluate an expression. This will become relevant when
    we add let bindings to the language. *)

(** Represent a value as a string. *)
let value_to_str : value -> string = function 
  | VInt i -> string_of_int i
  | VBool b -> string_of_bool b
  | VUnit -> "()"
  | VString s -> s
  | VBuiltin -> "built in"



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
        | _ -> raise (RuntimeError "No other functions yet"))
      | _ -> raise (RuntimeError "No other functions yet"))
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
      (* let _ = print_endline(v) in *)
      let v1 = List.assoc v env in v1
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
  | ELet(_id, _, _, _e1, _e2) -> 
    let v1 = interp_expr env _e1 in 
    (* let _ = print_endline("let id: " ^ _id ^ " and value: " ^ value_to_str v1) in  *)
    let new_env: context = (_id, v1) :: env in
    let v2 = interp_expr new_env _e2 in v2

(** Interpret a top-level let binding or type definition. *)
let interp_binding (env : context) : binding -> context = function
  | BLet(_id, _, _, _e1) -> 
    (* let _ = print_endline("let e: " ^ expr_to_str _e1) in   *)
    let v1 = interp_expr env _e1 in 

    let new_env: context = (_id, v1) :: env in let _ = print_endline(value_to_str v1) in new_env

(** Interpret a program. This just means interpreting all top-level bindings in
    order. *)
let rec interpret (prog : program) (env: context) : unit =
  match prog with 
  | head :: tail -> 
    let res = interp_binding env head in interpret tail res
  | [] -> ()
  (* match prog with
  | BLet(_, [], _, _) :: rest ->
      let t = interp_expr env  in 
      let new_env: context = (id, t) :: env in
      let res = interpret rest new_env in res  
  | [] -> ()
  | _ -> raise (RuntimeError "Unexpected program structure")
  (* match prog with *)
  | [ BLet (_, [], _, EApp (EVar ps, EApp (EVar soi, expr))) ] -> (
      if ps <> "print_string" || soi <> "string_of_int" then
        failwith "Unexpected program structure"
        else match interp_expr [] expr with
          | VInt i -> print_int i
          | VUnit -> print_string "()"
          | VBool b -> print_string(string_of_bool b)
          | VString s -> print_string s
      )
  | _ -> failwith "Unexpected program structure" *)