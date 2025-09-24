open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Ast

let constant_tests =
  "constants"
  >::: List.map make_expr_ast_str
         [
           ("unit", failwith "Fill in with representation of unit value", "()");
           ("unit with space", failwith "Fill in with unit value", "(  )");
         ]

let precedence_tests =
  "precedence"
  >::: List.map make_expr_str_str
         [
           ( "^ higher prec than =",
             "\"ab\" = (\"a\" ^ \"b\")",
             "\"ab\" = \"a\" ^ \"b\"" );
         ]

let expr_basic_tests =
  "basic"
  >::: List.map make_expr_ast_str
         [
           ( "let",
             failwith "Fill in with ast representation of code",
             "let x = 1 in x" );
           ("variable", EVar "x", "x");
         ]

let expr_tests =
  "expressions" >::: [ constant_tests; expr_basic_tests; precedence_tests ]

let types =
  "types"
  >::: List.map make_type_ast_str
         [
           ("int", failwith "AST for 'int'", "int");
           ("bool", failwith "AST for 'bool'", "bool");
           ("string", failwith "AST for 'string'", "string");
           ("unit", failwith "AST for 'unit", "unit");
         ]

let typed_exprs =
  "expressions"
  >::: List.map make_expr_ast_str
         [
           ( "let no args",
             failwith "AST representation of the code below",
             "let x : int = 2 in x" );
         ]
       @ List.map expr_expect_failure [ ("let type", "let f : = 2 in f + 1") ]

let type_tests = "type suite" >::: [ types; typed_exprs ]

let top_level_let =
  "top-level lets"
  >::: List.map make_bind_ast_str
         [
           ( "let no params no type",
             BLet ("x", [], None, EConst (CInt 1)),
             "let x = 1" );
         ]

let binding_tests = "bindings" >::: [ top_level_let ]

let parse_tests =
  "test suite for parsing" >::: [ expr_tests; type_tests; binding_tests ]
