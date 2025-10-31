open OUnit2
open Ocaml_lite.Lexer
open Ocaml_lite.Ast
open Ocaml_lite.Parser
open Ocaml_lite.Typecheck
open Ocaml_lite.Interpret

let make_expr_ast_str (inp : string * 'a * string) : test =
  let label, left, right = inp in
  label >:: fun _ ->
  assert_equal ~printer:expr_to_str left (fst (parse_enter expr right))

let make_expr_str_str (inp : string * string * string) : test =
  let label, left, right = inp in
  label >:: fun _ ->
  assert_equal ~printer:expr_to_str
    (fst (parse_enter expr left))
    (fst (parse_enter expr right))

let expr_expect_failure (inp : string * string) : test =
  let label, source = inp in
  label >:: fun _ ->
  try
    match parse_enter expr source with
    | _, [] -> assert_failure (source ^ " was parsed without error")
    | _, _ -> assert_bool "" true
  with
  | ParseError _ -> assert_bool "" true
  | e -> raise e

let make_type_ast_str (inp : string * typ * string) : test =
  let label, left, right = inp in
  label >:: fun _ ->
  assert_equal ~printer:typ_to_str left (fst (parse_enter typ right))

let make_bind_ast_str (inp : string * binding * string) : test =
  let label, left, right = inp in
  label >:: fun _ ->
  assert_equal ~printer:binding_to_str left (fst (parse_enter binding right))

let bind_expect_failure (inp : string * string) : test =
  let label, source = inp in
  label >:: fun _ ->
  try
    let _ = parse_enter binding source in
    assert_failure (source ^ " was parsed without error")
  with
  | ParseError _ -> assert_bool "" true
  | e -> raise e

let tc_expr (inp : string * string * typ option) : test =
  let label, prog, output = inp in
  label >:: fun _ ->
  try
    let p, r = parse_enter expr prog in
    match r with
    | [] -> (
        let t = typecheck_expr p [] in
        match output with
        | None -> assert_failure "Program typechecked but shouldn't have"
        | Some t2 -> assert_bool "Inferred the wrong type" (t == t2))
    | t :: _ ->
        raise (ParseError ("Expected end of input, got " ^ tok_to_str t))
  with
  | TypeError _ ->
      assert_bool "program failed typechecking" (Option.is_none output)
  | OUnitTest.OUnit_failure _ as e -> raise e
  | e -> raise e

let tc_program (inp : string * string * bool) : test =
  let label, prog, pass = inp in
  label >:: fun _ ->
  try
    let _ = typecheck (parse prog) [] in
    assert_bool "program typechecked but shouldn't have" pass
  with
  | TypeError _ -> assert_bool "program failed typechecking" (not pass)
  | OUnitTest.OUnit_failure _ as e -> raise e
  | e -> raise e

let forget_types (_e : expr) : expr =
  failwith "FILL IN: Convert the AST to a version without types"

let compare_value (v1 : value) (v2 : value) : bool =
  match (v1, v2) with 
    | VInt i1, VInt i2 -> i1 = i2
    | VBool b1, VBool b2 -> b1 = b2
    | VString v1, VString v2 -> v1 = v2
    | VUnit, VUnit -> true
    | VBuiltin, VBuiltin -> true
    | _ -> failwith "Unexpected value"

let interp_expr_test (inp : string * value * string) : test =
  let label, v, source = inp in
  label
  >: test_case ~length:OUnitTest.Short (fun _ ->
         assert_equal ~printer:value_to_str ~cmp:compare_value v
           (interp_expr [] (fst (parse_enter expr source))))

let expr_fail (label : string) (failure : string) (source : string) : test =
  label >:: fun _ ->
  try
    let _ = interp_expr [] (fst (parse_enter expr source)) in
    assert_failure failure
  with _ -> assert_bool "" true

let program_test (inp : string * (context -> bool) * string * string) : test =
  let label, check, source, msg = inp in
  label
  >: test_case ~length:OUnitTest.Short (fun _ ->
         let ctx = List.fold_left interp_binding [] (parse source) in 
         assert_bool msg (check ctx))

let check_val (x : string) (v : value) (ctx : context) : bool =
  match List.assoc_opt x ctx with Some t -> compare_value v t | None -> false
