open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Interpret

let let_exprs =
  "let expressions"
  >::: List.map interp_expr_test
         [
           ( "recursion",
             VInt 0,
             "let rec f x = if x = 0 then x else f (x - 1) in f 3" );
         ]

let lambda_exprs =
  "lambdas"
  >::: List.map interp_expr_test
         [
           ("closure", failwith "AST for the code below", "fun x => x");
           ("applying a closure", VInt 3, "(fun x => x + 2) 1");
           ( "context capture",
             VInt 3,
             "let f = let x = 1 in fun y => x + y in f 2" );
         ]

let definitions =
  "definition and use"
  >::: List.map program_test
         [
           ( "function definition",
             check_val "y" (VInt 6),
             "let f x = x + 2;; let y = f 4;;",
             "Couldn't refer to defined function" );
         ]

let recursion =
  "recursive functions"
  >::: List.map program_test
         [
           ( "recursive definition",
             check_val "y" (VInt 120),
             "let rec fact x = if x = 0 then 1 else x * fact (x - 1);; let y = \
              fact 5;;",
             "Couldn't run recursive function" );
         ]

let builtins =
  "built-in functions"
  >::: List.map program_test
         [
           ( "string_of_int",
             check_val "y" (failwith "Value for string 12"),
             "let y = string_of_int 12;;",
             "string_of_int failed" );
           ( "int_of_string",
             check_val "y" (VInt 12),
             "let y = int_of_string \"12\";;",
             "int_of_string failed" );
         ]

let interp_tests =
  "test suite for interpretation"
  >::: [
         lambda_exprs;
         definitions;
         builtins;
         expr_fail "int_of_string fail"
           "int_of_string succeeded on non-int string" "int_of_string \"abc\"";
       ]

let all_recur = "recursion" >::: [ let_exprs; recursion ]
