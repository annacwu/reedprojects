open OUnit2
open Ocaml_lite.Ast
open Ocaml_lite.Parser

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

let constant_tests =
  "constants"
  >::: List.map make_expr_ast_str
         [
           ("true", failwith "Describe true in your AST types", "true");
           ("false", failwith "Describe false in your AST types", "false");
         ]

let precedence_tests =
  "precedence"
  >::: List.map make_expr_str_str
         [
           ( "not higher prec than = (left)",
             "(not true) = false",
             "not true = false" );
           ("+ higher prec than <", "1 < (2 + 3)", "1 < 2 + 3");
           ("= higher prec than &&", "true && (1 = 2)", "true && 1 = 2");
           ( "&& higher prec than ||",
             "false || (true && true)",
             "false || true && true" );
         ]

let associativity_tests =
  "associativity"
  >::: List.map make_expr_str_str
         [
           ("and", "(true && true) && true", "true && true && true");
           ("or", "(true || true) || true", "true || true || true");
         ]
       @ List.map expr_expect_failure
           [
             ("comparison", "1 < 2 < 3");
             ("equal", "1 = 2 = 3");
             ("comp-equal", "1 = 2 < 3");
           ]

let expr_basic_tests =
  "basic"
  >::: List.map make_expr_ast_str
         [
           ( "if",
             failwith "Describe this expression in your AST types",
             "if true then 1 else 2" );
         ]

let keyword_group_tests =
  "keyword grouping"
  >::: List.map make_expr_str_str
         [
           ("if-op", "if true then 1 else (1 + 2)", "if true then 1 else 1 + 2");
           ("op-if", "1 + (if true then 2 else 3)", "1 + if true then 2 else 3");
         ]

let bool_expr_tests =
  "boolean expressions"
  >::: [ constant_tests; associativity_tests; precedence_tests ]

let if_tests = "if expressions" >::: [ expr_basic_tests; keyword_group_tests ]
