open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Interpret

let constants =
  "constants"
  >::: List.map interp_expr_test
         [
           ("string", VString "ab", "\"ab\"");
           ("unit", VUnit, "()");
         ]

let operators =
  "operators"
  >::: List.map interp_expr_test
         [
           ( "eq string",
             VBool true,
             "\"a\" = \"a\"" );
         ]

let let_exprs =
  "let expressions"
  >::: List.map interp_expr_test
         [ ("nested let", VInt 4, "let x = 2 in let y = x in x + y") ]

let if_exprs =
  "if expressions"
  >::: List.map interp_expr_test
         [
           ( "let scope in if",
             VInt 1,
             "let x = 2 in if let x = 0 in x < 1 then 1 else 2" );
         ]

let expressions =
  "expressions" >::: [ constants; operators; let_exprs; if_exprs ]

let definitions =
  "definition and use"
  >::: List.map program_test
         [
           ( "basic definition",
             check_val "y" (VInt 3),
             "let x = 1;; let y = x + 2;;",
             "Couldn't refer to previously defined value" );
         ]

let interp_tests =
  "test suite for interpretation" >::: [ expressions; definitions ]
