(** This module compiles OCaml-lite code to bytecode for the associated VM. *)

open Ast

exception CompileError of string
(** Generate a compile-time error. The only case where this happens currently is
    when a constructor is not applied to data. *)

(** Constants for different bytecode instructions. *)
module Codes = struct
  let const = 0x07
  let sconst = 0x0F
  let add = 0x20
  let sub = 0x21
  let mul = 0x22
  let div = 0x23
  let neg = 0x24
  let eq = 0x25
  let neq = 0x26
  let leq = 0x27
  let lt = 0x28
  let band = 0x29
  let bor = 0x2A
  let not = 0x2B
  let bmod = 0x2C
  let concat = 0x2D
  let br = 0x30
  let jump = 0x31
  let close = 0x40
  let call = 0x41
  let ret = 0x42
  let closerec = 0x43
  let over = 0x50
  let pop1 = 0x51
  let env = 0x52
  let cons = 0x60
  let gettag = 0x61
  let decons = 0x62
  let print = 0xF0
  let stoi = 0xF1
  let itos = 0xF2
  let error = 0xF3
end

type context = (id * int) list

(** Compile an expression. *)
let rec compile_expr (ex : expr) (sp: int) (env: context) : bytes =
  match ex with
  | EApp (e1, e2) -> compile_app e1 e2 sp env
  | EBinop (e1, op, e2) -> compile_binop e1 op e2 sp env
  | EUnop (op, e) -> compile_unop op e sp env
  | EVar v -> compile_var v env
  | EConst c -> compile_const c 
  | ECond(e1,e2,e3) -> compile_cond e1 e2 e3 sp env
  | ELet(id, _, t, e1,e2) -> compile_bind id t e1 e2 sp env

and compile_app (_e1: expr) (_e2: expr) (sp: int) (env: context) : bytes = 
  let func = compile_expr _e1 sp env in
  let arg = compile_expr _e2 (sp) env in
  Bytes.cat arg func

and compile_var (v: id) (env: context) : bytes = match v with 
  | "print_string" -> let op = Bytes.create 1 in let () = Bytes.set_uint8 op 0 Codes.print in op
  | "string_of_int" -> let op = Bytes.create 1 in let () = Bytes.set_uint8 op 0 Codes.itos in op
  | "int_of_string" -> let op = Bytes.create 1 in let () = Bytes.set_uint8 op 0 Codes.stoi in op
  | _ -> 
    let off = 
      (* let _ = print_endline("getting var: " ^ v) in *)
      List.assoc v env in
    let o = Bytes.create 2 in 
    let () = Bytes.set_uint8 o 0 Codes.over in 
    let () = Bytes.set_uint8 o 1 off in o

and compile_bind (_id: id) (_t: typ option) (_e1: expr) (_e2: expr) (sp: int) (env: context) : bytes =
  let bind = compile_expr _e1 sp env in
  let new_env = (_id, sp) :: env in 
  let ex = compile_expr _e2 (sp+1) new_env in
  let res = Bytes.cat bind ex in res

(** Compile a conditional statement *)
and compile_cond (_e1: expr) (_e2: expr)(_e3: expr) (sp: int) (env: context) : bytes =
  let cond = compile_expr _e1 sp env in
  let tbody = compile_expr _e2 sp env in
  let ebody = compile_expr _e3 (sp + 1) env in
  (* extend the condition to allow for a const [offset for instructions skipped] and br instruction *)
  let if_instr = Bytes.extend cond 0 10 in
  let () = Bytes.set_uint8 if_instr (Bytes.length cond) Codes.const in
  (* +10 to skip the next offset const and jump as well as the body *)
  let () = Bytes.set_int64_be if_instr (Bytes.length cond + 1) (Int64.of_int (Bytes.length ebody + 10)) in
  let () = Bytes.set_uint8 if_instr (Bytes.length cond + 9) Codes.br in
  (* concat to add the else body instructions  *)
  let tmp = Bytes.cat if_instr ebody in
  (* extend the else body to add the br instruction *)
  let else_instr = Bytes.extend tmp 0 10 in
  let () = Bytes.set_uint8 else_instr (Bytes.length tmp) Codes.const in
  let () = Bytes.set_int64_be else_instr (Bytes.length tmp + 1) (Int64.of_int (Bytes.length tbody)) in
  let () = Bytes.set_uint8 else_instr (Bytes.length tmp + 9) Codes.jump in
  (* concat to append the then body instructions *)
  let res = Bytes.cat else_instr tbody in
  res

