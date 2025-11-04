open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Ast
open Ocaml_lite.Parser

let associativity_tests =
  "associativity"
  >::: List.map make_expr_str_str [ ("app", "(x y) z", "x y z") ]

let expr_tests = "expressions" >::: [ associativity_tests ]

let types =
  "types"
  >::: List.map make_type_ast_str
         [ ("function", failwith "AST for type int -> bool", "int -> bool") ]

let typed_exprs =
  "expressions"
  >::: List.map make_expr_ast_str
         [
           ( "let with args",
             failwith "AST representation of the code below",
             "let f x y = x in f 2 false" );
         ]
       @ List.map expr_expect_failure
           [ ("function missing arg", "let f : -> int = 2 in f") ]

let type_tests = "type suite" >::: [ types; typed_exprs ]

let top_level_let =
  "top-level lets"
  >::: List.map make_bind_ast_str
         [
           ( "let with params and type",
             failwith "AST for the code below",
             "let f x = x + 1" );
         ]

let binding_tests = "bindings" >::: [ top_level_let ]

let program_tests =
  "top-level programs"
  >::: [
         ( "fake factorial" >:: fun _ ->
           assert_equal ~printer:program_to_str
             (failwith "AST for the program below")
             (parse
                ("let f x = if x < 0 then \"neg\" else \"pos\";;"
               ^ "let fact x = if x = 1 then x else x * (x - 1);;"
               ^ "let _ = f (fact 2);;")) );
       ]

let parse_tests =
  "test suite for parsing"
  >::: [ expr_tests; type_tests; binding_tests; program_tests ]
