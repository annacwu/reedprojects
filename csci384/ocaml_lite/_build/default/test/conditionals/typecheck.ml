open OUnit2
open Ocaml_lite_test_utils.Utils

let constants =
  "constants"
  >::: List.map tc_expr
         [
           ("int", "2", Some (failwith "Write 'int' in your AST types"));
           ("true", "true", Some (failwith "Write 'bool' in your AST types"));
           ("false", "false", Some (failwith "Write 'bool' in your AST types"));
         ]

let operators =
  "operators"
  >::: List.map tc_expr
         [
           ("plus pass", "3 + 4", Some (failwith "Fill in 'int' in AST"));
           ("plus arg fail", "2 + true", None);
           ("minus pass", "3 - 4", Some (failwith "Fill in 'int' in AST"));
           ("minus arg fail", "2 - true", None);
           ("lt pass", "3 < 4", Some (failwith "Fill in 'bool' in AST"));
           ("lt arg fail", "() < 5", None);
           ("eq pass int", "3 = 4", Some (failwith "Fill in 'bool in AST"));
           ( "eq pass bool",
             "true = true",
             Some (failwith "Fill in 'bool' in AST") );
           ("eq arg fail", "1 = ()", None);
         ]

let if_expr =
  "if"
  >::: List.map tc_expr
         [
           ( "pass",
             "if true then 2 else 3",
             Some (failwith "Fill in 'int' in AST") );
           ("bad condition", "if 1 then 2 else 3", None);
         ]

let expressions = "expressions" >::: [ constants; operators ]