(** Compile a binary operation. *)
and compile_binop (_e1 : expr) (_op : binop) (_e2 : expr) (sp: int) (env: context) : bytes =
  let left = compile_expr _e1 sp env in
  let right = compile_expr _e2 (sp + 1) env in
  let binary = Bytes.cat left right in
  let res = Bytes.extend binary 0 1 in
  match _op with
    | BAdd -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.add in
      res
    | BSub -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.sub in
      res
    | BMul -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.mul in
      res
    | BDiv -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.div in
      res
    | BMod -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.bmod in
      res
    | BAnd -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.band in
      res
    | BOr -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.bor in
      res
    | BLt -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.lt in
      res
    | BEq -> 
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.eq in
      res
    | BCat ->
      let () = Bytes.set_uint8 res (Bytes.length binary) Codes.concat in
      res
    
(** Compile a unary operation. *)
and compile_unop (_op : unop) (_e : expr) (sp: int) (env: context) : bytes =
  let const = compile_expr _e sp env in
  let res = Bytes.extend const 0 1 in
  match _op with
    | UNegate -> 
      let () = Bytes.set_uint8 res (Bytes.length const) Codes.neg in
      res
    | UNot ->
      let () = Bytes.set_uint8 res (Bytes.length const) Codes.not in
      res
  
(** Compile a constant value. *)
and compile_const (_c : constant) : bytes =
  let const = Bytes.create 9 in
  let () = Bytes.set_uint8 const 0 Codes.const in
  match _c with
    | CInt i -> 
      let () = Bytes.set_int64_be const 1 (Int64.of_int i) in
      const
    | CBool true ->
      let () = Bytes.set_int64_be const 1 (Int64.of_int 1) in
      const
    | CBool false ->
      let () = Bytes.set_int64_be const 1 (Int64.of_int 0) in
      const
    | CUnit ->
      let () = Bytes.set_int64_be const 1 (Int64.of_int 0) in
      const
    | CString s ->
      let stringbytes = Bytes.of_string s in
      let sconst = Bytes.extend stringbytes 1 1 in 
      let () = Bytes.set_uint8 sconst 0 Codes.sconst in
      sconst


(** Compile an OCaml-lite program. *)
let compile (prog : program) : bytes =
  let res =
    let rec comp_binding (src: program) (bc: bytes) (sp: int) (env: context) : bytes = 
    match src with
    (* | [ BLet (_, [], _, EApp (EVar ps, EApp (EVar soi, expr))) ] ->
        if ps <> "print_string" || soi <> "string_of_int" then
          failwith "Unexpected program structure"
        else
          let bs = compile_expr expr 0 [] in
          let r = Bytes.extend bs 0 2 in
          let () = Bytes.set_uint8 r (Bytes.length bs) Codes.itos in
          let () = Bytes.set_uint8 r (Bytes.length bs + 1) Codes.print in
          r *)
    | BLet(id, [], _, expr) :: rest -> 
      let e = compile_expr expr 0 env in 
      let new_bc = Bytes.cat bc e in 
      let new_env: context = (id, sp) :: env in 
      if rest = [] then new_bc else comp_binding rest new_bc (sp + 1) new_env 
      (* greg says it is right to just go thorugh each binding and compile it and then 
        then add it to the context with an updated stack pointer and that should be
        all you have to do there *)
    | _ -> failwith "Unexpected program structure"
  in comp_binding prog Bytes.empty 0 [] in
  (* Write out the bytecode magic number. *)
  let magic_number = Bytes.of_string "ReedPLDI" in
  (* Write out the number of functions. *)
  let num_fns_code = Bytes.create 8 in
  let () = Bytes.set_int64_be num_fns_code 0 (Int64.of_int 1) in
  (* Add the pointer to the main code. *)
  let main_ptr_code = Bytes.create 8 in
  let () = Bytes.set_int64_be main_ptr_code 0 (Int64.of_int 24) in
  (* Concatenate all the pieces. *)
  Bytes.concat Bytes.empty [ magic_number; num_fns_code; main_ptr_code; res ]
