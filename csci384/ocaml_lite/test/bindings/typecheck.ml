open OUnit2
open Ocaml_lite_test_utils.Utils

let constants =
  "constants"
  >::: List.map tc_program
         [
           ("string pass", "let x : string = \"test\";;", true);
           ("string fail", "let x : int = \"test\";;", false);
         ]

let operators =
  "operators"
  >::: List.map tc_program
         [
           ("plus pass", "let x : int = 3 + 4;;", true);
           ("minus arg fail", "let x : int = 2 - true;;", false);
           ("times ret fail", "let x : string = 2 * 1;;", false);
           ("lt pass", "let x : bool = 3 < 4;;", true);
           ("lt ret fail", "let x : unit = 5 < 4;;", false);
           ("eq pass int", "let x : bool = 3 = 4;;", true);
           ("eq arg fail", "let x : bool = 1 = ();;", false);
           ("concat arg fail", "let x : string = \"a\" ^ 1;;", false);
           ("or arg fail", "let x : bool = false || ();;", false);
           ("not ret fail", "let x : unit = not false;;", false);
         ]

let if_expr =
  "if" >::: List.map tc_program [ 
    ("if pass", "let x : bool = if 0 < 1 then false else true;;", true);
    ("if fail", "let x : int = if 0 < 1 then 2 else false;;", false);
]

let let_expr =
  "let"
  >::: List.map tc_program
         [
           ("pass int", "let x : int = let y = 2 in y;;", true);
           ( "fail type mismatch",
             "let x : string = let y : int = \"test\" in y;;",
             false );
         ]

let expressions = "expressions" >::: [ constants; operators; if_expr; let_expr ]

let builtins =
  "builtin functions"
  >::: List.map tc_program
         [
           ("string_of_int pass", "let x : string = string_of_int 12;;", true);
           ( "string_of_int arg fail",
             "let x : string = string_of_int false;;",
             false );
           ("string_of_int ret fail", "let x : int = string_of_int 3;;", false);
         ]

let tc_tests = "test suite for typechecking" >::: [ expressions; builtins ]
